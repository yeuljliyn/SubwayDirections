#pragma once

#include <string>

#include "SubwayGraph.h"
#include "SubwayTypes.h"

class PathFinder
{
public:
    RouteResult FindShortestPath(
        const SubwayGraph& subway,
        const std::string& startStation,
        const std::string& endStation) const;
};