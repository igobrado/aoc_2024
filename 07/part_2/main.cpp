#include <algorithm>
#include <cassert>
#include <expected>
#include <format>
#include <fstream>
#include <ranges>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 1)
    {
        std::puts("Input files needed");
        return -1;
    }
    std::puts(std::format("Using file on path: {}", argv[1]).c_str());
    std::ifstream stream{argv[1]};

    long        result{0};
    std::string line{};

    while (std::getline(stream, line))
    {
        auto tokens = line | std::views::split(' ')
                      | std::views::transform([](auto&& word) { return std::string(word.begin(), word.end()); });

        auto token_it = tokens.begin();
        long target   = std::stol(*token_it);

        std::vector<long> combinations;
        if (++token_it != tokens.end())
        {
            combinations.push_back(std::stol(*token_it));  // Add first value
        }

        for (long val : tokens                            //
                            | std::views::drop(2)         //
                            | std::views::transform(      //
                                [](const std::string& s)  //
                                {                         //
                                    return std::stol(s);
                                }))
        {
            std::vector<long> tmp;

            for (long element : combinations)
            {
                tmp.push_back(element + val);
                tmp.push_back(element * val);
                tmp.push_back(std::stol(std::to_string(element) + std::to_string(val)));
            }

            combinations = std::move(tmp);
        }

        if (std::ranges::any_of(combinations, [target](long value) { return value == target; }))
        {
            result += target;
        }
    }

    std::puts(std::format("Result: {}", result).c_str());
    return 0;
}
