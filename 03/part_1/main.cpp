#include <algorithm>
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

    std::regex  multiply(R"(mul\((\d{1,3}),(\d{1,3})\))");
    std::smatch match;

    uint32_t                    total_sum = 0;
    custom_range<file_iterator> file_range{file_iterator{stream}};
    std::ranges::for_each(file_range, [&](std::string const& a_line) {
        // Iterate through all matches in the memory string
        auto iterator(a_line.cbegin());
        while (std::regex_search(iterator, a_line.cend(), match, multiply))
        {
            uint32_t lhs = std::stoi(match[1].str());
            uint32_t rhs = std::stoi(match[2].str());

            total_sum += (lhs * rhs);
            iterator = match.suffix().first;
        }
    });

    std::puts(std::format("Valid multiplications{}", total_sum).c_str());

    return 0;
}
