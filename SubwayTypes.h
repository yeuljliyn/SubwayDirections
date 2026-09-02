#pragma once

#include <string>
#include <vector>

struct Segment
{
    int Line;
    std::string FromStation;
    std::string ToStation;
    int TimeSeconds;
};

struct Node
{
    std::string Station;
    int Line;
};

struct Edge
{
    int To;
    int TimeSeconds;
};

struct RouteResult
{
    bool Found = false;
    int TotalTimeSeconds = 0;
    std::vector<int> NodePath;
};