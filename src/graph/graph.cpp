#include "graph.h"
#include <sstream>

/************************* Info **************************/

Info::Info() {}
Info::Info(std::string label) : label(label) {}
Info::Info(double longt, double lat) : lon(longt), lat(lat) {}

/** 
 * Converts the Vertex Information to string.
 * If latitude and longitude are -1, the label will be returned.
*/
std::string Info::toStr() {
	if (lat != -1 && lon != -1) {
		std::ostringstream out;
		out << lon << ", " << lat;
		return out.str();
	}
	return label;
}

/************************* Vertex **************************/

Vertex::Vertex(int id) : id(id) {}
Vertex::Vertex(int id, Info info) : id(id), info(info) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
*/
Edge *Vertex::addEdge(Vertex *d, double w) {
    auto newEdge = new Edge(this, d, w);
    adj.push_back(newEdge);
	adjMap[d->getId()] = newEdge;
    d->incomingBlock.lock();
	d->incoming.push_back(newEdge);
	d->incomingBlock.unlock();
    return newEdge;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
bool Vertex::removeEdge(int in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getId() == in) {
            it = adj.erase(it);
			adjMap.erase(in);
            deleteEdge(edge);
            removedEdge = true;
        }
        else {
            it++;
        }
    }
    return removedEdge;
}

/*
 * Auxiliary function to remove an outgoing edge of a vertex.
 */
void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
		adjMap.erase(edge->getDest()->getId());
        deleteEdge(edge);
    }
}

bool Vertex::isConnectedTo(Vertex *dest) const {
	try
	{
		return adjMap.at(dest->id) != nullptr;
	}
	catch (const std::exception& e)
	{
		return false;
	}
}

bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

int Vertex::getId() const {
    return this->id;
}

Info Vertex::getInfo() const {
    return this->info;
}

std::vector<Edge *> Vertex::getAdj() const {
    return this->adj;
}
std::unordered_map<int, Edge *> Vertex::getAdjMap() const{
    return this->adjMap;
}

bool Vertex::isVisited() const {
    return this->visited;
}

bool Vertex::isProcessing() const {
    return this->processing;
}

unsigned int Vertex::getIndegree() const {
    return this->indegree;
}

double Vertex::getDist() const {
    return this->dist;
}

Edge *Vertex::getPath() const {
    return this->path;
}

std::vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}

void Vertex::setId(int id) {
    this->id = id;
}

void Vertex::setInfo(Info info) {
    this->info = info;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setProcesssing(bool processing) {
    this->processing = processing;
}

void Vertex::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

void Vertex::setDist(double dist) {
    this->dist = dist;
}

void Vertex::setPath(Edge *path) {
    this->path = path;
}

void Vertex::deleteEdge(Edge *edge) {
    Vertex *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getId() == id) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, double w, bool initial)
	: orig(orig), dest(dest), weight(w), initial(initial) {}

Vertex *Edge::getDest() const {
    return this->dest;
}

double Edge::getWeight() const {
    return this->weight;
}

Vertex *Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

bool Edge::isSelected() const {
    return this->selected;
}

bool Edge::isInitial() const {
    return this->initial;
}

double Edge::getFlow() const {
    return flow;
}

void Edge::setSelected(bool selected) {
    this->selected = selected;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(double flow) {
    this->flow = flow;
}

/********************** Graph  ****************************/

Graph::~Graph() {}

int Graph::getNumVertex() const {
    return vertexSet.size();
}

std::vector<Vertex *> Graph::getVertexSet() const {
    return vertexSet;
}

std::unordered_map<int, Vertex*> Graph::getVertexMap() const{
    return vertexMap;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
Vertex *Graph::findVertex(int id) const {
	try
	{
		return vertexMap.at(id);
	}
	catch(const std::exception& e)
	{
		for (auto v : vertexSet)
        	if (v->getId() == id)
           		return v;
    	return nullptr;
	}
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
bool Graph::addVertex(int id, Info info) {
	if (findVertex(id) != nullptr)
    	return false;
	
	Vertex *vtx = new Vertex(id, info);
	vertexMap[id] = vtx;
	vertexSet.push_back(vtx);
	return true;
}

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
bool Graph::removeVertex(int id) {
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->getId() == id) {
            auto v = *it;
            v->removeOutgoingEdges();
            for (auto u : vertexSet)
                u->removeEdge(id);
			vertexMap.erase(id);
            vertexSet.erase(it);
            delete v;
            return true;
        }
    }
    return false;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
bool Graph::addEdge(int sourc, int dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addEdge(v2, w);
    return true;
}

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
bool Graph::removeEdge(int sourc, int dest) {
    Vertex* srcVertex = findVertex(sourc);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

bool Graph::addBidirectionalEdge(int sourc, int dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, w);
    auto e2 = v2->addEdge(v1, w);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

/**
 * Erases all vertexes and edges from the graph.
 * @note Complexity: O(V + E)
*/
void Graph::resetGraph() {
	for (auto v : vertexSet) {
		for (auto e : v->getAdj())
			delete e;
		delete v;
	}
	vertexSet.clear();
	vertexMap.clear();
}
