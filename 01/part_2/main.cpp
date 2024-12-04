#include <algorithm>
#include <cinttypes>
#include <format>
#include <fstream>
#include <istream>
#include <iterator>
#include <ranges>
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

struct row
{
    int left, right;
};

std::istream& operator>>(std::istream& stream, row& row)
{
    stream >> row.left >> row.right;
    return stream;
}

int main(int argc, char** argv)
{
    if (argc < 1)
    {
        std::puts("Input files needed!");
        return -1;
    }

    std::ifstream str{argv[1]};
    using file_iterator = std::istream_iterator<row>;

    custom_range<file_iterator> range{file_iterator{str}};
    std::vector<int>            left, right{};

    std::ranges::for_each(range, [&left, &right](row const& a_row) {
        left.push_back(a_row.left);
        right.push_back(a_row.right);
    });

    uint32_t similarity_score{0};
    std::ranges::for_each(left, [&](auto value) { similarity_score += value * std::ranges::count(right, value); });

    std::puts(std::format("Similarity score: {}", similarity_score).c_str());
    return 0;
}
