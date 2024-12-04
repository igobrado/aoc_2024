#include <algorithm>
#include <format>
#include <fstream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

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
    std::puts(std::format("Using file on path: {}", argv[1]).c_str());

    using file_iterator = std::istream_iterator<std::string>;
    std::ifstream stream{argv[1]};

    custom_range<file_iterator> file_range{file_iterator{stream}};
    std::vector<std::string>    file{};
    std::ranges::copy(file_range, std::back_inserter(file));

    std::puts(std::format("ToDo: {}", 0).c_str());
}
