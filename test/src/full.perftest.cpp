#include <celero/Celero.h>

#include <chrono>
#include <fstream>

#include "optimizer/optimizer.h"

namespace io_utils
{
std::string get_file_content(std::istream& is);
} // namespace io_utils

class YamlOptimizerFixture : public celero::TestFixture
{
public:
    YamlOptimizerFixture()
    {
        // Load the YAML configurations
        config128 = loadYamlFromFile("test/data/config128.yaml");
        config1024 = loadYamlFromFile("test/data/config1024.yaml");
    }

    // Helper function to load YAML from file
    std::string loadYamlFromFile(const std::string& filename)
    {
        std::ifstream is(filename);
        return io_utils::get_file_content(is);
    }

    // Set up any necessary resources before each benchmark iteration
    void setUp(const celero::TestFixture::ExperimentValue&) {}

    // Clean up any allocated resources after each benchmark iteration
    void tearDown()
    {
        // Clean up the YamlOptimizer instance
        optimizer.reset();
    }

    // Benchmark function for optimizing a 200-line YAML config
    void optimize200Lines()
    {
        // Reset the YamlOptimizer instance
        optimizer.reset(new YamlOptimizer(config128));
        optimizer->optimize();
    }

    // Benchmark function for optimizing a 1000-line YAML config
    void optimize1000Lines()
    {
        // Reset the YamlOptimizer instance
        optimizer.reset(new YamlOptimizer(config1024));
        optimizer->optimize();
    }

private:
    std::unique_ptr<YamlOptimizer> optimizer;

    std::string config128{};
    std::string config1024{};
};

// Define the performance test cases
BASELINE_F(YamlOptimizerPerformance, Optimize200Lines, YamlOptimizerFixture, 0,
           0)
{
    optimize200Lines();
}

BENCHMARK_F(YamlOptimizerPerformance, Optimize1000Lines, YamlOptimizerFixture,
            0, 0)
{
    optimize1000Lines();
}
