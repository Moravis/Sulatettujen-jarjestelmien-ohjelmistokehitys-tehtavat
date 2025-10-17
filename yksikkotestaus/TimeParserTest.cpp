#include <gtest/gtest.h>
#include "../TimeParser.h"

TEST(TimeParserTest, TestCaseCorrectTime) {
    
    char time_test[] = "141205";
    int expected_seconds = 14 * 3600 + 12 * 60 + 5;
    ASSERT_EQ(time_parse(time_test), expected_seconds);

    char time_test2[] = "000105";
    ASSERT_EQ(time_parse(time_test2), 65);

}

TEST(TimeParserTest, TestCaseIncorrectTime){

    char time_test[] = "000077";
    ASSERT_EQ(time_parse(time_test), TIME_VALUE_ERROR);

}

TEST(TimeParserTest, TestStringLen){

    char time_test[] = "00500";
    ASSERT_EQ(time_parse(time_test), TIME_LEN_ERROR);

    char time_test2[] = "0005555";
    ASSERT_EQ(time_parse(time_test2), TIME_LEN_ERROR);

    ASSERT_EQ(time_parse(NULL), TIME_ARRAY_ERROR);

}

TEST(TimeParserTest, TestCaseNoTime){
    char time_test[] = "123ryg";
    ASSERT_EQ(time_parse(time_test),DIGIT_ERROR);
}

// https://google.github.io/googletest/reference/testing.html
// https://google.github.io/googletest/reference/assertions.html
