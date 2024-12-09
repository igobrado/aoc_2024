#include <algorithm>
#include <cinttypes>
#include <format>
#include <fstream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
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

    std::unordered_map<int, std::vector<int>> graph{};
    std::vector<std::vector<int>>             updates_data;

    std::ranges::for_each(file_range, [&graph, &updates_data](std::string const& a_str) {
        if (a_str.contains("|"))
        {
            std::stringstream ss(a_str);
            int               x, y;
            char              delimiter;

            ss >> x >> delimiter >> y;
            graph[x].push_back(y);
        }
        else if (a_str.contains(","))
        {
            std::vector<int> numbers{};
            std::ranges::transform(a_str | std::views::split(','), std::back_inserter(numbers), [](auto&& part) {
                return std::stoi(std::string{part.begin(), part.end()});
            });

            updates_data.emplace_back(std::move(numbers));
        }
    });

    if (updates_data.empty() || graph.empty())
    {
        std::puts("Invalid parsing!");
        return -1;
    }

    std::vector<int> valid_middle_pages{};

    auto is_order_valid = [](std::vector<int> const&                          a_update,
                             std::unordered_map<int, std::vector<int>> const& a_graph) {
        std::unordered_map<int, size_t> position;
        std::ranges::for_each(a_update | std::views::enumerate, [&position](auto const& pair) {
            auto [idx, value] = pair;
            position[value]   = idx;
        });

        return std::ranges::all_of(a_graph, [&](const auto& pair) {
            const auto& [x, neighbors] = pair;
            return std::ranges::all_of(neighbors, [&](int y) {
                return !position.contains(x) || !position.contains(y) || position[x] <= position[y];
            });
        });
    };

    for (auto const& update : updates_data)
    {
        if (is_order_valid(update, graph))
        {
            int mid_page = update[update.size() / 2];
            valid_middle_pages.push_back(mid_page);
        }
    }

    std::puts(
        std::format("Valid middle pages: {}", std::accumulate(valid_middle_pages.begin(), valid_middle_pages.end(), 0))
            .c_str());

    return 0;
}
