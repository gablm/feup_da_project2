#include "manager.h"

ReturnDataTSP Manager::tspCaller(HeuristicType type)
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
			return realWorldHeuristic();
	}
	
	return {-1, {}, {}, -1};
}
