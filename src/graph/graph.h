#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>

class Edge;
class Vertex;
class Graph;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

class Vertex {
public:
    Vertex(int id);
	Vertex(int id, void *info);
    bool operator<(Vertex& vertex) const;

	int getId() const;
    void *getInfo() const;
    std::vector<Edge *> getAdj() const;
    bool isVisited() const;
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge *getPath() const;
    std::vector<Edge *> getIncoming() const;

    void setId(int id);
	void setInfo(void *info);
    void setVisited(bool visited);
    void setProcesssing(bool processing);
    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge *path);
    Edge *addEdge(Vertex *dest, double w);
    bool removeEdge(int id);
    void removeOutgoingEdges();
	
protected:
	int id;
    void *info;
    std::vector<Edge *> adj;

    bool visited = false;
    bool processing = false;
    unsigned int indegree;
    double dist = 0;
    Edge *path = nullptr;

    std::vector<Edge *> incoming;

    void deleteEdge(Edge *edge);
};

/********************** Edge  ****************************/

class Edge {
public:
    Edge(Vertex *orig, Vertex *dest, double w);

    Vertex *getDest() const;
    double getWeight() const;
    bool isSelected() const;
    Vertex *getOrig() const;
    Edge *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge *reverse);
    void setFlow(double flow);
	
protected:
	Vertex *orig;
    Vertex *dest;
    double weight;

    bool selected = false;
    Edge *reverse = nullptr;

    double flow;
};

/********************** Graph  ****************************/

class Graph {
public:
    ~Graph();

    Vertex *findVertex(int in) const;
    bool addVertex(int in, void *info);
    bool removeVertex(int in);

    bool addEdge(int sourc, int dest, double w);
    bool removeEdge(int source, int dest);
    bool addBidirectionalEdge(int sourc, int dest, double w);

    int getNumVertex() const;
    std::vector<Vertex *> getVertexSet() const;
	std::unordered_map<int, Vertex *> getVertexMap() const;
protected:
    std::vector<Vertex *> vertexSet;
	std::unordered_map<int, Vertex *> vertexMap;

    double **distMatrix = nullptr;
    int **pathMatrix = nullptr;
};

#endif