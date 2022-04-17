#!/usr/bin/env bash

# Load assert library providing commands such as "assert_eq" and "assert_contain"
source external/assert_sh/assert.sh

# Path to the output binary to be smoke-tested
EXECUTABLE="./xtractor/main"
TESTDATADIR="xtractor/testdata"

# Define tests
Test_NoArgumentsProvided_ExpectUsageHint() {
    # Need to capture stderr because that's where errors go
    stderr_output=$($EXECUTABLE 2>&1 >/dev/null)
    return_code="$?"

    expected_output="Usage: "
    expected_return_code=1

    assert_contain "$stderr_output" "$expected_output" "${FUNCNAME[0]}" || exit 1
    assert_eq "$return_code" "$expected_return_code" "${FUNCNAME[0]}" || exit 1
}

Test_OneArgumentAndStdinProvided_ExpectCorrectResults() {
    output=$(cat $TESTDATADIR/sample_data.txt | $EXECUTABLE 3)
    return_code="$?"

    expected_output=$'1650171056\n1650171028\n1650171090'
    expected_return_code=0

    assert_contain "$output" "$expected_output" "${FUNCNAME[0]}" || exit 1
    assert_eq "$return_code" "$expected_return_code" "${FUNCNAME[0]}" || exit 1
}

Test_TwoArgumentsAndExistingFileProvided_ExpectCorrectResults() {
    output=$($EXECUTABLE 3 $TESTDATADIR/sample_data.txt)
    return_code="$?"

    expected_output=$'1650171056\n1650171028\n1650171090'
    expected_return_code=0

    assert_contain "$output" "$expected_output" "${FUNCNAME[0]}" || exit 1
    assert_eq "$return_code" "$expected_return_code" "${FUNCNAME[0]}" || exit 1
}

Test_TwoArgumentsAndMissingFileProvided_ExpectErrorMessage() {
    # Need to capture stderr because that's where errors go
    output=$($EXECUTABLE 3 $TESTDATADIR/this_file_does_not_exist.txt 2>&1 >/dev/null)
    return_code="$?"

    expected_output="Error"
    expected_return_code=1

    assert_contain "$output" "$expected_output" "${FUNCNAME[0]}" || exit 1
    assert_eq "$return_code" "$expected_return_code" "${FUNCNAME[0]}" || exit 1
}

Test_TwoArgumentsAndIncorrectNumberProvided_ExpectErrorMessage() {
    # Need to capture stderr because that's where errors go
    output=$($EXECUTABLE -3 $TESTDATADIR/sample_data.txt 2>&1 >/dev/null)
    return_code="$?"

    expected_output="Error"
    expected_return_code=1

    assert_contain "$output" "$expected_output" "${FUNCNAME[0]}" || exit 1
    assert_eq "$return_code" "$expected_return_code" "${FUNCNAME[0]}" || exit 1
}

# Run all tests
Test_NoArgumentsProvided_ExpectUsageHint
Test_OneArgumentAndStdinProvided_ExpectCorrectResults
Test_TwoArgumentsAndExistingFileProvided_ExpectCorrectResults
Test_TwoArgumentsAndMissingFileProvided_ExpectErrorMessage
Test_TwoArgumentsAndIncorrectNumberProvided_ExpectErrorMessage
