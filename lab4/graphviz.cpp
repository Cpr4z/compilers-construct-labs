#include "graphviz.hpp"
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include "utils.hpp"

namespace
{
struct Vertex
{
    size_t m_id{};
    std::string m_label;

    std::strong_ordering operator<=>(const Vertex&) const = default;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex>;

    void sGenerateDot(const Node& tree, const std::filesystem::path& path)
    {
        Graph graph;
        const auto vertex = [&graph](size_t id, const std::string& name)
        {
            for (const auto& v : boost::make_iterator_range(vertices(graph)))
            {
                if (graph[v] == Vertex{id, name})
                {
                    return v;
                }
            }
            return boost::add_vertex({id, name}, graph);
        };

        size_t count = 0;
        std::stack<std::pair<Node, size_t>> stack;
        stack.emplace(tree, count);
        while (!stack.empty())
        {
            auto [n, num] = stack.top();
            stack.pop();
            auto u = vertex(num, n.m_data.m_value + "\\nid: " + std::to_string(num));
            for (const auto& child : n.m_children)
            {
                ++count;
                stack.emplace(child, count);
                auto v = vertex(count, child.m_data.m_value + " " + std::to_string(count));
                boost::add_edge(u, v, graph);
            }
        }
        boost::dynamic_properties dp;
        dp.property("node_id", boost::get(&Vertex::m_id, graph));
        dp.property("label", boost::get(&Vertex::m_label, graph));

        std::ofstream file{path};
        boost::write_graphviz_dp(file, graph, dp);
        boost::write_graphviz(file, graph);
    }

    std::string sReadFile(const std::filesystem::path& path) {
        std::ifstream t{path};
        t.seekg(0, std::ios::end);
        auto size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size);
        return buffer;
    }

    void GenerateSVGFromDotAndOpen(std::string_view dot)
    {
        const boost::filesystem::path path_to_dot(dot);
        const boost::filesystem::path path_to_svg = [&path_to_dot]() { auto tmp = path_to_dot; return tmp.replace_extension(".svg"); } ();

        auto func = [&path_to_dot, &path_to_svg]()
                {
            auto generate_svg_command = (boost::format("dot -Tsvg %s -o %s") % path_to_dot.string() % path_to_svg.string()).str();
            std::system(generate_svg_command.c_str());
        };

        Utils::invokeWithFrozenConsoleStream(func);
    }
} // namespace

namespace Graphviz {
void saveDot(const Node& tree, const std::filesystem::path& path)
{
    sGenerateDot(tree, path);
}

void GenerateSVGFromDotAndOpen(const std::filesystem::path& path)
{
    return ::GenerateSVGFromDotAndOpen(path.string());
}

}  // namespace Graphviz
