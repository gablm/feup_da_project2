#include "../manager.h"

ReturnDataTSP Manager::triangularApproximationHeuristic()
{
	if (dType == real_world) return {-1, {}, {}, -1};

	auto start = std::chrono::high_resolution_clock::now();
	double totalDistance = 0;
	std::vector<int> stops;
	std::vector<double> distances;


	auto end = std::chrono::high_resolution_clock::now();
	return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}
