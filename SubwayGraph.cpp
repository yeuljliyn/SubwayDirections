#include "SubwayGraph.h"

void SubwayGraph::Build(
    const std::vector<Segment>& segments,
    int transferSeconds)
{
    mNodes.clear();
    mGraph.clear();

    for (int i = 0; i < segments.size(); i++)
    {
        const Segment& segment = segments[i];

        int from = getOrCreateNode(
            segment.FromStation,
            segment.Line
        );

        int to = getOrCreateNode(
            segment.ToStation,
            segment.Line
        );

        addUndirectedEdge(
            from,
            to,
            segment.TimeSeconds
        );
    }

    addTransferEdges(transferSeconds);
}

bool SubwayGraph::HasStation(
    const std::string& station) const
{
    for (int i = 0; i < mNodes.size(); i++)
    {
        if (mNodes[i].Station == station)
        {
            return true;
        }
    }

    return false;
}

std::vector<int> SubwayGraph::FindNodeIDs(
    const std::string& station) const
{
    std::vector<int> nodeIDs;

    for (int i = 0; i < mNodes.size(); i++)
    {
        if (mNodes[i].Station == station)
        {
            nodeIDs.push_back(i);
        }
    }

    return nodeIDs;
}

int SubwayGraph::GetNodeCount() const
{
    return mNodes.size();
}

const Node& SubwayGraph::GetNode(
    int nodeID) const
{
    return mNodes[nodeID];
}

const std::vector<Edge>& SubwayGraph::GetEdges(
    int nodeID) const
{
    return mGraph[nodeID];
}

int SubwayGraph::getOrCreateNode(
    const std::string& station,
    int line)
{
    for (int i = 0; i < mNodes.size(); i++)
    {
        if (mNodes[i].Station == station &&
            mNodes[i].Line == line)
        {
            return i;
        }
    }

    mNodes.push_back({ station, line });
    mGraph.push_back({});

    return mNodes.size() - 1;
}

void SubwayGraph::addUndirectedEdge(
    int from,
    int to,
    int timeSeconds)
{
    mGraph[from].push_back({ to, timeSeconds });
    mGraph[to].push_back({ from, timeSeconds });
}

void SubwayGraph::addTransferEdge(
    int firstIndex,
    int secondIndex,
    int transferSeconds)
{
    if (mNodes[firstIndex].Station !=
        mNodes[secondIndex].Station)
    {
        return;
    }

    if (mNodes[firstIndex].Line ==
        mNodes[secondIndex].Line)
    {
        return;
    }

    addUndirectedEdge(
        firstIndex,
        secondIndex,
        transferSeconds
    );
}

void SubwayGraph::addTransferEdges(
    int transferSeconds)
{
    for (int firstIndex = 0;
        firstIndex < mNodes.size();
        firstIndex++)
    {
        for (int secondIndex = firstIndex + 1;
            secondIndex < mNodes.size();
            secondIndex++)
        {
            addTransferEdge(
                firstIndex,
                secondIndex,
                transferSeconds
            );
        }
    }
}