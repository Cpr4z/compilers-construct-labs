#define BOOST_TEST_MODULE Lab1Tests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "Utils.h"

#include "../AutomationFactory.hpp"
#include "../NFABuilder.hpp"
#include "../AutomationVizualizator.hpp"

namespace bdata = boost::unit_test::data;

TokensSequence preprocessingTest(std::string&& regex)
{
    return utils::preprocessing::validateRegex(std::move(regex));
}

struct NFATestFixture
{
    explicit NFATestFixture(std::string&& regex)
    {
        TokensSequence polishedSequence = utils::preprocessing::validateRegex(std::move(regex));
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(utils::AutomationType::e::NFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();
        if (const NFABuilderPtr & nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder))
        {
            nfaBuilder->Init(std::move(polishedSequence));
            m_nfa = nfaBuilder->Build();
        }
    }

    bool Imitate(std::string&& input)
    {
        return m_nfa->Imitate(std::move(input));
    }

private:
    IAutomationPtr m_nfa;
};

BOOST_AUTO_TEST_CASE(testPreprocessing)
{
    BOOST_CHECK(preprocessingTest("a*b") == (TokensSequence{"a", "*", "b", "."}));
    BOOST_CHECK(preprocessingTest("a+") == (TokensSequence{"a", "a", "*", "."}));
    BOOST_CHECK(preprocessingTest("a+b") == (TokensSequence{"a", "a", "*", ".", "b", "."}));
    BOOST_CHECK(preprocessingTest("a*") == (TokensSequence{"a", "*"}));
    BOOST_CHECK(preprocessingTest("a|b") == (TokensSequence{"a", "b", "|"}));
    BOOST_CHECK(preprocessingTest("a(a|b)") == (TokensSequence{"a", "a", "b", "|", "."}));
    BOOST_CHECK(preprocessingTest("(a|b)*abb") == (TokensSequence{"a", "b", "|", "*", "a", ".", "b", ".", "b", "."}));
}

BOOST_AUTO_TEST_SUITE(NFA_TEST_SUITE)

BOOST_DATA_TEST_CASE(NFA_TestCase1_Valid, bdata::make({"b", "ab", "aaaaaaab"}))
{
    NFATestFixture fixture("a*b");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase1_Invalid, bdata::make({"ba", "bbbbaaa", "a"}))
{
    NFATestFixture fixture("a*b");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase2_Valid, bdata::make({"a", "b"}))
{
    NFATestFixture fixture("a|b");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase2_Invalid, bdata::make({"ba", "bb", "bbbbaaa"}))
{
    NFATestFixture fixture("a|b");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase3_Valid, bdata::make({"a", "aa", "ba", "aaa", "baa"}))
{
    NFATestFixture fixture("(a|b).a*");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase3_Invalid, bdata::make({"", "caa", "bcb", "abb"})) // 1 test failed
{
    NFATestFixture fixture("(a|b).a*");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase4_Valid, bdata::make({"aabb", "babb", "aaabbbabb" "aabbabb", "babbabb"}))
{
    NFATestFixture fixture("(a|b)*abb");
//    NFATestFixture fixture("(a|b).*abb");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase4_Invalid, bdata::make({"", "a", "b", "bb", "aa", "aaab", "baab", "bbbbbb", "abbbbaaaaa"})) // all tests failed
{
    NFATestFixture fixture("(a|b)*abb");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase5_Valid, bdata::make({"a", "aa", "aaaaaaa"}))
{
    NFATestFixture fixture("a+");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase5_Invalid, bdata::make({"", "b", "aab", "ba", "bbbbb", "bbbbaaa"})) // 1 test failed
{
    NFATestFixture fixture("a+");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase6_Valid, bdata::make({"ab", "aab", "aaaaaaab"}))
{
    NFATestFixture fixture("a+b");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase6_Invalid, bdata::make({"b", "aa", "ba", "bbbbb", "bbbbaaa"})) // 2 tests failed
{
    NFATestFixture fixture("a+b");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase7_Valid, bdata::make({"abc", "aabc", "aabbc", "aaabbbc"}))
{
    NFATestFixture fixture("a+b+c");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase7_Invalid, bdata::make({"", "ab", "bc", "ac", "abbbbbcc"}))
{
    NFATestFixture fixture("a+b+c");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase8_Valid, bdata::make({"cagd", "cbgd", "ccccbbggd", "cbbbbbbd", "cccad"}))
{
    NFATestFixture fixture("c*(a|b*)g*d");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase8_Invalid, bdata::make({"aagd", "ccbh", "cbgbg"}))
{
    NFATestFixture fixture("c*(a|b*)g*d");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase9_Valid, bdata::make({"aaabh", "aaaggg", "aaaaaagggf", "aaaaaagggfffff", "aaabbbbbbh", "aaabbbbbcl", "aaabbbbccccch", "aaabbbbbbd", "aaabbbbbdddd"}))
{
    NFATestFixture fixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase9_Invalid, bdata::make({"g", "a", "aggg", "abbbb", "aaabbbbcc", "aaaaaaabbc", "aaabbclh", "aaadddd"}))
{
    NFATestFixture fixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase10_Valid, bdata::make({"mmnoprrrs", "mnoprs"}))
{
    NFATestFixture fixture("(m|n)*op+(q|r*)s");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase10_Invalid, bdata::make({"opr", "opxqs", "oopqq", "ooprrrs", "opqrs", "nnooprs"}))
{
    NFATestFixture fixture("(m|n)*op+(q|r*)s");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase11_Valid, bdata::make({"ad", "bccef", "bbbcdgg", "bccccccd", "aefg", "bbbbcceffffg", "bbcefggggg"}))
{
    NFATestFixture fixture("(a|b+).c*(d|ef+).g*");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase11_Invalid, bdata::make({"d", "c", "bccdd", "abg", "bcdh", "aaefg"}))
{
    NFATestFixture fixture("(a|b+).c*(d|ef+).g*");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase12_Valid, bdata::make({"vv", "vvvv", "aavv", "bbbbvv", "vvv", "ccvvv",}))
{
    NFATestFixture fixture("(aa|bb|cc)*vv+");
    BOOST_CHECK(fixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase12_Invalid, bdata::make({"ba", ""}))
{
    NFATestFixture fixture("(aa|bb|cc)*vv+");
    BOOST_CHECK(!fixture.Imitate(sample));
}

BOOST_AUTO_TEST_SUITE_END()
