#include <iostream>
#include <vector>
#include <string>
#include <exception>

#include "PathFinder.h"
#include "SubwayCsvLoader.h"
#include "SubwayGraph.h"

int main()
{
    const std::string csvPath =
        "data/subway_data.csv";

    std::vector<Segment> segments =
        SubwayCsvLoader::Load(csvPath);

    SubwayGraph subway;
    subway.Build(segments);

    PathFinder pathFinder;

    std::string startStation;
    std::string endStation;

    while (true)
    {
        std::cout << "출발 역 : ";
        std::cin >> startStation;

        std::cout << "도착 역 : ";
        std::cin >> endStation;

        if (!subway.HasStation(startStation))
        {
            std::cout
                << "존재하지 않는 출발역입니다. 다시 입력해주세요.\n\n";

            continue;
        }

        if (!subway.HasStation(endStation))
        {
            std::cout
                << "존재하지 않는 도착역입니다. 다시 입력해주세요.\n\n";

            continue;
        }

        if (startStation == endStation)
        {
            std::cout
                << "출발역과 도착역은 같을 수 없습니다. 다시 입력해주세요.\n\n";

            continue;
        }

        break;
    }

    RouteResult result =
        pathFinder.FindShortestPath(
            subway,
            startStation,
            endStation
        );

    if (!result.Found)
    {
        std::cout << "경로를 찾을 수 없습니다.\n";
        return 0;
    }

    std::cout << "\n[탐색 결과]\n";

    std::cout
        << startStation
        << " -> "
        << endStation
        << '\n';

    std::cout << "이동경로: ";

    for (int i = 0; i < result.NodePath.size(); i++)
    {
        int nodeID = result.NodePath[i];

        const Node& node =
            subway.GetNode(nodeID);

        if (i > 0)
        {
            int previousNodeID =
                result.NodePath[i - 1];

            const Node& previousNode =
                subway.GetNode(previousNodeID);

            if (node.Station == previousNode.Station &&
                node.Line != previousNode.Line)
            {
                continue;
            }
        }

        std::cout << node.Station;

        if (i < result.NodePath.size() - 1)
        {
            int nextNodeID =
                result.NodePath[i + 1];

            const Node& nextNode =
                subway.GetNode(nextNodeID);

            if (node.Station == nextNode.Station &&
                node.Line != nextNode.Line)
            {
                std::cout << "(환승)";
            }

            std::cout << " -> ";
        }
    }

    int minutes =
        result.TotalTimeSeconds / 60;

    int seconds =
        result.TotalTimeSeconds % 60;

    std::cout << '\n';

    std::cout
        << "총 소요 시간: "
        << minutes
        << "분 "
        << seconds
        << "초\n";

    return 0;
};