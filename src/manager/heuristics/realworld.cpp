#include "../manager.h"
#include "../../ui/ui.h"

/**
 * Finds the "perfect match" edge for each vertex.
 * A "perfect match" is an edge where the distance 
 * between the base vertex and another vertex is the minimal.
 * That is the best path option only considering that vertex.
 * @param mst Minimum-span tree graph.
 * @return Vector of edges that are "perfect matches".
 * @note Complexity: O(V + E)
*/
std::vector<Edge *> Manager::perfectMatching(Graph &mst)
{
	std::vector<Edge *> matches;
	std::vector<Vertex *> oddVertexes;
	
	for (auto vtx : mst.getVertexSet())
	{
		vtx->setVisited(false);
		if (vtx->getAdj().size() % 2) oddVertexes.push_back(vtx);
	}

	for (auto vtx : mst.getVertexSet())
	{
		if (vtx->isVisited()) continue;

		Edge *match = nullptr;

		for (auto e : vtx->getAdj())
		{
			Vertex *u = e->getDest();
			if (u->isVisited()) continue;
			
			if (match == nullptr || e->getWeight() < match->getWeight()) match = e;
		}

		if (match != nullptr)
		{
			matches.push_back(match);
			vtx->setVisited(true);
			match->getDest()->setVisited(true);
		}
	}

	return matches;
}

/**
 * Returns an Euler Path, which is a trail that visits every edge exacly once.
 * Vertexes can be re-visited.
 * @param base Base Vertex where the path/walk will start.
 * @return Vector containing the Euler Path/Walk.
 * @note Complexity: O(V)
*/
std::vector<Vertex *> Manager::eulerPath(Vertex *base)
{
	std::vector<Vertex *> stops;
	std::stack<Vertex *> stk;
	stk.push(base);

	while (!stk.empty())
	{
		Vertex *u = stk.top();
		if (u->getAdj().size() == 0)
		{
			stops.push_back(u);
			stk.pop();
			continue;
		}
		
		for (auto edg : u->getAdj())
			stk.push(edg->getDest());
		u->clearAdj();
	}

	return stops;
}

/**
 * Generates an Hamilton Circuit from a Euler Path.
 * Goes through the Euler Path and removed non-unique vertexes from it.
 * @param eulerStops Euler Path/Walk.
 * @return Vector with the stops of the Hamilton Circuit.
 * @note Complexity: O(V)
*/
std::vector<int> Manager::hamilton(std::vector<Vertex *> &eulerStops)
{
	std::unordered_set<Vertex *> vertexes;
	std::vector<int> stops;

	for (auto vtx : eulerStops)
	{
		if (vertexes.find(vtx) == vertexes.end())
		{
			vertexes.insert(vtx);
			stops.push_back(vtx->getId());
		}
	}

	return stops;
}

/**
 * Checks if the path inside the (stops) vector is valid.
 * It goes through every pair of sequential nodes and checks if there is a edge between both in the graph.
 * The distance between stops is also calculated during this verification.
 * @note Complexity: O(V)
 * @param g Network graph
 * @param stops Vector containing the stops of the path.
 * @param dists Vector where the distance between stops is saved.
 * @param totalDist Pointer to double where the total distanced travelled is saved.
 * @return If the path is invalid.
*/
bool Manager::invalidPath(Graph &g, std::vector<int> &stops, std::vector<double> &dists, double *totalDist)
{
	for (size_t i = 0; i < stops.size() - 1; i++)
	{
		Vertex *u = g.findVertex(stops[i]);
		Vertex *v = g.findVertex(stops[i + 1]);
		Edge *edg = u->getEdgeTo(v);
		if (edg == nullptr) return true;
		dists.push_back(edg->getWeight());
		*totalDist += edg->getWeight();
	}

	return false;
}

/**
 * THe Christofides algorithm is used to generated 
 * a approximated solution to the travelling salesman problem.
 * It starts by generating a minimum-span tree for the graph. 
 * Then for every vertex, a matching edge with the lowest distance 
 * is added to the mst.
 * After this, a euler walk is generated and from this, an hamilton circuit.
 * As some graph are not fully connected, a valid path is not guaranteed.
 * @param base Base vertex where the tour will be started.
 * @note Complexity: O(V + E)
 * @return Structure with the time taken, stops, distances between stops and max distance travelled.
*/
ReturnDataTSP Manager::realWorldHeuristic(Vertex *base)
{
	auto start = std::chrono::high_resolution_clock::now();
	double totalDistance = 0;
	std::vector<int> stops;
	std::vector<double> distances;

	// create mst
	Graph mst = PrimMST(network, base);

	// perfect match odd nodes
	std::vector<Edge *> matches = perfectMatching(mst);

	// join edges to mst
	for (auto edg : matches)
	{
		mst.addBidirectionalEdge(
			edg->getDest()->getId(),
			edg->getOrig()->getId(),
			edg->getWeight()
		);
	}

	// do euler path
	Vertex *mstBase = mst.findVertex(base->getId());
	std::vector<Vertex *> eulerStops = eulerPath(mstBase);

	// do hamilton path
	stops = hamilton(eulerStops);
	stops.push_back(base->getId());

	if (invalidPath(network, stops, distances, &totalDistance))
	{
		UI::showMessage(
			"NO AVAILABLE PATH",
			"The algorithm was not able to find a valid path."
		);
		return {-2, {}, {}, -2};
	}

	auto end = std::chrono::high_resolution_clock::now();
	mst.resetGraph();
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}
