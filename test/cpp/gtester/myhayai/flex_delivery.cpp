/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2018, Kuylen E, Willem L, Broeckhove J
 *
 *  This software has been altered form the hayai software by Nick Bruun.
 *  The original copyright, to be found in the directory two levels higher
 *  still aplies.
 */
/**
 * @file
 * Header file for TestFactory.
 */

#include "flex_delivery.h"

#include "DeliveryMan.h"
#include "myhayai/Benchmark.h"
#include "myhayai/BenchmarkRunner.h"
#include "myhayai/Test.h"
#include "util/StringUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <memory>

using namespace std;
using namespace stride::util;
using boost::property_tree::ptree;
using namespace myhayai;

namespace Tests {

void flex_delivery()
{
        //---------------------------------------------------------------------------------------------
        // The "all lambda" flex approach. You register a factory (outer lambda) that, on each call,
        // produces a test (inner lambda).
        // This particular test only has a body. It sleeps and then lets the DeliveryMan do his job.
        //---------------------------------------------------------------------------------------------
        // clang-format off
        BenchmarkRunner::RegisterTest("FlexDelivery", "Flex1 distance=20", 5,
                                      []() {
                                              return Test(
                                                      []() {
                                                              DeliveryMan(1).DeliverPackage(20);
                                                      }
                                              );
                                      }
        );
        //clang-format on

        //---------------------------------------------------------------------------------------------
        // Used to check that the BenchmarkRunner can continue after exceptions thrown by tests.
        //---------------------------------------------------------------------------------------------
        // clang-format off
        BenchmarkRunner::RegisterTest("Exceptional", "Throwing runtime_error", 5,
                                      []() {
                                              return Test([]() {
                                                                  throw runtime_error("Exception from exceptional test.");
                                                          }
                                              );
                                      }
        );
        //clang-format on

        //---------------------------------------------------------------------------------------------
        // Roughly as above, except thatnow we do not define the factory in place,
        // and the test has a setup in addition to a body.
        //---------------------------------------------------------------------------------------------
        // clang-format off
        auto param2_factory = []() {
                auto p = make_shared<DeliveryMan>();
                return Test(
                        [p]() {
                                p->Sleep(3);
                                p->DeliverPackage(5);
                        },
                        [p]() { *p = DeliveryMan(2); }
                );
        };
        //clang-format on

        BenchmarkRunner::RegisterTest("FlexDelivery", "Flex2 - distance=5, speed=2", 5, param2_factory);

        // Register a test with disabled status.
        BenchmarkRunner::RegisterTest("FlexDelivery", "Flex2 - disabled", 5, param2_factory, InfoFactory(), true);

        //---------------------------------------------------------------------------------------------
        // Now we're cooking, with another indirection layer. the factory_builder builds test_factories.
        // It can accept parameters (which the factory cannot since it has to have a void() signature)
        // At registration time, the call to the factory builder the produces the factory.
        // Also we're using InfoFactory to (potentially) allow a Viewer to inject info in their output.
        //---------------------------------------------------------------------------------------------
        // clang-format off
        auto param_factory_builder = [](unsigned int distance, unsigned int duration, unsigned int speed) {
                return [distance, duration, speed]() {
                        auto p = make_shared<DeliveryMan>();
                        return Test(
                                [p, duration, distance]() {
                                        p->Sleep(duration);
                                        p->DeliverPackage(distance);
                                },
                                [p, speed]() { *p = DeliveryMan(speed); }
                        );
                };
        };
        //clang-format on

        for (unsigned int i = 1; i < 4; ++i) {
                BenchmarkRunner::RegisterTest("FlexDelivery", "Flex3 - case " + ToString(i), 15,
                                              param_factory_builder(1, 1, i),
                                              [i]() {
                                                      ptree pt;
                                                      pt.put("FlexDelivery.speed", i);
                                                      return pt;
                                              });
        }
        volatile auto somecondition = true;
        if (somecondition) {
                BenchmarkRunner::RegisterTest("FlexDelivery", "Flexconditional", 1, param_factory_builder(3, 10, 1),
                                              []() {
                                                      ptree pt;
                                                      pt.put("FlexDelivery.distance", 3);
                                                      return pt;
                                              });
        }
        BenchmarkRunner::RegisterTest("FlexDelivery", "Deluxe", 1, param_factory_builder(5, 20, 2));
}

}
