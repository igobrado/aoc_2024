#include <algorithm>
#include <format>
#include <fstream>
#include <iterator>
#include <numeric>
#include <ranges>
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
    std::vector<std::vector<int>> reports;

    std::string line;

    while (std::getline(str, line))
    {
        std::istringstream line_stream(line);
        reports.push_back({std::istream_iterator<int>{line_stream}, std::istream_iterator<int>{}});
    }

    auto generate_differences = [](std::vector<int> const& a_report) {
        std::vector<int> differences;
        for (size_t i = 0; i < a_report.size() - 1; ++i)
        {
            differences.push_back(a_report[i + 1] - a_report[i]);
        }

        return differences;
    };

    auto is_report_valid = [&](std::vector<int> const& a_report) {
        auto const differences    = generate_differences(a_report);
        auto const is_report_safe = std::ranges::all_of(differences, [](int diff) {
            return std::abs(diff) >= 1 and std::abs(diff) <= 3;
        });

        if (!is_report_safe)
        {
            return false;
        }

        bool const is_ascending = std::ranges::all_of(differences, [](int diff) { return diff > 0; });
        bool const is_decending = std::ranges::all_of(differences, [](int diff) { return diff < 0; });

        return is_ascending || is_decending;
    };

    auto check_reports_with_reduction = [&](std::vector<int> const& a_report) {
        if (is_report_valid(a_report))
            return true;

        for (auto i : std::ranges::views::iota(0ul, a_report.size()))
        {
            std::vector<int> modified_report = a_report;
            modified_report.erase(modified_report.begin() + i);

            if (is_report_valid(modified_report))
            {
                return true;
            }
        }

        return false;
    };

    std::puts(std::format("safe_count: {}", std::ranges::count_if(reports, check_reports_with_reduction)).c_str());

    return 0;
}
