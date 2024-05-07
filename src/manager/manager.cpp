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
 * Returns the current dataset type as a formatted string.
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
 * Returns the time taken to load the current dataset.
*/
long Manager::getLoadTime() const {
	return loadtime;
}

/**
 * Returns the graph for the current dataset.
*/
Graph Manager::getNetwork() const {
	return network;
}

/**
 * Returns true if there is any dataset loaded.
*/
bool Manager::isAnyDataSetLoaded() const {
	return dType != none;
}

/**
 * Returns true if the graph is fully connected.
*/
bool Manager::isFullyConnected() const {
	return fullyConnected;
}
