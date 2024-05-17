#include <algorithm>
#include <unordered_map>

#include "../manager.h"

std::vector<std::vector<Vertex*>> createClusters(Graph& g, double distance) {
    Vertex* v = g.findVertex(0);

    std::vector<std::vector<Vertex*>> clusters;

    clusters.push_back({v});

    std::vector<Vertex*> nodes = g.getVertexSet();

    for (auto vertex : nodes) {
        bool fitted = false;
        if (vertex->getId() == 0) continue;

        for (auto& cluster : clusters) {
            if (vertex->getAdjMap().at(cluster[0]->getId())->getWeight() <=
                distance) {
                cluster.push_back(vertex);
                fitted = true;
                break;
            }
        }
        if (!fitted) {
            clusters.push_back({vertex});
        }
    }
    return clusters;
}

Graph PrimMST(Graph& g, Vertex* base) {
    Graph mst;
    std::priority_queue<Vertex*> queue;

    base->setDist(0);
    for (auto vtx : g.getVertexSet()) {
        mst.addVertex(vtx->getId(), vtx->getInfo());
        vtx->setPath(nullptr);
        vtx->setVisited(false);
        vtx->setDist(__DBL_MAX__);
    }

    queue.push(base);
    while (!queue.empty()) {
        Vertex* u = queue.top();
        queue.pop();
        if (u->isVisited()) continue;
        u->setVisited(true);

        for (auto e : u->getAdj()) {
            Vertex* v = e->getDest();
            if (!v->isVisited() && e->getWeight() < v->getDist()) {
                v->setPath(e);
                queue.push(v);
                v->setDist(e->getWeight());
            }
        }
    }

    for (auto vtx : g.getVertexSet()) {
        Edge* edg = vtx->getPath();
        if (edg == nullptr) continue;
        mst.addBidirectionalEdge(edg->getDest()->getId(),
                                 edg->getOrig()->getId(), edg->getWeight());
    }
    return mst;
}

void trianApproxDfs(Vertex* vtx, Vertex* last, std::vector<int>& stops,

                    std::vector<double>& distances, double* total) {
    vtx->setVisited(true);
    stops.push_back(vtx->getId());

    if (last != nullptr) {
        Edge* edg = last->getEdgeTo(vtx);
        double dist = edg == nullptr ? 0 : edg->getWeight();
        distances.push_back(dist);
        *total += dist;
    }

    for (auto edg : vtx->getAdj()) {
        Vertex* v = edg->getDest();
        if (v->isVisited()) continue;
        trianApproxDfs(v, vtx, stops, distances, total);
    }
}

double triangularCluster(Graph& graph, Vertex* base, std::vector<int>& stops,
                         std::vector<double>& distances) {
    double totalDistance = 0;
    Graph mst = PrimMST(graph, base);

    for (auto vtx : graph.getVertexSet()) vtx->setVisited(false);

    Vertex* mstBase = mst.findVertex(base->getId());
    trianApproxDfs(mstBase, nullptr, stops, distances, &totalDistance);
    return totalDistance;
}

ReturnDataTSP Manager::otherHeuristic() {
    auto start = std::chrono::high_resolution_clock::now();

    // Calculate which distance to use
    double totalWeight = 0;
    size_t numberVertex = network.getVertexSet().size();

    for (size_t i = 0; i < numberVertex - 1; i++) {
        for (size_t j = i + 1; j < numberVertex; j++) {
            Edge* edge =
                network.getVertexSet()[i]->getEdgeTo(network.getVertexSet()[j]);
            totalWeight += edge->getWeight();
        }
    }
    double distance = totalWeight / numberVertex;

    std::vector<std::vector<Vertex*>> clusters =
        createClusters(network, distance);

    double totalDistance = 0;
    // save results related to each cluster saved with the anchor of each
    // cluster
    std::unordered_map<int, std::vector<double>> clusterDistances;
    std::unordered_map<int, std::vector<int>> clusterStops;

    // Calculate connections inside clusters
    for (auto cluster : clusters) {
        Graph clusterGraph;
        std::vector<int> stops;
        std::vector<double> distances;

        for (Vertex* v : cluster) {
            clusterGraph.addVertex(v->getId(), v->getInfo());
        }

        for (size_t i = 0; i < (cluster.size() - 1); i++) {
            for (size_t j = i + 1; j < cluster.size(); j++) {
                Edge* e = cluster[i]->getEdgeTo(cluster[j]);
                clusterGraph.addBidirectionalEdge(
                    cluster[i]->getId(), cluster[j]->getId(), e->getWeight());
            }
        }
        totalDistance += triangularCluster(
            clusterGraph, clusterGraph.findVertex(cluster[0]->getId()), stops,
            distances);
        clusterDistances[cluster[0]->getId()] = distances;
        clusterStops[cluster[0]->getId()] = stops;
    }

    // Connect cluster with respect to start and end of MST
    Graph anchorGraph;

    for (auto cluster : clusters) {
        anchorGraph.addVertex(cluster[0]->getId(), cluster[0]->getInfo());
    }

    for (auto originCluster : clusters) {
        Vertex* origin = originCluster[0];
        for (auto endCluster : clusters) {
            if (originCluster == endCluster) continue;
            int end = clusterStops[endCluster[0]->getId()].back();
            anchorGraph.addEdge(
                origin->getId(), end,
                origin->getEdgeTo(network.findVertex(end))->getWeight());
        }
    }
    // Perform Triangular Aproximation to connect the graphs
    std::vector<double> connectingDistances;
    std::vector<int> connectingStops;

    totalDistance += triangularCluster(anchorGraph, network.findVertex(0),
                                       connectingStops, connectingDistances);

    std::vector<int> finalStops;
    std::vector<double> finalDistances;

    for (size_t i = 0; i < connectingStops.size(); i++) {
        finalStops.insert(finalStops.end(), clusterStops[i].begin(),
                          clusterStops[i].end());
        finalDistances.insert(finalDistances.end(), clusterDistances[i].begin(),
                              clusterDistances[i].end());
                              if (i < connectingStops.size()-1)
        finalDistances.push_back(connectingDistances[i]);
    }
    finalStops.push_back(connectingStops[0]);
    int finalCluster = connectingStops.back();
    Vertex* finalVertex = network.findVertex(clusterStops[finalCluster].back());
    finalDistances.push_back(
        network.findVertex(0)->getEdgeTo(finalVertex)->getWeight());

    auto end = std::chrono::high_resolution_clock::now();
    return {std::chrono::duration<double>(end - start).count(), finalStops,
            finalDistances, totalDistance};
}
