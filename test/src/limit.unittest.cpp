#include <catch2/catch_all.hpp>
#include "optimizer/optimizer.h"

#include <ranges>

TEST_CASE("Optimization Limit - Simple 1") {
    const std::string input = R"(Nodes:
  A:
    B: C
    C: D
  B:
    C: D
  C:
    B: C
    C: D
)";

    const std::string expectedOutput = R"(Nodes:
  A: &anchor_0
    B: C
    C: D
  B:
    C: D
  C: *anchor_0
)";

    YamlOptimizer optimizer(input, OptimizationSettings{
        .optimization_limit = 1,
    });
    optimizer.optimize();
    const std::string output = optimizer.str();

    REQUIRE(output == expectedOutput);
}

TEST_CASE("Optimization Limit - Simple 2..5") {
    const std::string input = R"(Nodes:
  A:
    B: C
    C: D
  B:
    C: D
  C:
    B: C
    C: D
)";

    std::string const& expectedOutput = input;

    for (int i : std::views::iota(2, 6))
    {
        YamlOptimizer optimizer(input, OptimizationSettings{
            .optimization_limit = i,
        });
        optimizer.optimize();
        std::string output = optimizer.str();

        REQUIRE(output == expectedOutput);
    }
}

TEST_CASE("Optimization Limit - Branchy 1..5")
{
    std::string input = R"(Nodes:
  A:
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  B:
    C: D
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
  C:
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  D:
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
    B: C
  E:
    F: G
    H: I
    I: J
)";

    const std::string expectedOutput = R"(Nodes:
  A: &anchor_0
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  B:
    C: D
    E: &anchor_1
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
  C: *anchor_0
  D:
    E: *anchor_1
    B: C
  E:
    F: G
    H: I
    I: J
)";

    for (int i : std::views::iota(1, 6))
    {
        YamlOptimizer optimizer(input, OptimizationSettings{
            .optimization_limit = i,
        });
        optimizer.optimize();
        std::string output = optimizer.str();

        REQUIRE(output == expectedOutput);
    }
}

TEST_CASE("Optimization Limit - Branchy 6")
{
    std::string input = R"(Nodes:
  A:
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  B:
    C: D
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
  C:
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  D:
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
    B: C
  E:
    F: G
    H: I
    I: J
)";

    const std::string expectedOutput = R"(Nodes:
  A: &anchor_0
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  B:
    C: D
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
  C: *anchor_0
  D:
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
    B: C
  E:
    F: G
    H: I
    I: J
)";

    YamlOptimizer optimizer(input, OptimizationSettings{
        .optimization_limit = 6,
    });
    optimizer.optimize();
    const std::string output = optimizer.str();

    REQUIRE(output == expectedOutput);
}

TEST_CASE("Optimization Limit - Branchy 7..10")
{
    std::string input = R"(Nodes:
  A:
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  B:
    C: D
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
  C:
    B: C
    D:
      - E: F
        G: H
      - E: F
        G: I
      - E: F
        G: J
  D:
    E:
      - F: G
        H: I
      - F: G
        H: J
      - F: G
        H: K
    B: C
  E:
    F: G
    H: I
    I: J
)";

    const std::string& expectedOutput = input;

    for (int i : std::views::iota(7, 11))
    {
        YamlOptimizer optimizer(input, OptimizationSettings{
            .optimization_limit = i,
        });
        optimizer.optimize();
        std::string output = optimizer.str();

        REQUIRE(output == expectedOutput);
    }
}
