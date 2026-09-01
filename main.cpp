#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

struct Node
{
    string station;
    int line;
};

struct Edge
{
    int to;
    int time;
};

struct Path
{
    int node;
    int time;

    bool operator<(const Path& other) const
    {
        return time > other.time;
    }
};

vector<Node> nodes;
vector<vector<Edge>> graph;

int findNode(string station, int line)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].station == station &&
            nodes[i].line == line)
        {
            return i;
        }
    }

    nodes.push_back({ station, line });
    graph.push_back({});

    return nodes.size() - 1;
}

void addEdge(string a, string b, int line, int time)
{
    int aIndex = findNode(a, line);
    int bIndex = findNode(b, line);

    graph[aIndex].push_back({ bIndex, time });
    graph[bIndex].push_back({ aIndex, time });
}

void addLine(int line, vector<string> stations, vector<int> times)
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

void addTransferEdges()
{
    for (int i = 0; i < nodes.size(); i++)
    {
        for (int j = i + 1; j < nodes.size(); j++)
        {
            if (nodes[i].station == nodes[j].station &&
                nodes[i].line != nodes[j].line)
            {
                graph[i].push_back({ j, 180 });
                graph[j].push_back({ i, 180 });
            }
        }
    }
}

bool stationExists(string station)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].station == station)
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

void findShortestPath(string startStation, string endStation)
{
    const int INF = 1000000000;

    vector<int> distance(nodes.size(), INF);
    vector<int> previous(nodes.size(), -1);

    priority_queue<Path> pq;

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].station == startStation)
        {
            distance[i] = 0;
            pq.push({ i, 0 });
        }
    }

    while (!pq.empty())
    {
        Path currentPath = pq.top();
        pq.pop();

        int current = currentPath.node;
        int currentTime = currentPath.time;

        if (distance[current] < currentTime)
        {
            continue;
        }

        for (int i = 0; i < graph[current].size(); i++)
        {
            int next = graph[current][i].to;
            int nextTime = graph[current][i].time;

            int newTime = currentTime + nextTime;

            if (newTime < distance[next])
            {
                distance[next] = newTime;
                previous[next] = current;

                pq.push({ next, newTime });
            }
        }
    }

    int endNode = -1;
    int minTime = INF;

    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].station == endStation &&
            distance[i] < minTime)
        {
            minTime = distance[i];
            endNode = i;
        }
    }

    if (endNode == -1 || minTime == INF)
    {
        cout << "경로를 찾을 수 없습니다.\n";
        return;
    }

    vector<int> path;

    int current = endNode;

    while (current != -1)
    {
        path.push_back(current);
        current = previous[current];
    }

    reverse(path.begin(), path.end());

    cout << "\n[탐색 결과]\n";
    cout << startStation << " -> " << endStation << "\n";

    cout << "이동경로: ";

    for (int i = 0; i < path.size(); i++)
    {
        bool isTransfer = false;

        if (i < path.size() - 1 &&
            nodes[path[i]].station == nodes[path[i + 1]].station &&
            nodes[path[i]].line != nodes[path[i + 1]].line)
        {
            isTransfer = true;
        }

        if (i > 0 &&
            nodes[path[i]].station == nodes[path[i - 1]].station &&
            nodes[path[i]].line != nodes[path[i - 1]].line)
        {
            continue;
        }

        cout << nodes[path[i]].station;

        if (isTransfer)
        {
            cout << "(환승)";
        }

        if (i < path.size() - 1)
        {
            cout << " -> ";
        }
    }

    cout << "\n";

    int minutes = minTime / 60;
    int seconds = minTime % 60;

    cout << "총 소요 시간: "
        << minutes << "분 "
        << seconds << "초\n";
}

int main()
{
    makeSubway();

    string startStation;
    string endStation;

    while (true)
    {
        cout << "출발 역 : ";
        cin >> startStation;

        cout << "도착 역 : ";
        cin >> endStation;

        if (!stationExists(startStation))
        {
            cout << "존재하지 않는 출발역입니다. 다시 입력해주세요.\n\n";
            continue;
        }

        if (!stationExists(endStation))
        {
            cout << "존재하지 않는 도착역입니다. 다시 입력해주세요.\n\n";
            continue;
        }

        if (startStation == endStation)
        {
            cout << "출발역과 도착역은 같을 수 없습니다. 다시 입력해주세요.\n\n";
            continue;
        }

        break;
    }

    findShortestPath(startStation, endStation);

    return 0;
}