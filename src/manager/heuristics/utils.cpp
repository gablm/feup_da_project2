#include "../manager.h"
#include <cmath>

#define PI 3.14159265
#define EARTH_RADIUS 6371000

double Manager::haversineDistance(Info pos1, Info pos2)
{
	return haversineDistance(pos1.lat, pos1.lon, pos2.lat, pos2.lon);
}

double to_radians(double num)
{
	return num * PI / 180.0;
}

double Manager::haversineDistance(double lat1, double lon1, 
	double lat2, double lon2)
{
	double lat1_r = to_radians(lat1);
	double lon1_r = to_radians(lon1);
	double lat2_r = to_radians(lat2);
	double lon2_r = to_radians(lon2);

	double lat_d = lat2_r - lat1_r;
	double lon_d = lon2_r - lon2_r;

	double aux = pow(sin(lat_d / 2), 2) + 
		cos(lat1_r) * cos(lat2_r) * pow(sin(lon_d / 2), 2);

	return EARTH_RADIUS * 2.0 * atan2(sqrt(aux), sqrt(1.0 - aux));
}

bool Manager::isGraphFullyConnected(Graph &graph)
{
	long vertexCount = graph.getNumVertex();
	for (auto vtx : graph.getVertexSet())
	{
		if (vtx->getAdj().size() != vertexCount - 1) return false;
	}
	return true;
}
