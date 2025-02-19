#define BOOST_TEST_MODULE Lab1Tests
#include <boost/test/included/unit_test.hpp>
#include "Utils.h"

TokensSequence preprocessingTest(std::string&& regex)
{
    return utils::preprocessing::validateRegex(regex);
}

BOOST_AUTO_TEST_CASE(testPreprocessing)
{
    BOOST_CHECK(preprocessingTest("a*b") == (TokensSequence{"a", "*", "b", "."}));
    BOOST_CHECK(preprocessingTest("a+") == (TokensSequence{"a", "*", "."}));
    BOOST_CHECK(preprocessingTest("a*") == (TokensSequence{"a", "*"}));
    BOOST_CHECK(preprocessingTest("a|b") == (TokensSequence{"a", "b", "|"}));
    BOOST_CHECK(preprocessingTest("a(a|b)") == (TokensSequence{"a", "a", "b", "|", "."}));
    BOOST_CHECK(preprocessingTest("(a|b)*abb") == (TokensSequence{"a", "b", "|", "*", "a", ".", "b", ".", "b", "."}));
}