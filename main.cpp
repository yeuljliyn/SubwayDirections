#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

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

vector<Node> nodes;
vector<vector<Edge>> graph;

int findNode(string station, int line)
{
    for (int i = 0; i < nodes.size(); i++)
    {
        if (nodes[i].station == station && nodes[i].line == line)
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

int main()
{
    return 0;
}