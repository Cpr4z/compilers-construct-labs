#pragma once
#include "types.hpp"

namespace GraphTypes
{
    using Vertex = std::string;
    using Vertices = BasicTypes::Set<Vertex>;
    using Edges = BasicTypes::UMap<Vertex, Vertices>;
}