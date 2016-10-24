//
// Created by felix on 10/17/16.
//

#ifndef CMS_TRAINS_H
#define CMS_TRAINS_H

#include <boost/mpl/list.hpp>
#include <boost/mpl/bool.hpp>
#include <iostream>
#include <list>
#include <boost/asio/detail/shared_ptr.hpp>
#include <algorithm>
#include <boost/variant/variant.hpp>
#include <boost/any.hpp>
#include <thread>
#include <mutex>
#include "cargo.hpp"

namespace cm {
    //Train instance creation will have the following syntax
    //static Train::Ptr&& Train::getInstance<Locomotive<Capacity>, CarriageList<Carriage<CargoList<Cargo...> >...> >();

    class Train {
    public:
        typedef std::shared_ptr<Train> Ptr;

        Train() : _name("Default constructed train"), _id(nxt_id++), mut() {}

        Train(std::string name) : _name(name), _id(nxt_id++), mut() {}

        virtual ~Train() {}

        Train(const Train &t) : _name(t._name), _id(nxt_id++), mut() {}

        Train &operator=(const Train &t) {
            _name = (t._name);
            //TODO: consider static mutex for nxt_id
            _id = nxt_id++;
        }

        Train(const Train &&t) : _name(t._name), _id(t._id), mut() {}

        Train &&operator=(const Train &&t) {
            _name = (t._name);
            _id = t._id;
        }

        virtual bool canHold(Cargo::Ptr) = 0;

        virtual bool load(Cargo::Ptr) = 0;

        virtual Cargo::Ptr unload() = 0;

        virtual int getTotalWeight() = 0;

        virtual int getCapacity() = 0;

        virtual int calculatePossibleLoad(std::list<Cargo::Ptr>) = 0;

        int getID() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            return _id;
        }

        std::string getName() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            return _name;
        }

        bool isEmpty() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            return getTotalWeight() == 0;
        }

        bool isFull() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            return getTotalWeight() >= getCapacity();
        }

    protected:
        std::recursive_mutex mut;

    private:
        int _id;
        std::string _name;
        static int nxt_id;
    };

    inline std::ostream &operator<<(std::ostream &out, Train &train) {
        out << train.getName() << " (uid: " << train.getID() << ")";
        return out;
    }

    inline std::ostream &operator<<(std::ostream &out, Train::Ptr &train_ptr) {
        if (train_ptr) out << *train_ptr;
        else out << "!!empty train pointer!!";
        return out;
    }

    /*********************************************************************
     **                ASSERTION structs for carriage                   **
     *********************************************************************/

    //First assertion is that all elements are Cargo classes
    //TODO: fix all elements are cargo
    /*
    template<bool, typename CL>
    struct ALL_ELEMENTS_ARE_CARGO;

    template<typename CL>
    struct ALL_ELEMENTS_ARE_CARGO<true, CL>{
        static const bool value = ALL_ELEMENTS_ARE_CARGO<
                IS_CARGO<typename CL::HEAD>::value,
                typename CL::TAIL>::value;
    };
    template<>
    struct ALL_ELEMENTS_ARE_CARGO<true, CL_NULL_ELEM>{
        static const bool value = true;
    };

    template<typename CL>
    struct ASSERT_IS_CARGO{
        static const bool value = ALL_ELEMENTS_ARE_CARGO<IS_CARGO<typename CL::HEAD>::value, typename CL::TAIL>::value;
    };
    */

    //Second assertion says that if any of the Cargo types are liquid Cargo types, all must be.
    template<bool, typename E, typename L>
    struct ALL_OR_NONE_ARE_LIQUID;
    template<typename E, typename L>
    struct ALL_OR_NONE_ARE_LIQUID<true, E, L> {
        static const bool value = ALL_OR_NONE_ARE_LIQUID<
                IS_LIQUID_CARGO<E>::value == IS_LIQUID_CARGO<typename L::HEAD>::value,
                typename L::HEAD, typename L::TAIL>::value;
    };
    template<typename E>
    struct ALL_OR_NONE_ARE_LIQUID<true, E, CL_NULL_ELEM> {
        static const bool value = IS_LIQUID_CARGO<E>::value;
    };
    template<typename CL>
    struct ASSERT_TANKER_VALIDITY {
        static const bool value = ALL_OR_NONE_ARE_LIQUID<true, typename CL::HEAD, typename CL::TAIL>::value;
    };

    //Third assertion says that one may not mix different kinds of livestock in one carriage
    //OBS: in the current implementation CARGO_LIST<Sheep, Sheep> is illegal.
    template<bool prevLivestock, bool currLivestock, typename REST>
    struct ONLY_ONE_KIND_OF_LIVESTOCK;
    //TODO: Use CL_AND_CONDITION again using nested templates.
    template<bool currLivestock, typename REST>
    struct ONLY_ONE_KIND_OF_LIVESTOCK<false, currLivestock, REST> {
        static const bool hasLivestock = ONLY_ONE_KIND_OF_LIVESTOCK<
                currLivestock,
                IS_LIVESTOCK_CARGO<typename REST::HEAD>::value,
                typename REST::TAIL>::hasLivestock;
    };
    template<typename REST>
    struct ONLY_ONE_KIND_OF_LIVESTOCK<true, false, REST> {
        static const bool hasLivestock = ONLY_ONE_KIND_OF_LIVESTOCK<
                true,
                IS_LIVESTOCK_CARGO<typename REST::HEAD>::value,
                typename REST::TAIL>::hasLivestock;
    };
    template<bool currLivestock>
    struct ONLY_ONE_KIND_OF_LIVESTOCK<false, currLivestock, CL_NULL_ELEM> {
        static const bool hasLivestock = currLivestock;
    };
    template<>
    struct ONLY_ONE_KIND_OF_LIVESTOCK<true, false, CL_NULL_ELEM> {
        static const bool hasLivestock = true;
    };

    template<typename CL>
    struct ASSERT_LIVESTOCK_VALIDITY {
        static const bool value = ONLY_ONE_KIND_OF_LIVESTOCK<
                false,
                IS_LIVESTOCK_CARGO<typename CL::HEAD>::value,
                typename CL::TAIL>::hasLivestock;
    };


    //Carriage validity class:
    //TODO: allow for empty carriages
    //this struct instantiates the different assertion structs, and thussly will only compile if they are fulfilled.
    template<typename CL>
    struct IS_A_VALID_CARGO_LIST {
        //typedef ASSERT_IS_CARGO<CL> ACCEPTS_CARGO;
        typedef ASSERT_TANKER_VALIDITY<CL> IS_TANKER;
        typedef ASSERT_LIVESTOCK_VALIDITY<CL> HAS_LIVESTOCK;
    };

    //TODO: add a details namespace to remove clutter
    //Carriage:
    template<int cap, typename CL>
    struct Carriage {
        typedef boost::mpl::bool_<true> IS_CARRIAGE;

        typedef IS_A_VALID_CARGO_LIST<CL> META_INFO;

        typedef boost::mpl::int_<cap> CAPACITY;

        Carriage() : cargo(), carriage_cap(cap) {}

        ~Carriage() {}

        Carriage(const Carriage &other) : carriage_cap(other.carriage_cap) {
            cargo = other.cargo;
        }

        Carriage &operator=(const Carriage &other) {
            cargo = other.cargo;
            return *this;
        }

        Carriage(const Carriage &&other) : carriage_cap(other.carriage_cap) {
            cargo = std::move(other.cargo);
        }

        Carriage &operator=(const Carriage &&other) {
            cargo = std::move(other.cargo);
            return *this;
        }


        bool canHold(Cargo::Ptr c) const {
            int total = getTotalWeight();
            if (getTotalWeight() + c->weight > carriage_cap) return false;
            bool tmp = CL_RUNTIME_CONTAINS<CL>::value(c);
            if (tmp) return true;
            return false;
        }

        bool acceptableType(Cargo::Ptr c) const{

        }
        bool load(Cargo::Ptr c) {
            if (canHold(c)) {
                std::this_thread::sleep_for(std::chrono::seconds(c->loadTime));
                cargo.push_back(c);
                return true;
            } else return false;
        }

        Cargo::Ptr unload() {
            if (!cargo.empty()) {
                Cargo::Ptr tmp = cargo.back();
                cargo.pop_back();
                std::this_thread::sleep_for(std::chrono::seconds(tmp->loadTime));
                return tmp;
            } else return Cargo::Ptr();
        }

        int getTotalWeight() const {
            int load = 0;
            std::for_each(cargo.begin(), cargo.end(), [&load](Cargo::Ptr c) { load += c->weight; });
            return load;
        }

        int getCapacity() const {
            return carriage_cap;
        }

        std::list<Cargo::Ptr> cargo;
        const bool isTanker = META_INFO::IS_TANKER::value;
        const bool hasLivestock = META_INFO::HAS_LIVESTOCK::value;
        const int carriage_cap;

    };



    //TODO: add constraints on CARGO_LIST in carriage and CARRIAGE_LIST in train.

    template<typename H, typename... REST>
    struct CARRIAGE_LIST {
        static_assert(IS_SAME_TYPE<typename H::IS_CARRIAGE, boost::mpl::bool_<true> >::value,
                      "CARRIAGE_LISTS may only contain carriages.");
        typedef H HEAD;
        typedef CARRIAGE_LIST<REST...> TAIL;
    };

    template<typename H>
    struct CARRIAGE_LIST<H> {
        static_assert(IS_SAME_TYPE<typename H::IS_CARRIAGE, boost::mpl::bool_<true> >::value,
                      "CARRIAGE_LISTS may only contain carriages.");
        typedef H HEAD;
        typedef CL_NULL_ELEM TAIL;
    };

    template<int cap>
    struct Locomotive {
        typedef boost::mpl::bool_<true> IS_LOCOMOTIVE;
        static const int capacity = cap;
    };

    template<typename CL>
    struct CAPACITY_SUM {
        static_assert(CL::HEAD::IS_CARRIAGE::value, "CAPACITY_SUM only works for carriages");
        static const int value = CL::HEAD::CAPACITY::value + CAPACITY_SUM<typename CL::TAIL>::value;
    };

    template<>
    struct CAPACITY_SUM<CL_NULL_ELEM> {
        static const int value = 0;
    };


    //HAKON
    template<typename CL>
    class LoadVisitor : public LoadVisitor<typename CL::TAIL> {
    public:
        LoadVisitor(Cargo::Ptr c) : LoadVisitor<typename CL::TAIL>(c) {};

        virtual bool operator()(typename CL::HEAD &e) const {
            return e.load(cargo);
        }

        using LoadVisitor<typename CL::TAIL>::operator();
        using LoadVisitor<typename CL::TAIL>::cargo;
    };

    template<>
    struct LoadVisitor<CL_NULL_ELEM> : public boost::static_visitor<bool> {
    public:
        LoadVisitor(Cargo::Ptr c) : cargo(c) {}

        LoadVisitor(const LoadVisitor &other) : cargo(other.cargo) {}

        virtual bool operator()() const { return false; }

        Cargo::Ptr cargo;
    };


    //FELIX


    /**********************************************************************************
     **                          carriage visitors                                   **
     **********************************************************************************/
    template<typename CL>
    class CanHoldVisitor : public CanHoldVisitor<typename CL::TAIL> {
    public:
        CanHoldVisitor(Cargo::Ptr c) : CanHoldVisitor<typename CL::TAIL>(c) {};

        virtual bool operator()(typename CL::HEAD &e) const {
            return e.canHold(cargo);
        }

        using CanHoldVisitor<typename CL::TAIL>::operator();
        using CanHoldVisitor<typename CL::TAIL>::cargo;

    };

    template<>
    class CanHoldVisitor<CL_NULL_ELEM> : public boost::static_visitor<bool> {
    public:
        CanHoldVisitor(Cargo::Ptr c) : cargo(c) {}

        CanHoldVisitor(const CanHoldVisitor &other) : cargo(other.cargo) {}

        virtual bool operator()() const { return false; }

        Cargo::Ptr cargo;
    };

    template<typename CL>
    class GetTotalWeightVisitor : public GetTotalWeightVisitor<typename CL::TAIL> {
    public:
        virtual int operator()(typename CL::HEAD &e) const {
            return e.getTotalWeight();
        }

        using GetTotalWeightVisitor<typename CL::TAIL>::operator();
    };

    template<>
    class GetTotalWeightVisitor<CL_NULL_ELEM> : public boost::static_visitor<int> {
    public:
        virtual int operator()() const {
            return 0;
        }
    };

    template<typename CL>
    class UnloadVisitor : public UnloadVisitor<typename CL::TAIL> {
    public:
        virtual Cargo::Ptr operator()(typename CL::HEAD &e) const {
            return e.unload();
        }

        using UnloadVisitor<typename CL::TAIL>::operator();
    };

    template<>
    class UnloadVisitor<CL_NULL_ELEM> : public boost::static_visitor<Cargo::Ptr> {
    public:
        virtual Cargo::Ptr operator()() const { return Cargo::Ptr(); }
    };

    template<typename CL>
    class GetCapacityVisitor : public GetCapacityVisitor<typename CL::TAIL> {
    public:
        virtual int operator()(typename CL::HEAD &e) const {
            return e.getCapacity();
        }

        using GetCapacityVisitor<typename CL::TAIL>::operator();
    };

    template<>
    class GetCapacityVisitor<CL_NULL_ELEM> : public boost::static_visitor<int> {
    public:
        virtual int operator()() const {
            return 0;
        }
    };


    template<typename CL, typename T>
    struct CARRIAGE_LIST_INITIALIZER {
        static void initializeCarriageList(T &cl) {
            typename CL::HEAD elem;
            cl.push_back(elem);
            CARRIAGE_LIST_INITIALIZER<typename CL::TAIL, T>::initializeCarriageList(cl);
        }
    };


    template<typename T>
    struct CARRIAGE_LIST_INITIALIZER<CL_NULL_ELEM, T> {
        static void initializeCarriageList(T &cl) {}
    };

    //template<template<int> typename Locomotive L, template<typename H, typename... REST> typename CARRIAGE_LIST CL>
    template<typename LOCOMOTIVE, typename CARRIAGE_L>
    class TrainImpl : public Train {
    public:
        typedef typename MAKE_BOOST_VARIANT<CARRIAGE_L>::VARIANT_TYPE carriagevariant_t;
        typedef std::list<carriagevariant_t> carriagelist_t;

        TrainImpl(std::string name) : Train(name), carriages() {
            CARRIAGE_LIST_INITIALIZER<CARRIAGE_L, carriagelist_t>::initializeCarriageList(carriages);
        };

        //TODO: Rule of 5
        bool canHold(Cargo::Ptr c) {
            std::lock_guard<std::recursive_mutex> lock(mut);
            bool ch = false;
            std::for_each(carriages.begin(),
                          carriages.end(),
                          [&c, &ch](carriagevariant_t cv) {
                              ch = ch || boost::apply_visitor(CanHoldVisitor<CARRIAGE_L>(c), cv);
                          });
            return ch;
        }

        bool load(Cargo::Ptr c) {
            std::lock_guard<std::recursive_mutex> lock(mut);
            bool loaded = false;
            for (auto carriage: carriages) {
                loaded = boost::apply_visitor(LoadVisitor<CARRIAGE_L>(c), carriage);
            }
            return loaded;
        };

        Cargo::Ptr unload() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            Cargo::Ptr c;
            for (auto carriage: carriages) {
                c = boost::apply_visitor(UnloadVisitor<CARRIAGE_L>(), carriage);
                if (c) return c;
            }
            return c;
        }

        int getTotalWeight() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            int total = 0;
            for (auto carriage: carriages) {
                total += boost::apply_visitor(GetTotalWeightVisitor<CARRIAGE_L>(), carriage);
            }
            return total;
        }

        int getCapacity() {
            std::lock_guard<std::recursive_mutex> lock(mut);
            return capacity;
        }

        int calculatePossibleLoad(std::list<Cargo::Ptr> cargo) {
            std::lock_guard<std::recursive_mutex> lock(mut);
            assert(this->isEmpty());
            int total_loaded = 0;
            for (carriagevariant_t& carriage:carriages) {
                int loaded = 0;
                for (auto c:cargo) {
                    bool typeCompat = boost::apply_visitor(CanHoldVisitor<CARRIAGE_L>(c), carriage);
                    bool weightCompat = c->weight + loaded <= boost::apply_visitor(GetCapacityVisitor<CARRIAGE_L>(), carriage);
                    if(typeCompat && weightCompat) {
                        loaded += c->weight;
                    }
                }
                total_loaded += loaded;
            }
            return total_loaded;
        }

    private:
        static const int capacity = CAPACITY_SUM<CARRIAGE_L>::value;
        static_assert(LOCOMOTIVE::capacity >= capacity,
                      "The constructed train does not have a sufficiently strong locomotive to pull all it's carriages.");
        carriagelist_t carriages;
    };
}
#endif //CMS_TRAINS_H
