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

int main()
{
    return 0;
}