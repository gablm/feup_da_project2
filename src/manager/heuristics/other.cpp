#include "../manager.h"

/**
 * Divides the initial graph into clusters 
 * where each node is at maximum (distance) km of a central node.
 * @param g Base graph.
 * @param distance Maximum distance between nodes in a cluster.
 * @param clusters Vector to put the clusters.
 * @note Complexity: O(V + C) where C is the number of clusters.
*/
void Manager::createClusters(Graph& g, double distance,
    std::vector<std::vector<Vertex*>>& clusters)
{
    Vertex* v = g.findVertex(0);

    clusters.push_back({v});

    std::vector<Vertex*> nodes = g.getVertexSet();

    for (auto& vertex : nodes)
	{
        bool fitted = false;
        if (vertex->getId() == 0) continue;

        for (auto& cluster : clusters)
		{
            if (vertex->getEdgeTo(cluster.front())->getWeight() <= distance)
			{
                cluster.push_back(vertex);
                fitted = true;
                break;
            }
        }
        if (!fitted) clusters.push_back({vertex});
    }
}

/**
 * Runs the triangular approximation algorithm for each cluster.
 * @param graph Cluster graph.
 * @param vertex Base vertex.
 * @param stops Vector where the stops are saved.
 * @param distances Vector with the distances between stops.
 * @return Total distance travelled inside the cluster.
 * @note Complexity: O(V + E)
*/
double Manager::triangularCluster(Graph& graph, Vertex* base,
    std::vector<int>& stops, std::vector<double>& distances)
{
    double totalDistance = 0;
    Graph mst = PrimMST(graph, base);

    for (Vertex* vtx : graph.getVertexSet()) vtx->setVisited(false);

    Vertex* mstBase = mst.findVertex(base->getId());
    trianApproxDfs(mstBase, nullptr, stops, distances, &totalDistance);
    return totalDistance;
}

/**
 * This heuristic uses triangular approximation, 
 * but first divides the network into smaller closer clusters. (Divide&Conqueer)
 * @return Structure with the time taken, stops, distances between stops and max distance travelled.
 * @note Complexity: O(V + E)
*/
ReturnDataTSP Manager::otherHeuristic()
{
    // Calculate which distance to use
    double totalWeight = 0;
    size_t numberVertex = network.getVertexSet().size();
    int count = 0;

    auto testStart = std::chrono::high_resolution_clock::now();
    auto vertexSet = network.getVertexSet();

    // Calculate average distance
    for (size_t i = 0; i < numberVertex - 1; i++)
	{
        for (size_t j = i + 1; j < numberVertex; j++)
		{
            count++;
            Edge* edge = vertexSet[i]->getEdgeTo(vertexSet[j]);
            totalWeight += edge->getWeight();
        }
    }

    auto testEnd = std::chrono::high_resolution_clock::now();
    auto testFinal = std::chrono::duration<double>(testEnd - testStart).count();
    std::cout << testFinal;
    double distance = totalWeight / (count) * 0.2;

    auto start = std::chrono::high_resolution_clock::now();

    testStart = std::chrono::high_resolution_clock::now();
    // Create the clusters
    std::vector<std::vector<Vertex*>> clusters;
    createClusters(network, distance, clusters);

    testEnd = std::chrono::high_resolution_clock::now();
    testFinal = std::chrono::duration<double>(testEnd - testStart).count();

    double totalDistance = 0;

    // save results related to each cluster saved with the anchor of each
    // cluster
    std::unordered_map<int, std::vector<double>> clusterDistances;
    std::unordered_map<int, std::vector<int>> clusterStops;

    testStart = std::chrono::high_resolution_clock::now();
    // Calculate connections inside clusters
    for (auto& cluster : clusters)
	{
        Graph clusterGraph;
        std::vector<int> stops;
        std::vector<double> distances;

        // Populate the graph
        for (Vertex* & v : cluster)
		{
            clusterGraph.addVertex(v->getId(), v->getInfo());
        }

        for (auto it = cluster.begin(); std::next(it) != cluster.end(); ++it)
		{
            for (auto jt = std::next(it); jt != cluster.end(); ++jt)
			{
                Edge* e = (*it)->getEdgeTo(*jt);
                clusterGraph.addBidirectionalEdge(
                    (*it)->getId(), (*jt)->getId(), e->getWeight());
            }
        }

        // Perform the algorithm
        totalDistance += triangularCluster(
            clusterGraph, 
			clusterGraph.findVertex(cluster.front()->getId()),
            stops,
			distances
		);

        clusterDistances[cluster.front()->getId()] = distances;
        clusterStops[cluster.front()->getId()] = stops;
    }

    testEnd = std::chrono::high_resolution_clock::now();
    testFinal = std::chrono::duration<double>(testEnd - testStart).count();
    // Connect cluster with respect to start and end of MST
    Graph anchorGraph;

    testStart = std::chrono::high_resolution_clock::now();
    // Populate the graph
    for (auto cluster : clusters)
	{
        anchorGraph.addVertex(cluster.front()->getId(), cluster.front()->getInfo());
    }

    for (auto originCluster : clusters)
	{
        Vertex* origin = originCluster.front();
        for (auto endCluster : clusters)
		{
            if (originCluster == endCluster) continue;
            int end = clusterStops[endCluster.front()->getId()].back();
            anchorGraph.addEdge(
                origin->getId(), end,
                origin->getEdgeTo(network.findVertex(end))->getWeight());
        }
    }

    testEnd = std::chrono::high_resolution_clock::now();
    testFinal = std::chrono::duration<double>(testEnd - testStart).count();
    // Perform Triangular Aproximation to connect the clusters
    std::vector<double> connectingDistances;
    std::vector<int> connectingStops;

    testStart = std::chrono::high_resolution_clock::now();
    totalDistance += triangularCluster(
		anchorGraph, 
		anchorGraph.findVertex(0),
        connectingStops,
		connectingDistances
	);

    testEnd = std::chrono::high_resolution_clock::now();
    testFinal = std::chrono::duration<double>(testEnd - testStart).count();
    std::vector<int> finalStops;
    std::vector<double> finalDistances;

    testStart = std::chrono::high_resolution_clock::now();
    // Iterate through the result to join paths
    auto jt = connectingDistances.begin();
    for (auto it = connectingStops.begin(); it != connectingStops.end(); it++)
	{
        finalStops.insert(finalStops.end(), clusterStops[*it].begin(),
                          clusterStops[*it].end());
        finalDistances.insert(finalDistances.end(),
                              clusterDistances[*it].begin(),
                              clusterDistances[*it].end());
        if (jt != connectingDistances.end())
		{
            finalDistances.push_back(*jt);
            jt++;
        }
    }

    finalStops.push_back(connectingStops.front());
    int finalCluster = connectingStops.back();
    Vertex* finalVertex = network.findVertex(clusterStops[finalCluster].back());
    Edge* finalEdge = network.findVertex(0)->getEdgeTo(finalVertex);
    double finalWeight = finalEdge != nullptr ? finalEdge->getWeight() : 0;
    finalDistances.push_back(finalWeight);
    totalDistance += finalWeight;

    testEnd = std::chrono::high_resolution_clock::now();
    testFinal = std::chrono::duration<double>(testEnd - testStart).count();
    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double>(end - start).count(), finalStops, finalDistances, totalDistance};
}
