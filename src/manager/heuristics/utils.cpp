#include "../manager.h"
#include <cmath>

#define PI 3.14159265
#define EARTH_RADIUS 6371000

/**
 * Overload for the haversineDistance function taking two graph Info elements.
 * Uses Haversine formula to calculate the distance between two coordinates.
 * @return Distance between two coordinates, or -1 if coordinates are invalid.
 * @param pos1 Information about 1st vertex
 * @param pos2 Information about 2nd vertex 
*/
double Manager::haversineDistance(Info pos1, Info pos2)
{
	if (pos1.lat == -1 || pos2.lat == -1 
		|| pos1.lon == -1 || pos2.lon == -1) return -1;

	return haversineDistance(pos1.lat, pos1.lon, pos2.lat, pos2.lon);
}

/**
 * Converts a number in degrees to radians.
 * @param num Number to convert.
*/
double Manager::to_radians(double num)
{
	return num * PI / 180.0;
}

/**
 * Calculates the distance between two coordinates using the Haversine formula.
 * @param lat1 Latitude of 1st point
 * @param lon1 Longitude of 1st point
 * @param lat2 Latitude of 2nd point
 * @param lon2 Longitude of 2nd point
 * @note Complexity: O(log n) due to sqrt()
*/
double Manager::haversineDistance(double lat1, double lon1, 
	double lat2, double lon2)
{
	double lat1_r = to_radians(lat1);
	double lon1_r = to_radians(lon1);
	double lat2_r = to_radians(lat2);
	double lon2_r = to_radians(lon2);

	double lat_d = lat2_r - lat1_r;
	double lon_d = lon2_r - lon1_r;

	double aux = pow(sin(lat_d / 2), 2) + 
		cos(lat1_r) * cos(lat2_r) * pow(sin(lon_d / 2), 2);

	return EARTH_RADIUS * 2.0 * atan2(sqrt(aux), sqrt(1.0 - aux));
}

/**
 * Checks if a graph is fully connected 
 * by checking if all vertexes have (count of vertexes - 1) edges.~
 * @note Complexity: O(n)
 * @return True if connected
*/
bool Manager::isGraphFullyConnected()
{
	size_t vertexCount = network.getNumVertex();
	for (auto vtx : network.getVertexSet())
	{
		if (vtx->getAdj().size() != vertexCount - 1) return false;
	}
	return true;
}

/**
 * Shows a screen with information about the graph connection process.
 * Intented to be run in a separated thread.
 * @param active Pointer to boolean indicating if the process is active.
 * @param vtxnum Pointer to the number of vertexes already processed.
 * @param total Total number of vertexes.
*/
void thrLoadScreen2(bool *active, long *vtxnumb, long total)
{
	auto start = std::chrono::high_resolution_clock::now();
	CLEAR;
	std::cout 
	<< "Fully connecting graph...\n";
	do
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(now - start).count();
		std::cout << RESET << *vtxnumb << " of " << total 
			<< " | " << std::fixed << std::setprecision(1) << elapsed << "s";
		PAUSE(100);
	} while (*active);
}

/**
 * Fully connects graph.
 * @note Complexity: O(n ^ 2)
*/
void Manager::fullyConnectGraph()
{
	bool active = true;
	long vtxnumb = 0;
	size_t vertexCount = network.getNumVertex();
	std::thread loader = std::thread(thrLoadScreen2, 
		&active, &vtxnumb, vertexCount);

	for (auto vtx : network.getVertexSet())
	{
		vtxnumb++;
		if (vtx->getAdj().size() == vertexCount - 1) continue;
		for (auto vtx2 : network.getVertexSet())
		{
			if (vtx == vtx2) continue;
			if (vtx->getEdgeTo(vtx2) != nullptr) continue;
		
			double dist = Manager::haversineDistance(vtx->getInfo(), vtx2->getInfo());
			vtx->addEdge(vtx2, dist);
		}
	}

	active = false;
	loader.join();

	std::cout << "\nVerifying success...\n";
	fullyConnected = isGraphFullyConnected();
}
