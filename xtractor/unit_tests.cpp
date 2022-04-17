#include "xtractor/lib.h"
#include <gtest/gtest.h>
#include <limits>

class IsDigitsTestP : public testing::TestWithParam<std::tuple<std::string, bool>>
{
};

TEST_P(IsDigitsTestP, GivenString_CorrectlyDeterminesIfAllCharactersAreDigits)
{
    // SETUP
    const auto [str, expected_result] = GetParam();

    // CALL & CHECK
    EXPECT_EQ(expected_result, is_digits(str));
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(IsDigitsTest,
                         IsDigitsTestP,
                         testing::Values(
                             std::make_tuple("", false),            // handling empty strings
                             std::make_tuple("abc", false), 
                             std::make_tuple("1abc", false), 
                             std::make_tuple("abc9", false), 
                             std::make_tuple("abc7def7ghi", false), 
                             std::make_tuple("123", true),
                             std::make_tuple("1", true),
                             std::make_tuple("999999999999", true)  // handling long inputs
                             ));
// clang-format on

class ParseLineTestP : public testing::TestWithParam<std::tuple<std::string, bool, int, int>>
{
};

TEST_P(ParseLineTestP, GivenString_ProvidesCorrectPairIfStringContainsOnlyDigits)
{
    // SETUP
    const auto [str, has_value, expected_first, expected_second] = GetParam();

    // CALL
    const auto optional_result = parse_line(str);

    // CHECK
    ASSERT_EQ(has_value, optional_result.has_value());

    if (optional_result.has_value())
    {
        const auto result = optional_result.value();

        EXPECT_EQ(expected_first, result.first);
        EXPECT_EQ(expected_second, result.second);
    }
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(ParseLineTest,
                         ParseLineTestP,
                         testing::Values(
                             std::make_tuple("", false, 0, 0),                      // handling empty strings
                             std::make_tuple("123456", false, 0, 0), 
                             std::make_tuple("333.444", false, 0, 0), 
                             std::make_tuple("333 444f", false, 0, 0), 
                             std::make_tuple("333 444\n55 66", false, 0, 0),        // two lines in one
                             std::make_tuple("123 456", true, 456, 123),
                             std::make_tuple("84573875 1", true, 1, 84573875),
                             std::make_tuple("12345678 99999999999", false, 0, 0)   // input too long
                             ));
// clang-format on

class ParseStreamTestHandlesBadInputP : public testing::TestWithParam<std::tuple<std::string, int>>
{
};

TEST_P(ParseStreamTestHandlesBadInputP, WhenSizeZeroRequested_ReturnsEmptyDatabase)
{
    // SETUP
    const auto [str, valid_item_count] = GetParam();
    std::istringstream stream(str);
    constexpr std::size_t max_size = 0U;

    // CALL
    const auto db = parse_stream(stream, max_size);

    // CHECK
    EXPECT_EQ(0U, db.size());
}

TEST_P(ParseStreamTestHandlesBadInputP, WhenSizeNRequested_ReturnsAllValidItems)
{
    // SETUP
    const auto [str, max_result_count] = GetParam();
    std::istringstream stream(str);
    constexpr std::size_t max_size = std::numeric_limits<int>::max(); // Basically no upper limit

    // CALL
    const auto db = parse_stream(stream, max_size);

    // CHECK
    EXPECT_EQ(max_result_count, db.size());
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(ParseStreamTestHandlesBadInput,
                         ParseStreamTestHandlesBadInputP,
                         testing::Values(
                             std::make_tuple("", 0),
                             std::make_tuple("123456", 0), 
                             std::make_tuple("123 456", 1), 
                             std::make_tuple("123 456\n567 890", 2),
                             std::make_tuple("123 456\n567 890\nabc\n144 441\n33 44", 4)
                             ));
// clang-format on

class ParseStreamTestFindsMaxItemP : public testing::TestWithParam<std::tuple<std::string, int>>
{
};

TEST_P(ParseStreamTestFindsMaxItemP, WhenSizeOneRequested_ReturnsMaxItem)
{
    // SETUP
    const auto [str, expected_result] = GetParam();
    std::istringstream stream(str);

    // CALL
    const auto db = parse_stream(stream, 1U);

    // CHECK
    ASSERT_EQ(1U, db.size());

    const auto first_item = *db.begin();

    EXPECT_EQ(expected_result, first_item.second);
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(ParseStreamTestFindsMaxItem,
                         ParseStreamTestFindsMaxItemP,
                         testing::Values(
                             std::make_tuple("1 400", 1), 
                             std::make_tuple("1 400\n2 300", 1),
                             std::make_tuple("1 400\n2 500", 2),
                             std::make_tuple("1 400\n2 500\n3 600", 3),
                             std::make_tuple("1 400\n2 500\n3 200", 2)
                             ));
// clang-format on

class ParseStreamTestReturnsCorrectCollectionP : public testing::TestWithParam<std::tuple<std::string, std::vector<int>>>
{
};

TEST_P(ParseStreamTestReturnsCorrectCollectionP, GivenStringAndSizeThree_ReturnUniqueIdsOfLargestValues)
{
    // SETUP
    const auto [str, expected_results] = GetParam();
    const int max_size{3};
    std::istringstream stream(str);

    // CALL
    const auto db = parse_stream(stream, max_size);

    // CHECK
    ASSERT_EQ(max_size, db.size());

    const bool containers_contain_same_elements = std::equal(db.begin(), db.end(), expected_results.begin(), [](auto actual_pair, int expected)
                                                             { return actual_pair.second == expected; });

    EXPECT_TRUE(containers_contain_same_elements);
}

const std::string sample_data_input{R"(1650171011 8
1650171028 367
1650171037 21
1650171056 244
1650171058 105
1650171066 188)"};
const std::vector<int> sample_data_expected_result{1650171066, 1650171056, 1650171028};

const std::string multiple_instances_input{R"(1 200
2 200
3 115
4 110)"};
const std::vector<int> multiple_instances_expected_result{3, 1, 2};

// clang-format off
INSTANTIATE_TEST_SUITE_P(ParseStreamTestReturnsCorrectCollection,
                         ParseStreamTestReturnsCorrectCollectionP,
                         testing::Values(
                             std::make_tuple(sample_data_input, sample_data_expected_result),
                             std::make_tuple(multiple_instances_input, multiple_instances_expected_result)
                             ));
// clang-format on
