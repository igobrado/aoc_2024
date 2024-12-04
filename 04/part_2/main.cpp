#include <algorithm>
#include <cinttypes>
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

    std::string mas       = "MAS";
    uint32_t    mas_count = 0;

    auto is_mas_or_reverse = [&mas](const std::string_view& word) {
        return std::ranges::equal(word, mas) || std::ranges::equal(word | std::views::reverse, mas);
    };

    for (int row = 1; row < file.size() - 1; ++row)
    {
        for (int column = 1; column < file[row].size() - 1; ++column)
        {
            //  M . S
            //  . A .
            //  M . S

            // Logic:
            // Start from row 1 so that you can go take top left and top right from the diagonal.
            //
            //                                                                M            A          S
            // 1. Construct top to right diagonal word from 3 letters - [i - 1][j - 1], [i][j], [i + 1][ j + i]
            //                                                                S           A           M
            // 2. Construct top to left diagonal word from 3 letters - [i - 1][j + 1], [i][j], [i + 1][j - 1]
            //
            // Compare with MAS and SAM

            std::string word{file[row - 1][column - 1], file[row][column], file[row + 1][column + 1]};
            std::string word_two{file[row - 1][column + 1], file[row][column], file[row + 1][column - 1]};

            if (is_mas_or_reverse(word) && is_mas_or_reverse(word_two))
            {
                ++mas_count;
            }
        }
    }

    std::puts(std::format("mas_count: {}", mas_count).c_str());
}
