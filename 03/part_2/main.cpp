#include <algorithm>
#include <concepts>
#include <format>
#include <fstream>
#include <iterator>
#include <ranges>
#include <regex>
#include <string>

template<typename IteratorType>
struct custom_range
{
    custom_range(IteratorType begin, IteratorType end = {})
    : m_begin{begin}
    , m_end{end}
    {
    }

    auto begin() -> decltype(auto)
    {
        return m_begin;
    }
    auto end() -> decltype(auto)
    {
        return m_end;
    }

    IteratorType m_begin;
    IteratorType m_end;
};

int main(int argc, char** argv)
{
    if (argc < 1)
    {
        std::puts("Input files needed");
        return -1;
    }
    std::puts(std::format("Using file on patk: {}", argv[1]).c_str());

    using file_iterator = std::istream_iterator<std::string>;
    std::ifstream stream{argv[1]};

    std::regex multiply(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::regex enable_multiply(R"(do\(\))");
    std::regex disable_mutilply(R"(don't\(\))");

    std::regex combined_regex(R"(mul\((\d{1,3}),(\d{1,3})\)|do\(\)|don't\(\))");

    uint32_t sum     = 0;
    bool     enabled = true;

    custom_range<file_iterator> file_range{file_iterator{stream}};
    std::ranges::for_each(file_range, [&](std::string const& a_line) {
        custom_range<std::sregex_iterator> range{{a_line.begin(), a_line.end(), combined_regex}};

        std::ranges::for_each(range, [&](std::smatch const& a_match) {
            // First check whether match is a 'do' regex
            // Then Check if it's a 'don't'
            // Then multiply

            if (std::regex_match(a_match.str(), enable_multiply))
            {
                enabled = true;
            }
            else if (std::regex_match(a_match.str(), disable_mutilply))
            {
                enabled = false;
            }
            else if (enabled && std::regex_match(a_match.str(), multiply))
            {
                uint32_t lhs = stoi(a_match[1].str());
                uint32_t rhs = stoi(a_match[2].str());
                sum += (lhs * rhs);
            }
        });
    });

    std::puts(std::format("Sum: {}", sum).c_str());
}
