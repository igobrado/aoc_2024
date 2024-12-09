#include <cassert>
#include <expected>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <ranges>
#include <set>
#include <string>
#include <unordered_set>
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

enum class direction
{
    kNorth = 0,
    kEast,
    kSouth,
    kWest
};

constexpr std::pair<int, int> get_next_position(direction const a_direction, int const a_xpos, int const a_ypos)
{
    switch (a_direction)
    {
        case direction::kNorth:
            return {a_xpos, a_ypos - 1};
        case direction::kEast:
            return {a_xpos + 1, a_ypos};
        case direction::kSouth:
            return {a_xpos, a_ypos + 1};
        case direction::kWest:
            return {a_xpos - 1, a_ypos};
    }
    return {a_xpos, a_ypos};  // Fallback, though this case is unreachable
}

struct position
{
    int       m_x, m_y;
    direction m_direction;

    constexpr bool operator==(position const& a_rhs) const noexcept
    {
        return m_x == a_rhs.m_x && m_y == a_rhs.m_y && m_direction == a_rhs.m_direction;
    }
};

namespace std {
template<>
struct hash<position>
{
    constexpr size_t operator()(position const& a_position) const noexcept
    {
        return hash<int>()(a_position.m_x) ^ hash<int>()(a_position.m_y)
               ^ hash<int>()(static_cast<int>(a_position.m_direction));
    }
};
}  // namespace std

struct parsed_input
{
    std::set<std::pair<int, int>> m_obstructions;
    position                      m_guard_position;
    struct dimension
    {
        int m_width, m_height;
    } m_dimension;
};

constexpr parsed_input parse_lines(std::vector<std::string> const& a_lines)
{
    std::set<std::pair<int, int>> obstructions;
    position                      guard{0, 0, direction::kNorth};
    int                           maxX = static_cast<int>(a_lines[0].size()), maxY = static_cast<int>(a_lines.size());

    for (auto const y : std::views::iota(0, maxY))
    {
        for (auto const x : std::views::iota(0, maxX))
        {
            switch (a_lines[y][x])
            {
                case '#':
                    obstructions.emplace(x, y);
                    break;
                case '^':
                    guard = {x, y, direction::kNorth};
                    break;
            }
        }
    }

    return {obstructions, guard, {maxX, maxY}};
}

constexpr std::expected<std::set<std::pair<int, int>>, bool> get_visited(
    std::set<std::pair<int, int>>& a_obstructions,
    position                       a_guard,
    parsed_input::dimension const& a_dimension)
{
    auto [current_x, current_y, where_to_go] = a_guard;
    std::unordered_set<position>  visited;
    std::set<std::pair<int, int>> path;

    constexpr auto change_direction = [](direction& a_dir) {
        a_dir = static_cast<direction>((static_cast<int>(a_dir) + 1) % 4);
    };

    constexpr auto out_of_bounds = [](int const a_nx, int const a_ny, parsed_input::dimension const& a_dimension) {
        return a_nx < 0 || a_nx >= a_dimension.m_width || a_ny < 0 || a_ny >= a_dimension.m_height;
    };

    visited.insert({current_x, current_y, where_to_go});
    path.insert({current_x, current_y});

    while (true)
    {
        auto [new_x, new_y] = get_next_position(where_to_go, current_x, current_y);
        if (out_of_bounds(new_x, new_y, a_dimension))
        {
            return path;  // Terminate if out of bounds
        }
        else if (a_obstructions.contains({new_x, new_y}))
        {
            change_direction(where_to_go);  // Rotate if obstruction found
        }
        else
        {
            if (visited.contains({new_x, new_y, where_to_go}))
            {
                return std::unexpected(false);  // Loop detected
            }

            current_x = new_x;
            current_y = new_y;
        }

        visited.insert({current_x, current_y, where_to_go});
        path.insert({current_x, current_y});
    }
}

int main(int argc, char** argv)
{
    if (argc < 1)
    {
        std::puts("Input files needed");
        return -1;
    }
    std::puts(std::format("Using file on path: {}", argv[1]).c_str());

    using file_iterator = std::istream_iterator<std::string>;
    std::ifstream               stream{argv[1]};
    custom_range<file_iterator> file_range{file_iterator{stream}};

    std::vector<std::string> lines;
    std::ranges::copy(file_range, std::back_inserter(lines));

    auto [obstructions, guard, dim] = parse_lines(lines);
    auto visited                    = get_visited(obstructions, guard, dim);
    std::cout << (visited ? visited->size() : 0) << std::endl;
    return 0;
}
