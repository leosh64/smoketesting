#ifndef XTRACTOR_LIB_H
#define XTRACTOR_LIB_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>

/// @brief: Pair of <numeric_value, unique_record_identifier> (order inversed w.r.t. input file)
using Item = std::pair<int, int>;
/// @brief: Data structure to hold intermediate candidates and final results
using Database = std::multimap<int, int>;

/// @brief: Returns true if a non-empty string contains only digit characters
inline bool is_digits(std::string const &str)
{
    return (str.size() > 0U) && (std::all_of(str.begin(), str.end(), ::isdigit));
}

/// @brief: Parses line of format "<unique record identifier><white_space><numeric value>" into Item pair
std::optional<Item> parse_line(std::string const &line)
{
    std::optional<Item> result;

    std::size_t const separator = line.find_first_of(' ');

    if (separator != std::string::npos)
    {
        std::string const first = line.substr(0, separator);
        std::string const second = line.substr(separator + 1);

        // Constraint: both unique_identifier and numeric_value must be positive integers!
        if (is_digits(first) && is_digits(second))
        {
            try
            {
                int const unique_identifier = std::stoi(first);
                int const numeric_value = std::stoi(second);

                result = {numeric_value, unique_identifier};
            }
            catch (std::out_of_range &e)
            {
                // Do nothing, optional will remain empty
            }
        }
    }

    return result;
}

/// @brief: Reads stream with multiple lines and extracts up to `max_size` elements with highest numerical_value
Database parse_stream(std::basic_istream<char> &stream, std::size_t max_size)
{
    Database db;

    if (max_size > 0U)
    {
        std::string line;
        while (getline(stream, line))
        {
            auto const optional_parsed_pair = parse_line(line);

            if (!optional_parsed_pair.has_value())
            {
                std::cerr << "Warning: Cannot parse line, skipping: " << line << std::endl;
                continue;
            }

            auto const parsed_pair = optional_parsed_pair.value();

            if (db.size() < max_size || db.begin()->first < parsed_pair.first)
            {
                // Logarithmic time complexity: O(log(max_size))
                db.insert(parsed_pair);
            }

            // Constant space complexity: O(max_size)
            if (db.size() > max_size)
            {
                // Logarithmic time complexity: O(log(max_size))
                db.erase(db.begin());
            }
        }
    }

    return db;
}

#endif // XTRACTOR_LIB_H
