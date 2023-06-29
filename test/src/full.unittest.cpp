#include <catch2/catch_all.hpp>
#include "optimizer/optimizer.h"

TEST_CASE("Full substitution - Simple") {
    const std::string input = R"(- - Apple
  - Orange
  - Banana
- Fruits:
  - Apple
  - Orange
  - Banana
- name: John Doe
  age: 30
  profession: Developer
- Person:
    name: John Doe
    age: 30
    profession: Developer
- SomeList:
    - 1
    - 2
    - 3
    - 4
- SomeValue: true
)";

    const std::string expectedOutput = R"(- &anchor_0
  - Apple
  - Orange
  - Banana
- Fruits: *anchor_0
- &anchor_1
  name: John Doe
  age: 30
  profession: Developer
- Person: *anchor_1
- SomeList:
    - 1
    - 2
    - 3
    - 4
- SomeValue: true
)";

    YamlOptimizer optimizer(input);
    optimizer.optimize();
    const std::string output = optimizer.str();

    REQUIRE(output == expectedOutput);
}

TEST_CASE("Full substitution - Multi-level") {
    const std::string input = R"(- top_level:
    - nested:
        - - Apple
          - Orange
          - Banana
- another_level:
    - even_deeper:
        - further:
            - key1: value1
              key2: value2
- other_level:
    - some_key:
        - Apple
        - Orange
        - Banana
    - additional_key:
        key1: value1
        key2: value2
- SomeList:
    - 1
    - 2
    - 3
    - 4
- SomeValue: true
)";

    const std::string expectedOutput = R"(- top_level:
    - nested:
        - &anchor_0
          - Apple
          - Orange
          - Banana
- another_level:
    - even_deeper:
        - further:
            - &anchor_1
              key1: value1
              key2: value2
- other_level:
    - some_key: *anchor_0
    - additional_key: *anchor_1
- SomeList:
    - 1
    - 2
    - 3
    - 4
- SomeValue: true
)";

    YamlOptimizer optimizer(input);
    optimizer.optimize();
    const std::string output = optimizer.str();

    REQUIRE(output == expectedOutput);
}
