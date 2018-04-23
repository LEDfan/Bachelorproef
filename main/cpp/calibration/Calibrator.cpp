#include "Calibrator.h"
#include "../../test/cpp/gtester/ScenarioData.h"
#include <sim/SimRunner.h>
#include <sim/Simulator.h>

#include <numeric>
#include <omp.h>
#include <random>

namespace calibration {

Calibrator::Calibrator()
    : results(), logger(stride::util::LogUtils::CreateCliLogger("calibration_logger", "calibration_log.txt"))
{
}

void Calibrator::Run()
{
        std::vector<std::string> cases = {"influenza_a", "influenza_b", "influenza_c", "measles_16", "r0_12"};

        //         logger->info("Starting to compute exact values for testcases");
        // #pragma omp parallel for
        //         for (unsigned int caseIdx = 0; caseIdx < cases.size(); ++caseIdx) {
        //                 auto       tag       = cases[caseIdx];
        //                 const auto d         = Tests::ScenarioData::Get(tag);
        //                 auto       config_pt = std::get<0>(d);
        //                 auto       runner    = stride::SimRunner::Create();
        //                 runner->Setup(config_pt);
        //                 runner->Run();
        //                 const unsigned int res = runner->GetSim()->GetPopulation()->GetInfectedCount();
        //                 logger->info("Found exact value for testcase {}: {}", tag, res);
        //         }

        unsigned int       count = 2;
        std::random_device rd;

        std::vector<std::random_device::result_type> seeds;

        for (unsigned int i = 0; i < count * cases.size(); i++) {
                seeds.push_back(rd());
        }

#pragma omp parallel for collapse(2)
        for (unsigned int caseIdx = 0; caseIdx < cases.size(); ++caseIdx) {
                for (unsigned int i = 0; i < count; i++) {
                        auto tag = cases[caseIdx];

                        const auto d         = Tests::ScenarioData::Get(tag);
                        auto       config_pt = std::get<0>(d);

                        auto seed = seeds[caseIdx * count + i];

                        config_pt.put("run.rng_seed", seed);
                        logger->info("Starting the testcase {}, run {} of {} using seed {}", tag, i, count, seed);
                        auto runner = stride::SimRunner::Create();
                        runner->Setup(config_pt);
                        runner->Run();

                        // Get the infected count
                        const unsigned int res = runner->GetSim()->GetPopulation()->GetInfectedCount();
                        logger->info("Finished running testcase {}, {} people were infected", tag, res);
#pragma omp critical
                        results[tag].push_back(res);
                }
        }
}

void Calibrator::PrintResults()
{
        for (auto& result_pair : results) {
                auto&       results = result_pair.second;
                std::string tag     = result_pair.first;
                double      sum     = std::accumulate(results.begin(), results.end(), 0.0);
                double      mean    = sum / results.size();

                // Inspired by https://stackoverflow.com/a/7616783
                std::vector<double> diff(results.size());
                std::transform(results.begin(), results.end(), diff.begin(), [mean](double x) { return x - mean; });
                double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
                double stdev  = std::sqrt(sq_sum / results.size());

                logger->info("Found mean {} and standard deviation {} for testcase {}", mean, stdev, tag);
        }
}

std::map<std::string, std::vector<unsigned int>> Calibrator::GetResults() { return results; }

} // namespace calibration
