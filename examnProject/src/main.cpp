#include <iostream>
#include <boost/signals2.hpp>
#include <thread>
#include "trains.hpp"
#include "station.hpp"
#include "CMS.hpp"
//#include "SimulationController.hpp"

int main() {
    typedef cm::TrainImpl<cm::Locomotive<1000>,
            cm::CARRIAGE_LIST<
                    cm::Carriage<100, cm::CARGO_LIST<cm::Grains> >//,
                    //cm::Carriage<100, cm::CARGO_LIST<cm::Water, cm::Oil> >
            > > Train1_t;
    Train1_t train;
    std::cout << std::to_string(train.canHold(std::make_shared<cm::Grains>(50)));
    return 0;
}