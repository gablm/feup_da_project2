#include "../manager.h"
#include "../../ui/ui.h"
#include <stack>

ReturnDataTSP Manager::realWorldHeuristic(Vertex *base)
{
	auto start = std::chrono::high_resolution_clock::now();
	double totalDistance = 0;
	std::vector<int> stops;
	std::vector<double> distances;

	for (auto vtx : network.getVertexSet()) {
		if (vtx->getAdj().size() < 2)
		{
			UI::showMessage(
				"INVALID GRAPH",
				"At least one or more nodes only have one or less edges. \nThis means a path visiting every nodes only once is impossible to obtain."
			);
			return {-1, {}, {}, -1};
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}
