#include <array>
#include <iostream>
#include "graphviz.hpp"
#include "tokenizator.hpp"
#include "parser.hpp"
#include "utils.hpp"

namespace {

    bool sTest(size_t i, std::string input, bool expected_result)
    {
        Tokenizator tokenizator;
        auto tokens = tokenizator.tokenize(std::move(input));
        Parser parser(std::move(tokens));

        auto job = [&parser, &tokens]()
        {
            auto parsing = [&parser, &tokens](){ parser.parse();};
            return Utils::invokeNoexcept(parsing);
        };

        bool result = job();

        auto printExpected = [&](std::string_view what, bool expected)
        {
            std::cout << " -- Expected " << what << " == " << std::boolalpha << expected << ", got " << result << std::endl;
        };

        printExpected("result", expected_result);
        std::cout << "Test " << std::to_string(i) << (expected_result == result ? " passed" : " failed") << std::endl;
        return expected_result == result;
    }


    const std::array<std::tuple<std::string, bool>, 21> cTests =
    {{
        {"{a = const}", true},
        {"{a = const;}", true},
        {"{a = const; a = b * a}", true},
        {"{a = const; a = b * a; a = const}", true},
        {"{a = const; a = b * a; a = const;  a = (not a + b)}", true},
        {"{a = (b) * a}", true},
        {"{a = const; {a = -a}}", true},
        {"{a = const <> a - a}", true},
        {"{a = const <> a; {b = not const}}", true},
        {"{a = a div b mod c}", true},
        {"{a = a div b mod c; {a = +b <= const}}", true},
        {"{ a = -b + const < (not a + b) * b div a; {a = a <> a; {c = const}}}", true},
        {"{ a = b div c; a = a == g}", true},

        {"a = const", false},
        {"{a = const b = a}", false},
        {"(a = b +c)", false},
        {"{ a = const; v = a <> b", false},
        {"( b = a { k = k + a})", false},
        {" z = d + k", false},
        {"{ a = d div mod a}", false},
        {"{ a = (a + b; k = b}", false}
    }};

    void sRunTests() {
        size_t i = 1;
        size_t passed = 0;
        for (const auto& [input, expected_result] : cTests)
        {
            passed += sTest(i, input, expected_result);
            ++i;
        }
        std::cout << "Passed " << passed << "/" << cTests.size() << std::endl;
    }

}  // namespace


int main() {
//    sRunTests();

    std::string s = "{ a = -b + const < (not a + b) * b div a; {a = a <> a; {c = const}}}";
//    std::string s = "{ a = b div c; a = a == g}";

//    std::string s = "{a = b + v;}";
    Tokenizator tokenizator;
    auto tokens = tokenizator.tokenize(std::move(s));
    Parser parser(tokens);
    auto tree = parser.parse();
    std::cout << s << std::endl;
    Graphviz::saveDot(tree, "./tree.dot");
    Graphviz::GenerateSVGFromDotAndOpen("./tree.dot");
}

