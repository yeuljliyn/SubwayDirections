#include <algorithm>
#include <queue>
#include <vector>

#include "PathFinder.h"

struct QueueItem
{
    int NodeID;
    int Time;

    bool operator<(const QueueItem& other) const
    {
        return Time > other.Time;
    }
};

RouteResult PathFinder::FindShortestPath(
    const SubwayGraph& subway,
    const std::string& startStation,
    const std::string& endStation) const
{
    const int INF = 1000000000;

    std::vector<int> distance(
        subway.GetNodeCount(),
        INF
    );

    std::vector<int> previous(
        subway.GetNodeCount(),
        -1
    );

    std::priority_queue<QueueItem> pathQueue;

    std::vector<int> startNodeIDs =
        subway.FindNodeIDs(startStation);

    for (int i = 0; i < startNodeIDs.size(); i++)
    {
        int nodeID = startNodeIDs[i];

        distance[nodeID] = 0;
        pathQueue.push({ nodeID, 0 });
    }

    while (!pathQueue.empty())
    {
        QueueItem currentItem = pathQueue.top();
        pathQueue.pop();

        int currentNodeID = currentItem.NodeID;
        int currentTime = currentItem.Time;

        if (distance[currentNodeID] < currentTime)
        {
            continue;
        }

        const std::vector<Edge>& edges =
            subway.GetEdges(currentNodeID);

        for (int i = 0; i < edges.size(); i++)
        {
            int nextNodeID = edges[i].To;
            int nextTime = edges[i].TimeSeconds;

            int newTime = currentTime + nextTime;

            if (newTime < distance[nextNodeID])
            {
                distance[nextNodeID] = newTime;
                previous[nextNodeID] = currentNodeID;

                pathQueue.push({
                    nextNodeID,
                    newTime
                    });
            }
        }
    }

    std::vector<int> endNodeIDs =
        subway.FindNodeIDs(endStation);

    int endNodeID = -1;
    int minimumTime = INF;

    for (int i = 0; i < endNodeIDs.size(); i++)
    {
        int nodeID = endNodeIDs[i];

        if (distance[nodeID] < minimumTime)
        {
            minimumTime = distance[nodeID];
            endNodeID = nodeID;
        }
    }

    RouteResult result;

    if (endNodeID == -1 || minimumTime == INF)
    {
        return result;
    }

    result.Found = true;
    result.TotalTimeSeconds = minimumTime;

    int currentNodeID = endNodeID;

    while (currentNodeID != -1)
    {
        result.NodePath.push_back(currentNodeID);
        currentNodeID = previous[currentNodeID];
    }

    std::reverse(
        result.NodePath.begin(),
        result.NodePath.end()
    );

    return result;
}