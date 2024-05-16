#include <algorithm>
#include <set>

#include "../manager.h"

std::vector<std::vector<Vertex*>> createClusters(Graph g, double distance) {
    Vertex* v = g.findVertex(0);

    std::vector<std::vector<Vertex*>> clusters;
    std::vector<std::set<Edge*>> connections;

    clusters.push_back({v});

    std::vector nodes = g.getVertexSet();
    std::sort(nodes.begin(), nodes.end(), [v](Vertex* a, Vertex* b) {
        return v->getAdjMap().at(a->getId())->getWeight() <
               v->getAdjMap().at(b->getId())->getWeight();
    });

    size_t startCluster = 0;
    for (auto vertex : nodes) {
        if (v->getId() == 0) continue;

        bool fitted = false;
        for (size_t i = clusters.size(); i > 0; i--) {
            std::vector cluster = clusters[i-1];

            for (auto vertexCluster : cluster) {
                if (vertex->getAdjMap()
                        .at(vertexCluster->getId())
                        ->getWeight() <= distance) {
                    cluster.push_back(vertex);
                    fitted = true;
                    break;
                }
            }
            if (fitted)
                break;
        }
        if (!fitted) {
            clusters.push_back({vertex});
        }
    }


}

ReturnDataTSP Manager::otherHeuristic() {
    auto start = std::chrono::high_resolution_clock::now();
    double totalDistance = 0;
    std::vector<int> stops;
    std::vector<double> distances;

    std::vector<std::vector<Vertex*>> clusters = createClusters(network, totalDistance);
	std::vector<int> vertices;
	for (auto cluster : clusters){
		vertices.push_back(cluster[0]->getId());
	}
	// Calculate connections inside clusters
	for (auto cluster : clusters){
		// MST(cluster);
	}

	// Perform triangularAproximation with only the start and end nodes of the clusters
	// triangularAproximation(network, clusters);

	// TODO: Code to connect end nodes of some clusters to the start of others 
	

    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double>(end - start).count(), stops,
            distances, totalDistance};
}
