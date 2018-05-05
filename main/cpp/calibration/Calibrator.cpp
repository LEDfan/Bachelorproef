#include "Calibrator.h"
#include "../../test/cpp/gtester/ScenarioData.h"
#include <sim/Sim.h>
#include <sim/SimRunner.h>

#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>

namespace calibration {

Calibrator::Calibrator() : logger(stride::util::LogUtils::CreateCliLogger("calibration_logger", "calibration_log.txt"))
{
}

std::map<std::string, std::vector<unsigned int>> Calibrator::RunSingle(
    std::vector<std::pair<boost::property_tree::ptree, std::string>> configs) const
{
        std::map<std::string, std::vector<unsigned int>> results;
        logger->info("Starting to compute exact values for testcases");
#pragma omp parallel for
        for (unsigned int config = 0; config < configs.size(); config++) {
                auto pop    = stride::Population::Create(configs[config].first);
                auto runner = stride::SimRunner(configs[config].first, pop);
                runner.Run();
                const unsigned int res          = runner.GetSim()->GetPopulation()->GetInfectedCount();
                results[configs[config].second] = {res}; // TODO rework to use infectionViewer
        }
        return results;
}

std::map<std::string, std::vector<std::vector<unsigned int>>> Calibrator::RunMultiple(
    unsigned int count, std::vector<std::pair<boost::property_tree::ptree, std::string>> configs) const
{
        std::map<std::string, std::vector<std::vector<unsigned int>>> results;

        std::random_device rd;

        std::vector<std::random_device::result_type> seeds;

        for (unsigned int i = 0; i < count * configs.size(); i++) {
                seeds.push_back(rd());
        }

        for (const auto& config : configs) {
                results[config.second].resize(count);
        }
#pragma omp parallel for collapse(2)
        for (unsigned int config = 0; config < configs.size(); config++) {
                for (unsigned int i = 0; i < count; i++) {
                        auto tag       = configs[config].second;
                        auto config_pt = configs[config].first;
                        auto seed      = seeds[config * count + i];

                        config_pt.put("run.rng_seed", seed);
                        logger->info("Starting the testcase {}, run {} of {} using seed {}", tag, i, count, seed);
                        auto pop    = stride::Population::Create(config_pt);
                        auto runner = stride::SimRunner(config_pt, pop);
                        runner.Run();

                        // Get the infected count
                        const unsigned int res = runner.GetSim()->GetPopulation()->GetInfectedCount();
                        logger->info("Finished running testcase {}, {} people were infected", tag, res);
                        results[tag][i].push_back(res); // TODO rework to use infectionViewer
                }
        }
        return results;
}

void Calibrator::PrintStep(std::vector<unsigned int> results, std::string tag, unsigned int step) const
{

        auto p = FindMeanStdev(results);
        logger->info("Found mean {} and standard deviation {} for testcase {} at step {}", p.first, p.second, tag,
                     step);
}

std::pair<double, double> Calibrator::FindMeanStdev(std::vector<unsigned int> results) const
{
        double sum  = std::accumulate(results.begin(), results.end(), 0.0);
        double mean = sum / results.size();

        // Inspired by https://stackoverflow.com/a/7616783
        std::vector<double> diff(results.size());
        std::transform(results.begin(), results.end(), diff.begin(), [mean](double x) { return x - mean; });
        double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        double stdev  = std::sqrt(sq_sum / results.size());
        return std::make_pair(mean, stdev);
}

void Calibrator::PrintResults(std::map<std::string, std::vector<std::vector<unsigned int>>> results,
                              unsigned int                                                  step) const
{
        for (auto& result_pair : results) {
                std::vector<unsigned int> data;
                for (const auto& run : result_pair.second) {
                        if (run.size() - 1 < step) {
                                logger->error("This no such step in the simulation: {}", step);
                                continue;
                        }
                        data.push_back(run[step]);
                }
                PrintStep(data, result_pair.first, step);
        }
}

void Calibrator::PrintResults(std::map<std::string, std::vector<unsigned int>> results) const
{
        for (const auto& config : results) {
                logger->info("Found exact value for testcase {}: {}", config.first, *config.second.rbegin());
        }
}

void Calibrator::PrintResults(std::map<std::string, std::vector<unsigned int>> results, unsigned int step) const
{
        for (const auto& config : results) {
                logger->info("Found exact value for testcase {}: {} at step {}", config.first, config.second[step],
                             step);
        }
}

void Calibrator::PrintResults(std::map<std::string, std::vector<std::vector<unsigned int>>> results) const
{
        for (auto& result_pair : results) {
                if (result_pair.second.empty())
                        continue;
                std::vector<unsigned int> data;
                for (const auto& run : result_pair.second) {
                        if (run.empty())
                                continue;
                        data.push_back(*run.rbegin());
                }
                PrintStep(data, result_pair.first, result_pair.second[0].size() - 1);
        }
}
void Calibrator::WriteResults(std::map<std::string, std::vector<unsigned int>>              single,
                              std::map<std::string, std::vector<std::vector<unsigned int>>> multiple,
                              std::string                                                   filename) const
{
        boost::property_tree::ptree root;
        std::set<std::string>       tags;

        for (const auto& tag : single)
                tags.insert(tag.first);
        for (const auto& tag : multiple)
                tags.insert(tag.first);

        for (const auto& tag : tags) {
                boost::property_tree::ptree tags_root;
                unsigned int                size;
                if (multiple.count(tag) && !multiple[tag].empty())
                        size = multiple[tag][0].size();
                else if (single.count(tag) && !single[tag].empty())
                        size = single[tag].size();
                else
                        continue;
                for (unsigned int step = 0; step < size; step++) {
                        boost::property_tree::ptree step_root;
                        if (multiple.count(tag)) {
                                std::vector<unsigned int> data;
                                for (const auto& vect : multiple[tag]) {
                                        if (vect.size() - 1 < step)
                                                continue;
                                        data.push_back(vect[step]);
                                }
                                auto p = FindMeanStdev(data);
                                step_root.put("mean", p.first);
                                step_root.put("standard deviation", p.second);
                        }
                        if (single.count(tag)) {
                                step_root.put("exact", single[tag][step]);
                        }
                        tags_root.add_child(std::to_string(step), step_root);
                }
                root.add_child(tag, tags_root);
        }
        std::ofstream file;
        file.open(filename);
        if (!file.good()) {
                logger->error("There was a problem creating the file '{}'", filename);
                return;
        }
        boost::property_tree::write_json(file, root);
}

void Calibrator::WriteResults(std::map<std::string, std::vector<std::vector<unsigned int>>> results,
                              std::string                                                   filename) const
{
        WriteResults(std::map<std::string, std::vector<unsigned int>>(), results, filename);
}

void Calibrator::WriteResults(std::map<std::string, std::vector<unsigned int>> results, std::string filename) const
{
        WriteResults(results, std::map<std::string, std::vector<std::vector<unsigned int>>>(), filename);
}

} // namespace calibration
