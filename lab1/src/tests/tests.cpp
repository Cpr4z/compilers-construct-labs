#define BOOST_TEST_MODULE Lab1Tests
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "Utils.h"

#include "../AST.hpp"
#include "../AutomationFactory.hpp"
#include "../NFABuilder.hpp"
#include "../DFABuilder.hpp"
#include "../DFAMinimizator.hpp"

namespace bdata = boost::unit_test::data;

TokensSequence preprocessingTest(std::string&& regex)
{
    return Utils::preprocessing::validateRegex(std::move(regex));
}

struct NFATestFixture
{
    explicit NFATestFixture(std::string&& regex)
    {
        TokensSequence polishedSequence = Utils::preprocessing::validateRegex(std::move(regex));
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(Utils::AutomationType::e::NFA);
        IAutomationBuilderPtr builder = factory->CreateStateMachineBuilder();
        if (const NFABuilderPtr& nfaBuilder = std::dynamic_pointer_cast<NFABuilder>(builder))
        {
            nfaBuilder->Init(std::move(polishedSequence));
            m_nfa = nfaBuilder->Build();
        }
    }

    bool Imitate(std::string input)
    {
        if (m_nfa)
        {
            return m_nfa->Imitate(input);
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
        IAutomationFactoryPtr factory = AutomationFactory::CreateStateMachineFactory(Utils::AutomationType::e::DFA);
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

    bool Imitate(std::string input)
    {
        if (m_dfa)
        {
            return m_dfa->Imitate(input);
        }
        return false;
    }

    DFAPtr GetDFA() const
    {
        return std::static_pointer_cast<DFA>(m_dfa);
    }

private:
    IAutomationPtr m_dfa;
};

struct MinDFATestFixture
{
    explicit MinDFATestFixture(std::string regex)
    {
        DFATestFixture dfaTestFixture(std::move(regex));
        DFAMinimizatorPtr dfaMinimizator = DFAMinimizator::Instance();
        DFAPtr dfa = dfaTestFixture.GetDFA();
        if (dfa)
        {
            m_minDFA = dfaMinimizator->MinimizeKhophort(dfa);
        }
    }

    bool Imitate(std::string input)
    {
        if (m_minDFA)
        {
            return m_minDFA->Imitate(input);
        }
        return false;
    }

private:
    IAutomationPtr m_minDFA;
};

struct MinFAFixture
{
    explicit MinFAFixture(std::string&& regex)
    {
        DFAMinimizatorPtr minimizator = DFAMinimizator::Instance();
        m_minFA = minimizator->BuildFA(std::move(regex));
    }

    bool Imitate(std::string input)
    {
        if (m_minFA)
        {
            return m_minFA->Imitate(input);
        }
        return false;
    }

private:
    IAutomationPtr m_minFA;
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

BOOST_DATA_TEST_CASE(NFA_TestCase1_Valid, bdata::make({"b", "ab", "aaaaaaab", "aaab"}))
{
    NFATestFixture nfaFixture("a*b");
    DFATestFixture dfaFixture("a*b");
    MinDFATestFixture minDfaTestFixture("a*b");
    MinFAFixture minFaFixture("a*b");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase1_Invalid, bdata::make({"ba", "bbbbaaa", "a", "abab"}))
{
    NFATestFixture nfaFixture("a*b");
    DFATestFixture dfaFixture("a*b");
    MinDFATestFixture minDfaTestFixture("a*b");
    MinFAFixture minFaFixture("a*b");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase2_Valid, bdata::make({"a", "b"}))
{
    NFATestFixture nfaFixture("a|b");
    DFATestFixture dfaFixture("a|b");
    MinDFATestFixture minDfaTestFixture("a|b");
    MinFAFixture minFaFixture("a|b");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase2_Invalid, bdata::make({"", "ba", "bb", "bbbbaaa", "aaabb"}))
{
    NFATestFixture nfaFixture("a|b");
    DFATestFixture dfaFixture("a|b");
    MinDFATestFixture minDfaTestFixture("a|b");
    MinFAFixture minFaFixture("a|b");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase3_Valid, bdata::make({"a", "aa", "ba", "aaa", "baa"}))
{
    NFATestFixture nfaFixture("(a|b).a*");
    DFATestFixture dfaFixture("(a|b).a*");
    MinDFATestFixture minDfaTestFixture("(a|b).a*");
    MinFAFixture minFaFixture("(a|b).a*");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase3_Invalid, bdata::make({"", "caa", "bcb", "abb"}))
{
    NFATestFixture nfaFixture("(a|b).a*");
    DFATestFixture dfaFixture("(a|b).a*");
    MinDFATestFixture minDfaTestFixture("(a|b).a*");
    MinFAFixture minFaFixture("(a|b).a*");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase4_Valid, bdata::make({"a", "aa", "aaaaaaa"}))
{
    NFATestFixture nfaFixture("a+");
    DFATestFixture dfaFixture("a+");
    MinDFATestFixture minDfaTestFixture("a+");
    MinFAFixture minFaFixture("a+");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase4_Invalid, bdata::make({"", "b", "aab", "ba", "bbbbb", "bbbbaaa"}))
{
    NFATestFixture nfaFixture("a+");
    DFATestFixture dfaFixture("a+");
    MinDFATestFixture minDfaTestFixture("a+");
    MinFAFixture minFaFixture("a+");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase5_Valid, bdata::make({"ab", "aab", "aaaaaaab", "aaaaaaaaaaaaaaaaab"}))
{
    NFATestFixture nfaFixture("a+b");
    DFATestFixture dfaFixture("a+b");
    MinDFATestFixture minDfaTestFixture("a+b");
    MinFAFixture minFaFixture("a+b");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase5_Invalid, bdata::make({"", "b", "aa", "ba", "bbbbb", "bbbbaaa", "adsas"}))
{
    NFATestFixture nfaFixture("a+b");
    DFATestFixture dfaFixture("a+b");
    MinDFATestFixture minDfaTestFixture("a+b");
    MinFAFixture minFaFixture("a+b");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase6_Valid, bdata::make({"abcv", "aabcv", "aabbcv", "aaabbbcv", "abbcccv"}))
{
    NFATestFixture nfaFixture("a+b+c+v");
    DFATestFixture dfaFixture("a+b+c+v");
    MinDFATestFixture minDfaTestFixture("a+b+c+v");
    MinFAFixture minFaFixture("a+b+c+v");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase6_Invalid, bdata::make({"", "ab", "bc", "ac", "abbbbbcc", "dfad", "fwfeeefefe"}))
{
    NFATestFixture nfaFixture("a+b+c+v");
    DFATestFixture dfaFixture("a+b+c+v");
    MinDFATestFixture minDfaTestFixture("a+b+c+v");
    MinFAFixture minFaFixture("a+b+c+v");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase8_Valid, bdata::make({"aaabh", "aaabbbbbcl", "aaabbbbccccch", "aaabbbbbdddd"}))
{
    NFATestFixture nfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    DFATestFixture dfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    MinDFATestFixture minDfaTestFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    MinFAFixture minFaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase8_Invalid, bdata::make({"g", "a", "aggg", "abbbb", "aaabbbbcc", "aaaaaaabbc", "aaabbclh", "aaadddd"}))
{
    NFATestFixture nfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    DFATestFixture dfaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    MinDFATestFixture minDfaTestFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    MinFAFixture minFaFixture("a+.(a+.g+.f*|b+.(c*.(h|l+)|d+))");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase9_Invalid, bdata::make({"", "opr", "opxqs", "oopqq", "ooprrrs", "nnooprs"}))
{
    NFATestFixture nfaFixture("(m|n)*op+(q|r*)s");
    DFATestFixture dfaFixture("(m|n)*op+(q|r*)s");
    MinDFATestFixture minDfaTestFixture("(m|n)*op+(q|r*)s");
    MinFAFixture minFaFixture("(m|n)*op+(q|r*)s");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase10_Valid, bdata::make({"ad", "bbbcdgg", "bccccccd", "aefg", "bbcefggggg"}))
{
    NFATestFixture nfaFixture("(a|b+).c*(d|ef+).g*");
    DFATestFixture dfaFixture("(a|b+).c*(d|ef+).g*");
    MinDFATestFixture minDfaTestFixture("(a|b+).c*(d|ef+).g*");
    MinFAFixture minFaFixture("(a|b+).c*(d|ef+).g*");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase10_Invalid, bdata::make({"", "d", "c", "bccdd", "abg", "bcdh", "aaefg"}))
{
    NFATestFixture nfaFixture("(a|b+).c*(d|ef+).g*");
    DFATestFixture dfaFixture("(a|b+).c*(d|ef+).g*");
    MinDFATestFixture minDfaTestFixture("(a|b+).c*(d|ef+).g*");
    MinFAFixture minFaFixture("(a|b+).c*(d|ef+).g*");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase11_Valid, bdata::make({"vv", "vvvv", "aavv", "bbbbvv", "vvv", "ccvvv",}))
{
    NFATestFixture nfaFixture("(aa|bb|cc)*vv+");
    DFATestFixture dfaFixture("(aa|bb|cc)*vv+");
    MinDFATestFixture minDfaTestFixture("(aa|bb|cc)*vv+");
    MinFAFixture minFaFixture("(aa|bb|cc)*vv+");
    BOOST_CHECK(nfaFixture.Imitate(sample));
    BOOST_CHECK(dfaFixture.Imitate(sample));
    BOOST_CHECK(minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(minFaFixture.Imitate(sample));
}

BOOST_DATA_TEST_CASE(NFA_TestCase11_Invalid, bdata::make({"ba", "", "dfad", "bav", "avv", "aaavv", "aabb"}))
{
    NFATestFixture nfaFixture("(aa|bb|cc)*vv+");
    DFATestFixture dfaFixture("(aa|bb|cc)*vv+");
    MinDFATestFixture minDfaTestFixture("(aa|bb|cc)*vv+");
    MinFAFixture minFaFixture("(aa|bb|cc)*vv+");
    BOOST_CHECK(!nfaFixture.Imitate(sample));
    BOOST_CHECK(!dfaFixture.Imitate(sample));
    BOOST_CHECK(!minDfaTestFixture.Imitate(sample));
    BOOST_CHECK(!minFaFixture.Imitate(sample));
}

BOOST_AUTO_TEST_SUITE_END()