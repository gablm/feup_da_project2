#include "manager.h"

/**
 * Default destructor.
 * Deletes the allocated memory in the graph.
 * @note Complexity: O(V + E) 
*/
Manager::~Manager() {
	network.resetGraph();
}

/**
 * @return The current dataset type as a formatted string.
*/
std::string Manager::getCurrentDatasetType() const
{
	std::ostringstream out;
	switch (dType) {
		case none:
			return "None";
		case toy_shipping:
			return "Toy - Shipping";
		case toy_stadiums:
			return "Toy - Stadiums";
		case toy_tourism:
			return "Toy - Tourism";
		case real_world:
			out << "Real World - Graph " << option; 
			return out.str();
		case extra:
			out << "Extra - " << option << " Nodes"; 
			return out.str();
		default:
			return "None";
	};
}

/**
 * @return Time taken to load the current dataset.
*/
long Manager::getLoadTime() const {
	return loadtime;
}

/**
 * @return The graph for the current dataset.
*/
Graph Manager::getNetwork() const {
	return network;
}

/**
 * @return True if there is any dataset loaded.
*/
bool Manager::isAnyDataSetLoaded() const {
	return dType != none;
}

/**
 * @return True if the graph is fully connected.
*/
bool Manager::isFullyConnected() const {
	return fullyConnected;
}

/**
 * Converts a Heuristic type enum to a string with its name.
 * @param type Heuristic type
 * @return Heuristic name as a formatted string
*/
std::string Manager::getHeuristicAsString(HeuristicType type)
{
	switch (type)
	{
		case backtracking_2_1:
			return "Backtracking (T2.1)";
		case triangular_approximation_2_2:
			return "Triangular Approximation (T2.2)";
		case other_2_3:
			return "Other (T2.3)";
		case real_world_2_4:
			return "Real World (T2.4)";
	}
	return "Unknown";
}
