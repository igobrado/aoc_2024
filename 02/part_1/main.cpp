#include <algorithm>
#include <cmath>
#include <format>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char** argv)
{
    if (argc < 1)
    {
        std::puts("Input files needed");
        return -1;
    }

    std::puts(std::format("Using file on patk: {}", argv[1]).c_str());

    std::ifstream                 str{argv[1]};
    std::vector<std::vector<int>> rows;

    std::string line;

    while (std::getline(str, line))
    {
        std::istringstream line_stream(line);
        rows.push_back({std::istream_iterator<int>{line_stream}, std::istream_iterator<int>{}});
    }

    auto generate_differences = [](std::vector<int> const& a_row) {
        std::vector<int> differences;
        for (size_t i = 0; i < a_row.size() - 1; ++i)
        {
            differences.push_back(a_row[i + 1] - a_row[i]);
        }

        return differences;
    };

    auto is_report_safe = [&](std::vector<int> const& a_row) {
        auto const differences = generate_differences(a_row);
        auto const is_row_safe = std::ranges::all_of(differences, [](int diff) {
            return std::abs(diff) >= 1 and std::abs(diff) <= 3;
        });

        if (!is_row_safe)
        {
            return false;
        }

        bool const is_ascending = std::ranges::all_of(differences, [](int diff) { return diff > 0; });
        bool const is_decending = std::ranges::all_of(differences, [](int diff) { return diff < 0; });

        return is_ascending || is_decending;
    };

    int safe_count = std::ranges::count_if(rows, is_report_safe);

    std::puts(std::format("safe_count: {}", safe_count).c_str());
    return 0;
}
