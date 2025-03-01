#define BOOST_TEST_MODULE Lab1Tests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "Utils.h"

#include "../AutomationFactory.hpp"
#include "../NFABuilder.hpp"
#include "../DFABuilder.hpp"
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
        if (const NFABuilderPtr& nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder))
        {
            nfaBuilder->Init(std::move(polishedSequence));
            m_nfa = nfaBuilder->Build();
        }
    }

    bool Imitate(std::string&& input)
    {
        if (m_nfa)
        {
            return m_nfa->Imitate(std::move(input));
        }
        return false;
    }

    IAutomationPtr GetNFA() const { return m_nfa; }

private:
    IAutomationPtr m_nfa;
};

struct DFATestFixture
{
    explicit DFATestFixture(std::string&& regex)
    {
        NFATestFixture nfaTestFixture(std::move(regex));
        const IAutomationPtr nfaAutomat = nfaTestFixture.GetNFA();
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(utils::AutomationType::e::DFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();

        if (const DFABuilderPtr& dfaBuilder = std::dynamic_pointer_cast<DFABuilder>(builder))
        {
            if (const NFAPtr& nfa = std::dynamic_pointer_cast<NFA>(nfaAutomat))
            {
                dfaBuilder->Init(nfa);
                m_dfa = dfaBuilder->Build();
            }
        }
    }

    bool Imitate(std::string&& input)
    {
        if (m_dfa)
        {
            return m_dfa->Imitate(std::move(input));
        }
        return false;
    }

    IAutomationPtr m_dfa;

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

BOOST_AUTO_TEST_SUITE(NFA_AND_DFA_TEST_SUITE)

BOOST_DATA_TEST_CASE(NFA_TestCase1_Valid, bdata::make({"b", "ab", "aaaaaaab"}))
{
    NFATestFixture nfaFixture("a*b");
    DFATestFixture dfaFixture("a*b");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase1_Invalid, bdata::make({"ba", "bbbbaaa", "a"}))
{
    NFATestFixture nfaFixture("a*b");
    DFATestFixture dfaFixture("a*b");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase2_Valid, bdata::make({"a", "b"}))
{
    NFATestFixture nfaFixture("a|b");
    DFATestFixture dfaFixture("a|b");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase2_Invalid, bdata::make({"ba", "bb", "bbbbaaa"}))
{
    NFATestFixture nfaFixture("a|b");
    DFATestFixture dfaFixture("a|b");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase3_Valid, bdata::make({"a", "aa", "ba", "aaa", "baa"}))
{
    NFATestFixture nfaFixture("(a|b).a*");
    DFATestFixture dfaFixture("(a|b).a*");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase3_Invalid, bdata::make({"", "caa", "bcb", "abb"}))
{
    NFATestFixture nfaFixture("(a|b).a*");
    DFATestFixture dfaFixture("(a|b).a*");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase4_Valid, bdata::make({"aabb", "babb", "aaabbbabb", "aabbabb", "babbabb"}))
{
    NFATestFixture nfaFixture("(a|b)*abb");
    DFATestFixture dfaFixture("(a|b)*abb");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase4_Invalid, bdata::make({"", "a", "b", "bb", "aa", "aaab", "baab", "bbbbbb", "abbbbaaaaa"}))
{
    NFATestFixture nfaFixture("(a|b)*abb");
    DFATestFixture dfaFixture("(a|b)*abb");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase5_Valid, bdata::make({"a", "aa", "aaaaaaa"}))
{
    NFATestFixture nfaFixture("a+");
    DFATestFixture dfaFixture("a+");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase5_Invalid, bdata::make({"", "b", "aab", "ba", "bbbbb", "bbbbaaa"}))
{
    NFATestFixture nfaFixture("a+");
    DFATestFixture dfaFixture("a+");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase6_Valid, bdata::make({"ab", "aab", "aaaaaaab"}))
{
    NFATestFixture nfaFixture("a+b");
    DFATestFixture dfaFixture("a+b");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase6_Invalid, bdata::make({"b", "aa", "ba", "bbbbb", "bbbbaaa"}))
{
    NFATestFixture nfaFixture("a+b");
    DFATestFixture dfaFixture("a+b");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase7_Valid, bdata::make({"abc", "aabc", "aabbc", "aaabbbc"}))
{
    NFATestFixture nfaFixture("a+b+c");
    DFATestFixture dfaFixture("a+b+c");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase7_Invalid, bdata::make({"", "ab", "bc", "ac", "abbbbbcc"}))
{
    NFATestFixture nfaFixture("a+b+c");
    DFATestFixture dfaFixture("a+b+c");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase8_Valid, bdata::make({"cagd", "cbgd", "ccccbbggd", "cbbbbbbd", "cccad"}))
{
    NFATestFixture nfaFixture("c*(a|b*)g*d");
    DFATestFixture dfaFixture("c*(a|b*)g*d");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase8_Invalid, bdata::make({"aagd", "ccbh", "cbgbg"}))
{
    NFATestFixture nfaFixture("c*(a|b*)g*d");
    DFATestFixture dfaFixture("c*(a|b*)g*d");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase9_Valid, bdata::make({"aaabh", "aaaggg", "aaaaaagggf", "aaaaaagggfffff", "aaabbbbbbh", "aaabbbbbcl", "aaabbbbccccch", "aaabbbbbbd", "aaabbbbbdddd"}))
{
    NFATestFixture nfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    DFATestFixture dfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase9_Invalid, bdata::make({"g", "a", "aggg", "abbbb", "aaabbbbcc", "aaaaaaabbc", "aaabbclh", "aaadddd"}))
{
    NFATestFixture nfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    DFATestFixture dfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase10_Valid, bdata::make({"mmnoprrrs", "mnoprs"}))
{
    NFATestFixture nfaFixture("(m|n)*op+(q|r*)s");
    DFATestFixture dfaFixture("(m|n)*op+(q|r*)s");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase10_Invalid, bdata::make({"opr", "opxqs", "oopqq", "ooprrrs", "opqrs", "nnooprs"}))
{
    NFATestFixture nfaFixture("(m|n)*op+(q|r*)s");
    DFATestFixture dfaFixture("(m|n)*op+(q|r*)s");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase11_Valid, bdata::make({"ad", "bccef", "bbbcdgg", "bccccccd", "aefg", "bbbbcceffffg", "bbcefggggg"}))
{
    NFATestFixture nfaFixture("(a|b+).c*(d|ef+).g*");
    DFATestFixture dfaFixture("(a|b+).c*(d|ef+).g*");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase11_Invalid, bdata::make({"d", "c", "bccdd", "abg", "bcdh", "aaefg"}))
{
    NFATestFixture nfaFixture("(a|b+).c*(d|ef+).g*");
    DFATestFixture dfaFixture("(a|b+).c*(d|ef+).g*");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase12_Valid, bdata::make({"vv", "vvvv", "aavv", "bbbbvv", "vvv", "ccvvv",}))
{
    NFATestFixture nfaFixture("(aa|bb|cc)*vv+");
    DFATestFixture dfaFixture("(aa|bb|cc)*vv+");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase12_Invalid, bdata::make({"ba", ""}))
{
    NFATestFixture nfaFixture("(aa|bb|cc)*vv+");
    DFATestFixture dfaFixture("(aa|bb|cc)*vv+");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
}

BOOST_AUTO_TEST_SUITE_END()