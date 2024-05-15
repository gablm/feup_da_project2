#include "../manager.h"

/**
 * Creates a minimum span tree using Prim's Algorithm.
 * @note Complexity: O(E log V)
 * @param base Pointer to starting vertex.
*/
Graph Manager::PrimMST(Vertex *base)
{
	Graph mst;
	std::priority_queue<Vertex *> queue;

	base->setDist(0);
	for (auto vtx : network.getVertexSet())
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

	for (auto vtx : network.getVertexSet())
	{
		Edge *edg = vtx->getPath();
		if (edg == nullptr) continue;
		mst.addBidirectionalEdge(edg->getDest()->getId(), edg->getOrig()->getId(), edg->getWeight());
	}

	return mst;
}

void dfs(Vertex *vtx, Vertex *last, std::vector<int> &stops, 
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
		dfs(v, vtx, stops, distances, total);
	}
}

ReturnDataTSP Manager::triangularApproximationHeuristic()
{
	auto start = std::chrono::high_resolution_clock::now();
	double totalDistance = 0;
	std::vector<int> stops;
	std::vector<double> distances;
	Vertex *base = network.getVertexSet().front();
	Graph mst = PrimMST(base);

	for (auto vtx : network.getVertexSet())
		vtx->setVisited(false);

	Vertex *mstBase = mst.findVertex(base->getId());
	dfs(mstBase, nullptr, stops, distances, &totalDistance);

	Vertex *last = network.findVertex(stops.back());
	stops.push_back(base->getId());
	Edge *lastEdge = last->getEdgeTo(base);
	double lastDist = lastEdge == nullptr ? 0 : lastEdge->getWeight();
	distances.push_back(lastDist);
	totalDistance += lastDist;

	auto end = std::chrono::high_resolution_clock::now();
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}

