#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

struct Node
{
    std::string Station;
    int Line;
};

struct Edge
{
    int To;
    int Time;
};

struct Path
{
    int NodeIndex;
    int Time;

    bool operator<(const Path& other) const
    {
        return Time > other.Time;
    }
};

std::vector<Node> nodes;
std::vector<std::vector<Edge>> graph;

int findNode(const std::string& station, int line)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].Station == station &&
            nodes[i].Line == line)
        {
            return i;
        }
    }

    nodes.push_back({ station, line });
    graph.push_back({});

    return nodes.size() - 1;
}

void addEdge(
    const std::string& firstStation,
    const std::string& secondStation,
    int line,
    int time)
{
    int firstIndex = findNode(firstStation, line);
    int secondIndex = findNode(secondStation, line);

    graph[firstIndex].push_back({ secondIndex, time });
    graph[secondIndex].push_back({ firstIndex, time });
}

void addLine(
    int line,
    const std::vector<std::string>& stations,
    const std::vector<int>& times)
{
    for (int i = 0; i < times.size(); i++)
    {
        addEdge(
            stations[i],
            stations[i + 1],
            line,
            times[i]
        );
    }
}

void addTransferEdge(int firstIndex, int secondIndex)
{
    if (nodes[firstIndex].Station != nodes[secondIndex].Station)
    {
        return;
    }

    if (nodes[firstIndex].Line == nodes[secondIndex].Line)
    {
        return;
    }

    const int TRANSFER_TIME = 180;

    graph[firstIndex].push_back({ secondIndex, TRANSFER_TIME });
    graph[secondIndex].push_back({ firstIndex, TRANSFER_TIME });
}

void addTransferEdges()
{
    for (int firstIndex = 0; firstIndex < nodes.size(); firstIndex++)
    {
        for (int secondIndex = firstIndex + 1;
            secondIndex < nodes.size();
            secondIndex++)
        {
            addTransferEdge(firstIndex, secondIndex);
        }
    }
}

bool stationExists(const std::string& station)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].Station == station)
        {
            return true;
        }
    }

    return false;
}

void makeSubway()
{
    addLine(
        1,
        {
            "용산", "남영", "서울역", "시청", "종각",
            "종로3가", "종로5가", "동대문", "동묘앞",
            "신설동", "제기동", "청량리"
        },
        {
            110, 120, 120, 100, 90,
            90, 90, 80, 80, 90, 100
        }
    );

    addLine(
        2,
        {
            "당산", "합정", "홍대입구", "신촌", "이대",
            "아현", "충정로", "시청", "을지로입구",
            "을지로3가", "을지로4가", "동대문역사문화공원",
            "신당", "상왕십리", "왕십리", "한양대"
        },
        {
            170, 100, 110, 90, 90,
            90, 110, 90, 90, 80,
            100, 100, 100, 90, 100
        }
    );

    addLine(
        3,
        {
            "경복궁", "안국", "종로3가", "을지로3가",
            "충무로", "동대입구", "약수", "금호", "옥수"
        },
        {
            100, 90, 70, 80,
            100, 90, 90, 90
        }
    );

    addLine(
        4,
        {
            "이촌", "신용산", "삼각지", "숙대입구",
            "서울역", "회현", "명동", "충무로",
            "동대문역사문화공원", "동대문", "혜화"
        },
        {
            100, 90, 100, 100, 90,
            90, 80, 100, 90, 90
        }
    );

    addLine(
        5,
        {
            "마포", "공덕", "애오개", "충정로",
            "서대문", "광화문", "종로3가", "을지로4가",
            "동대문역사문화공원", "청구", "신금호",
            "행당", "왕십리", "마장"
        },
        {
            100, 110, 100, 90,
            120, 100, 90, 90,
            100, 100, 100, 100, 100
        }
    );

    addLine(
        6,
        {
            "망원", "합정", "상수", "광흥창", "대흥",
            "공덕", "효창공원앞", "삼각지", "녹사평",
            "이태원", "한강진", "버티고개", "약수",
            "청구", "신당", "동묘앞", "창신"
        },
        {
            100, 100, 100, 100, 110,
            100, 130, 110, 90, 100,
            110, 90, 90, 90, 100, 90
        }
    );

    addTransferEdges();
}

void findShortestPath(
    const std::string& startStation,
    const std::string& endStation)
{
    const int INF = 1000000000;

    std::vector<int> distance(nodes.size(), INF);
    std::vector<int> previous(nodes.size(), -1);

    std::priority_queue<Path> pathQueue;

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].Station == startStation)
        {
            distance[i] = 0;
            pathQueue.push({ i, 0 });
        }
    }

    while (!pathQueue.empty())
    {
        Path currentPath = pathQueue.top();
        pathQueue.pop();

        int currentIndex = currentPath.NodeIndex;
        int currentTime = currentPath.Time;

        if (distance[currentIndex] < currentTime)
        {
            continue;
        }

        for (int edgeIndex = 0;
            edgeIndex < graph[currentIndex].size();
            edgeIndex++)
        {
            int nextIndex = graph[currentIndex][edgeIndex].To;
            int nextTime = graph[currentIndex][edgeIndex].Time;

            int newTime = currentTime + nextTime;

            if (newTime < distance[nextIndex])
            {
                distance[nextIndex] = newTime;
                previous[nextIndex] = currentIndex;

                pathQueue.push({ nextIndex, newTime });
            }
        }
    }

    int endNodeIndex = -1;
    int minimumTime = INF;

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].Station == endStation &&
            distance[i] < minimumTime)
        {
            minimumTime = distance[i];
            endNodeIndex = i;
        }
    }

    if (endNodeIndex == -1 || minimumTime == INF)
    {
        std::cout << "경로를 찾을 수 없습니다.\n";
        return;
    }

    std::vector<int> path;

    int currentIndex = endNodeIndex;

    while (currentIndex != -1)
    {
        path.push_back(currentIndex);
        currentIndex = previous[currentIndex];
    }

    std::reverse(path.begin(), path.end());

    std::cout << "\n[탐색 결과]\n";
    std::cout << startStation << " -> " << endStation << "\n";

    std::cout << "이동경로: ";

    for (int i = 0; i < path.size(); i++)
    {
        int nodeIndex = path[i];

        if (i > 0)
        {
            int previousNodeIndex = path[i - 1];

            if (nodes[nodeIndex].Station ==
                nodes[previousNodeIndex].Station &&
                nodes[nodeIndex].Line !=
                nodes[previousNodeIndex].Line)
            {
                continue;
            }
        }

        std::cout << nodes[nodeIndex].Station;

        if (i < path.size() - 1)
        {
            int nextNodeIndex = path[i + 1];

            if (nodes[nodeIndex].Station ==
                nodes[nextNodeIndex].Station &&
                nodes[nodeIndex].Line !=
                nodes[nextNodeIndex].Line)
            {
                std::cout << "(환승)";
            }

            std::cout << " -> ";
        }
    }

    int minutes = minimumTime / 60;
    int seconds = minimumTime % 60;

    std::cout << "\n";
    std::cout
        << "총 소요 시간: "
        << minutes << "분 "
        << seconds << "초\n";
}

int main()
{
    makeSubway();

    std::string startStation;
    std::string endStation;

    while (true)
    {
        std::cout << "출발 역 : ";
        std::cin >> startStation;

        std::cout << "도착 역 : ";
        std::cin >> endStation;

        if (!stationExists(startStation))
        {
            std::cout
                << "존재하지 않는 출발역입니다. 다시 입력해주세요.\n\n";
            continue;
        }

        if (!stationExists(endStation))
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

    findShortestPath(startStation, endStation);

    return 0;
}