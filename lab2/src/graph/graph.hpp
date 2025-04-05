#pragma once
#include "../types/graph.hpp"

class Graph
{
public:
    explicit Graph(GraphTypes::Vertices, GraphTypes::Edges);
    [[nodiscard]] BasicTypes::Vector<GraphTypes::Vertex> topologicalSort() const;

    const auto& GetVertices() const { return m_vertices; }
    const auto& GetEdges() const { return m_edges; };

private:
    void checkVertex(const GraphTypes::Vertex&);

private:
    GraphTypes::Vertices m_vertices;
    GraphTypes::Edges m_edges;
};
