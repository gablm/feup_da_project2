#include "../manager.h"

void Manager::PrimMST(Vertex *base)
{
	std::priority_queue<Vertex *> queue;
	std::set<Vertex *> set;

	for (auto vtx : network.getVertexSet())
	{
		queue.emplace(vtx);
		set.insert(vtx);
		vtx->setPath(nullptr);
		vtx->setVisited(false);
		vtx->setDist(__DBL_MAX__);
	}
	base->setDist(0);

	while (!queue.empty())
	{
		Vertex *u = queue.top();
		queue.pop();
		set.erase(u);

		for (auto e : u->getAdj())
		{
			Vertex *v = e->getDest();
			if (set.find(v) != set.end() && e->getWeight() < v->getDist())
			{
				v->setPath(e);
				v->setDist(e->getWeight());
			}
		}
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

	std::cout << base->getId() << "->" << "\n";
	for (auto vtx : network.getVertexSet())
	{
		if (vtx->getPath() == nullptr) continue;
		std::cout  
		<< vtx->getPath()->getOrig()->getId() << "->";
	}

	std::cout << base->getId() << "\n";
	while (std::cin.get() != '\n') {}

	auto end = std::chrono::high_resolution_clock::now();
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}

