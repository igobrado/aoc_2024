#include <algorithm>
#include <cinttypes>
#include <format>
#include <fstream>
#include <iterator>
#include <numeric>
#include <queue>
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

std::vector<int> topological_sort(std::unordered_map<int, std::vector<int>> const& a_graph)
{
    std::unordered_map<int, int> in_degree;
    std::ranges::for_each(a_graph, [&in_degree](auto const& pair) {
        in_degree[pair.first];

        std::ranges::for_each(pair.second, [&in_degree](auto const neighbor) { ++in_degree[neighbor]; });
    });

    std::ranges::for_each(a_graph, [&in_degree](auto const& pair) {
        if (!in_degree.contains(pair.first))
        {
            in_degree[pair.first] = 0;
        }
    });

    std::queue<int> zero_in_degree;
    std::ranges::for_each(in_degree, [&zero_in_degree](auto const& pair) {
        if (pair.second == 0)
        {
            zero_in_degree.push(pair.first);
        }
    });

    std::vector<int> sorted_order;
    while (!zero_in_degree.empty())
    {
        sorted_order.push_back(zero_in_degree.front());
        zero_in_degree.pop();
    }

    return sorted_order;
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

    uint32_t sum = 0;
    for (const auto& update : updates_data)
    {
        if (!is_order_valid(update, graph))
        {
            // Create a subgraph restricted to the current update
            std::unordered_map<int, std::vector<int>> subgraph;
            for (int node : update)
            {
                if (graph.contains(node))
                {
                    for (int neighbor : graph.at(node))
                    {
                        if (std::ranges::find(update, neighbor) != update.end())
                        {
                            subgraph[node].push_back(neighbor);
                        }
                    }
                }
            }

            // Get the sorted order and calculate the middle element
            auto sorted = topological_sort(subgraph);
            auto it     = std::ranges::find_first_of(sorted, update);
            if (it != sorted.end())
            {
                sum += *it;
            }
        }
    }

    std::puts(std::format("Sorted = {}", sum).c_str());

    return 0;
}
