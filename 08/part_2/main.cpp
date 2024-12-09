#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ranges>
#include <set>
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

struct point
{
    int m_x, m_y;

    auto operator<=>(const point&) const = default;

    point operator+(point const& rhs) const noexcept
    {
        return {m_x + rhs.m_x, m_y + rhs.m_y};
    }

    point operator-(point const& rhs) const noexcept
    {
        return {m_x - rhs.m_x, m_y - rhs.m_y};
    }
};

struct grid
{
    std::map<char, std::vector<point>> m_frequency_position_mapping;

    int m_width  = 0;
    int m_height = 0;

    bool in_grid(const point& rhs) const
    {
        return (rhs.m_x >= 0)          //
               && (rhs.m_x < m_width)  //
               && (rhs.m_y >= 0)       //
               && (rhs.m_y < m_height);
    }

    void load(std::ifstream& a_stream)
    {
        using file_iterator = std::istream_iterator<std::string>;
        custom_range<file_iterator> file_range{file_iterator{a_stream}};

        std::ranges::for_each(file_range, [&](std::string const& a_line) {
            std::ranges::for_each(a_line, [&, i = 0](char c) mutable {
                if (c != '.')
                {
                    m_frequency_position_mapping[a_line[i]].push_back({i, m_height});
                }
                ++i;
            });

            m_width = std::max(m_width, static_cast<int>(a_line.size()));
            m_height++;
        });
    }
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

    grid map{};
    map.load(stream);

    std::set<point> antinodes;
    for (auto const& [frequency, positions] : map.m_frequency_position_mapping)
    {
        // Cartesian product has to be filtered with unique pairs
        auto filtered_carteseian_product = std::views::cartesian_product(positions, positions)
                                           | std::views::filter(                 //
                                               [](auto const& a_cartesian_pair)  //
                                               {                                 //
                                                   return a_cartesian_pair.first < a_cartesian_pair.second;
                                               });

        std::ranges::for_each(filtered_carteseian_product, [&](auto const& a_pair) {
            auto const& [left, right] = a_pair;
            point delta               = left - right;
            point new_pos             = left;

            while (map.in_grid(new_pos))
            {
                antinodes.insert(new_pos);
                new_pos = new_pos + delta;
            }

            new_pos = right;
            while (map.in_grid(new_pos))
            {
                antinodes.insert(new_pos);
                new_pos = new_pos - delta;
            }
        });
    }

    std::puts(std::format("Antinodes count: {}", antinodes.size()).c_str());
}
