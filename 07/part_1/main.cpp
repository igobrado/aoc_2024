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
        auto parsed_line = line | std::views::split(' ')
                           | std::views::transform([](auto&& word) { return std::string(word.begin(), word.end()); });

        auto token_it = parsed_line.begin();
        long target   = std::stol(*token_it);

        std::vector<long> combinations;
        if (++token_it != parsed_line.end())
        {
            combinations.push_back(std::stol(*token_it));
        }

        for (long value : parsed_line                                           //
                              | std::views::drop(2)                             //
                              | std::views::transform([](std::string const& s)  //
                                                      {                         //
                                                          return std::stol(s);
                                                      }))
        {
            // Generate new combinations
            auto additions       = combinations | std::views::transform([value](long el) { return el + value; });
            auto multiplications = combinations | std::views::transform([value](long el) { return el * value; });

            std::vector<long> tmp;
            tmp.reserve(additions.size() + multiplications.size());

            std::ranges::copy(additions, std::back_inserter(tmp));
            std::ranges::copy(multiplications, std::back_inserter(tmp));

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
