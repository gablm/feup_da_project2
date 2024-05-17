#include "../manager.h"

/**
 * Creates a minimum span tree using Prim's Algorithm.
 * Uses the default graph
 * @note Complexity: O(E log V)
 * @param g Base graph to operate on.
 * @param base Pointer to starting vertex.
*/
Graph Manager::PrimMST(Graph &g, Vertex *base)
{
	Graph mst;
	std::priority_queue<Vertex *> queue;

	base->setDist(0);
	for (auto vtx : g.getVertexSet())
	{
		mst.addVertex(vtx->getId(), vtx->getInfo());
		vtx->setPath(nullptr);
		vtx->setVisited(false);
		vtx->setDist(__DBL_MAX__);
	}

	queue.push(base);
	while (!queue.empty())
	{
		Vertex *u = queue.top();
		queue.pop();
		if (u->isVisited()) continue;
		u->setVisited(true);

		for (auto e : u->getAdj())
		{
			Vertex *v = e->getDest();
			if (!v->isVisited() && e->getWeight() < v->getDist())
			{
				v->setPath(e);
				queue.push(v);
				v->setDist(e->getWeight());
			}
		}
	}

	for (auto vtx : g.getVertexSet())
	{
		Edge *edg = vtx->getPath();
		if (edg == nullptr) continue;
		mst.addBidirectionalEdge(edg->getDest()->getId(), edg->getOrig()->getId(), edg->getWeight());
	}

	return mst;
}

/**
 * Depth-first search though a graph (in this case a Minimum-span tree), 
 * calculating the distance between stops and saving them.
 * @param vtx First vertex.
 * @param last Vertex processed before, nullptr if vtx is the first.
 * @param stops Vector to be filled with the stops
 * @param distances Vector to be filled with the distances between stops
 * @param total Pointer to a value where the total distance will be saved.
 * @note Complexity: O(V + E) 
*/
void Manager::trianApproxDfs(Vertex *vtx, Vertex *last, std::vector<int> &stops, 
	std::vector<double> &distances, double *total)
{
	vtx->setVisited(true);
	stops.push_back(vtx->getId());

	if (last != nullptr)
	{
		Edge *edg = last->getEdgeTo(vtx);
		double dist = edg == nullptr ? 0 : edg->getWeight();
		distances.push_back(dist);
		*total += dist;
	}
	
	for (auto edg : vtx->getAdj())
	{
		Vertex *v = edg->getDest();
		if (v->isVisited()) continue;
		trianApproxDfs(v, vtx, stops, distances, total);
	}
}

/**
 * Uses a minimum-span tree (MST) to approximate the vertexes 
 * and calculate a solution to the Salesman Travelling Problem.
 * @note Complexity: O(V + E)
 * @return Structure containing the time elapsed, vector with stops, 
 * vector with distances between those stops (in order) and total distance travelled.
*/
ReturnDataTSP Manager::triangularApproximationHeuristic()
{
	auto start = std::chrono::high_resolution_clock::now();
	double totalDistance = 0;
	std::vector<int> stops;
	std::vector<double> distances;
	Vertex *base = network.getVertexSet().front();
	Graph mst = PrimMST(network, base);

	for (auto vtx : network.getVertexSet())
		vtx->setVisited(false);

	Vertex *mstBase = mst.findVertex(base->getId());
	trianApproxDfs(mstBase, nullptr, stops, distances, &totalDistance);

	Vertex *last = network.findVertex(stops.back());
	stops.push_back(base->getId());
	Edge *lastEdge = last->getEdgeTo(base);
	double lastDist = lastEdge == nullptr ? 0 : lastEdge->getWeight();
	distances.push_back(lastDist);
	totalDistance += lastDist;

	auto end = std::chrono::high_resolution_clock::now();
	mst.resetGraph();
	
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}

