#include "manager.h"

/**
 * Calls the appropriate heuristic from within the manager.
 * @return Structure with time taken, stops, distances between stops 
 * and total distance travelled.
 * @param type Type of heuristic
 * @param base Only needed for 4th heuristic, defines the base vertex.
*/
ReturnDataTSP Manager::tspCaller(HeuristicType type, Vertex *base = nullptr)
{
	switch(type)
	{
		case backtracking_2_1:
			return backtrackingHeuristic();
		case triangular_approximation_2_2:
			return triangularApproximationHeuristic();
		case other_2_3:
			return otherHeuristic();
		case real_world_2_4:
			return realWorldHeuristic(base);
	}
	
	return {-1, {}, {}, -1};
}
