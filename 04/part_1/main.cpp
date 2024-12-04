#include <algorithm>
#include <cstdint>
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

    uint32_t xmas_count                  = 0;
    auto     check_and_update_xmas_count = [&xmas_count](std::string const& a_word) {
        constexpr char const* xmas_word = "XMAS";
        if ((a_word == xmas_word) || (std::string(a_word.rbegin(), a_word.rend()) == xmas_word))
        {
            ++xmas_count;
        }
    };

    for (auto const& row : std::ranges::views::iota(0ul, file.size()))
    {
        for (auto const& column : std::ranges::views::iota(0ul, file[row].size()))
        {
            if (column < file[row].size() - 3)
            {
                check_and_update_xmas_count(file[row].substr(column, 4));
            }

            if (row < file.size() - 3)
            {
                std::string word{
                    file[row][column],
                    file[row + 1][column],
                    file[row + 2][column],
                    file[row + 3][column]};
                check_and_update_xmas_count(word);
            }

            if (row < file.size() - 3 && column < file[row].size() - 3)
            {
                std::string word{
                    file[row][column],
                    file[row + 1][column + 1],
                    file[row + 2][column + 2],
                    file[row + 3][column + 3]};
                check_and_update_xmas_count(word);
            }

            if (row < file.size() - 3 && column >= 3)
            {
                std::string word{
                    file[row][column],
                    file[row + 1][column - 1],
                    file[row + 2][column - 2],
                    file[row + 3][column - 3]};

                check_and_update_xmas_count(word);
            }
        }
    }

    std::puts(std::format("xmas_count: {}", xmas_count).c_str());
}
