#pragma once

#include <string>
#include <vector>

#include "SubwayTypes.h"

class SubwayGraph
{
public:
    void Build(
        const std::vector<Segment>& segments,
        int transferSeconds = 180);

    bool HasStation(
        const std::string& station) const;

    std::vector<int> FindNodeIDs(
        const std::string& station) const;

    int GetNodeCount() const;

    const Node& GetNode(
        int nodeID) const;

    const std::vector<Edge>& GetEdges(
        int nodeID) const;

private:
    int getOrCreateNode(
        const std::string& station,
        int line);

    void addUndirectedEdge(
        int from,
        int to,
        int timeSeconds);

    void addTransferEdge(
        int firstIndex,
        int secondIndex,
        int transferSeconds);

    void addTransferEdges(
        int transferSeconds);

private:
    std::vector<Node> mNodes;
    std::vector<std::vector<Edge>> mGraph;
};