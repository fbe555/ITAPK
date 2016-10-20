//
// Created by felix on 10/17/16.
//

#ifndef CMS_CARGO_H
#define CMS_CARGO_H

#include <ratio>
#include <iostream>
#include <typeinfo>
#include <string.h>


namespace cm{

    //Constants
    const double BASE_LOAD_FACTOR = 1;
    const double LIQUID_LOAD_FACTOR = 0.5;
    const double LIVESTOCK_LOAD_FACTOR = 2;

    //Cargo struct
    class Cargo{
    public:
        typedef std::shared_ptr<Cargo> Ptr;
        Cargo(int w, int lt) : weight(w), loadTime(lt)
        {
        }

        virtual ~Cargo(){}

        const int weight;
        const int loadTime;
    };

    class Timber : public Cargo{
    public:
        Timber(int w): Cargo(w, BASE_LOAD_FACTOR*w){}
        ~Timber(){};
    };

    class Coal : public Cargo{
    public:
        Coal(int w): Cargo(w, BASE_LOAD_FACTOR*w){}
        ~Coal(){};
    };

    class Grains : public Cargo{
    public:
        Grains(int w): Cargo(w, BASE_LOAD_FACTOR*w){}
        ~Grains(){};
    };

    class Oil : public Cargo{
    public:
        Oil(int w): Cargo(w, LIQUID_LOAD_FACTOR*w){}
        ~Oil(){};
    };

    class Gasoline: public Cargo{
    public:
        Gasoline(int w): Cargo(w, LIQUID_LOAD_FACTOR*w){}
        ~Gasoline(){};
    };

    class Water: public Cargo{
    public:
        Water(int w): Cargo(w, LIQUID_LOAD_FACTOR*w){}
        ~Water(){};
    };

    class Sheep : public Cargo{
    public:
        Sheep(int w): Cargo(w, LIVESTOCK_LOAD_FACTOR*w){}
        ~Sheep(){};
    };

    class Cows : public Cargo{
    public:
        Cows(int w): Cargo(w, LIVESTOCK_LOAD_FACTOR*w){}
        ~Cows(){};
    };

    class Pigs : public Cargo{
    public:
        Pigs(int w): Cargo(w, LIVESTOCK_LOAD_FACTOR*w){}
        ~Pigs(){};
    };

    inline std::ostream& operator<<(std::ostream& out, Cargo const* cargo){
        out << cargo->weight << " kg of " << typeid(*cargo).name();
        out << " (" << cargo->loadTime << "s needed for loading)";
        return out;
    }

    /*******************************************************************
     **             TMP EVALUATION/CONDITION STRUCTS                  **
     *******************************************************************/
    template <typename CT>
    struct IS_LIQUID_CARGO{
        static const bool value = false;
    };

    template <>
    struct IS_LIQUID_CARGO<Oil>{
        static const bool value = true;
    };

    template <>
    struct IS_LIQUID_CARGO<Gasoline>{
        static const bool value = true;
    };

    template <>
    struct IS_LIQUID_CARGO<Water>{
        static const bool value = true;
    };


    template <typename CT>
    struct IS_LIVESTOCK_CARGO{
        static const bool value = false;
    };

    template <>
    struct IS_LIVESTOCK_CARGO<Sheep>{
        static const bool value = true;
    };

    template <>
    struct IS_LIVESTOCK_CARGO<Cows>{
        static const bool value = true;
    };

    template <>
    struct IS_LIVESTOCK_CARGO<Pigs>{
        static const bool value = true;
    };

    // is same type tmp boolean
    template<typename T, typename U>
    struct IS_SAME_TYPE{
        static const bool value = false;
    };

    template<typename T>
    struct IS_SAME_TYPE<T, T>{
        static const bool value = true;
    };

    template<typename T>
    struct IS_CARGO{
        static const bool value = std::is_base_of<Cargo, T>::value;
    };

    /*******************************************************************
     **                    TMP CARGOLIST STRUCTS                      **
     *******************************************************************/
    struct CL_NULL_ELEM{};

    template<typename H, typename... REST>
    struct CARGO_LIST{
        static_assert(IS_CARGO<H>::value, "CARGO_LISTS may only contain subclasses of cargo.");
        typedef H HEAD;
        typedef CARGO_LIST<REST...> TAIL;
    };

    template<typename H>
    struct CARGO_LIST<H>{
        static_assert(IS_CARGO<H>::value, "CARGO_LISTS may only contain subclasses of cargo.");
        typedef H HEAD;
        typedef CL_NULL_ELEM TAIL;
    };

    /*
    template<typename... CARGO_TYPES>
    struct Cargo_LIST{
        typedef CARGO_LIST<
    };

    template<typename first, typename ... rest>
    struct CARGO_LIST{
        typedef CL<first, CARGO_LIST<rest...>::LIST > LIST;
    };
     */

    /*******************************************************************
     **                      Cargolist utilities                      **
     *******************************************************************/

    template<typename CL, typename T>
    struct CL_CONTAINS{
        static const bool value = IS_SAME_TYPE<typename CL::LIST::HEAD, T>::value || CL_CONTAINS<typename CL::LIST::TAIL, T>::value;
    };

    template<typename T>
    struct CL_CONTAINS<CL_NULL_ELEM, T>{
        static const bool value = false;
    };

    template<typename CL>
    struct CL_NUM_ELEMS{
        static const int value = 1 + CL_NUM_ELEMS<typename CL::TAIL>::value;
    };

    template<>
    struct CL_NUM_ELEMS<CL_NULL_ELEM>{
        static const int value = 0;
    };

    //TODO: how about out of bounds?
    template<typename CL, int index>
    struct CL_GET_ELEM{
        typedef typename CL_GET_ELEM<typename CL::TAIL, index-1>::TYPE TYPE;
    };

    template<typename CL>
    struct CL_GET_ELEM<CL, 0>{
        typedef typename CL::HEAD TYPE;
    };

    template<typename CL>
    struct CL_RUNTIME_CONTAINS{
        template<typename T>
        static bool value(std::shared_ptr<T> c){
            return bool(dynamic_cast<typename CL::HEAD*>(c.get())) || CL_RUNTIME_CONTAINS<typename CL::TAIL>::value(c);
        }
    };

    template <>
    struct CL_RUNTIME_CONTAINS<CL_NULL_ELEM>{
        template<typename T>
        static bool value(std::shared_ptr<T> c){
            return false;
        }
    };

    template<typename CL, typename... EXPANDED>
    struct MAKE_BOOST_VARIANT{
        typedef typename MAKE_BOOST_VARIANT<typename CL::TAIL, typename CL::HEAD, EXPANDED...>::VARIANT_TYPE VARIANT_TYPE;
    };

    template<typename... EXPANDED>
    struct MAKE_BOOST_VARIANT<CL_NULL_ELEM, EXPANDED...>{
        typedef boost::variant<EXPANDED...> VARIANT_TYPE;
    };


};
#endif //CMS_CARGO_H
