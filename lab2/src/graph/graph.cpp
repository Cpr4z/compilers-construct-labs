#include "graph.hpp"

Graph::Graph(GraphTypes::Vertices vertices, GraphTypes::Edges edges) :
                m_vertices(std::move(vertices)), m_edges(std::move(edges))
{
    for (const auto& [from, tos] : edges)
    {
        for (const auto& to : tos)
        {
            checkVertex(to);
        }
    }

}

void Graph::checkVertex(const GraphTypes::Vertex& vertex)
{
    if (m_vertices.contains(vertex))
    {
        throw std::invalid_argument("[Graph::checkVertex_] \"" + vertex + "\" is not in V");
    }
}

BasicTypes::Vector<GraphTypes::Vertex> Graph::topologicalSort() const
{
    BasicTypes::UMap<GraphTypes::Vertex, bool> visited;
    for (const auto& v : m_vertices)
    {
        visited[v] = false;
    }

    BasicTypes::Vector<GraphTypes::Vertex> order;
    auto dfs = [&](auto&& self, const GraphTypes::Vertex& vertex) ->void
            {
                visited[vertex] = true;
                if (auto adj = m_edges.find(vertex); adj != m_edges.end())
                {
                    for (const auto& u : adj->second)
                    {
                        if (!visited[u])
                        {
                            self(self, u);
                        }
                    }
                }
                order.push_back(vertex);
            };

    for (const auto& v: m_vertices)
    {
        if (!visited[v])
        {
            dfs(dfs, v);
        }
    }
    std::reverse(order.begin(), order.end());
    return order;
}
