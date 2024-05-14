#include "../manager.h"

/**
 * Creates a minimum span tree using Prim's Algorithm.
 * @note Complexity: O(E log V)
 * @param base Pointer to starting vertex.
*/
void Manager::PrimMST(Vertex *base)
{
	std::priority_queue<Vertex *> queue;

	base->setDist(0);
	for (auto vtx : network.getVertexSet())
	{
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
}

void dfs(Vertex *vtx, Vertex *last, std::vector<int> &stops, 
	std::vector<double> &distances, double *total)
{
	vtx->setVisited(true);
	stops.push_back(vtx->getId());

	if (last != nullptr)
	{
		double dist = last->getEdgeTo(vtx)->getWeight();
		distances.push_back(dist);
		*total += dist;
	}

	if (vtx->getPath() != nullptr)
	{
		Vertex *v = vtx->getPath()->getOrig();
		if (!v->isVisited())
			dfs(v, vtx, stops, distances, total);
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
	PrimMST(base);

	for (auto vtx : network.getVertexSet())
		vtx->setVisited(false);

	dfs(base, nullptr, stops, distances, &totalDistance);
	Vertex *last = network.findVertex(stops.back());
	stops.push_back(base->getId());
	double lastDist = last->getEdgeTo(base)->getWeight();
	distances.push_back(lastDist);
	totalDistance += lastDist;

	auto end = std::chrono::high_resolution_clock::now();
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}

