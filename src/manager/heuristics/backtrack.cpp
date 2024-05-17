#include "../manager.h"
#include <limits.h>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <limits>
#include <list>





void backtrack(Graph* g, Vertex* v, std::vector<int>& currentPath, double& currentDistance, double* bestDistance, std::vector<int>* bestPath) {
    if (currentDistance > *bestDistance || (g->findVertex(0) == v && currentDistance != 0 && (int) currentPath.size() != g->getNumVertex())) {
        return;
    }

    currentPath.push_back(v->getId());

    for (auto edge : v->getAdj()) {
        Vertex* child = edge->getDest();
        if (!child->isVisited()) {
            child->setVisited(true);
            currentDistance += edge->getWeight();
            backtrack(g, child, currentPath, currentDistance, bestDistance, bestPath);
            currentDistance -= edge->getWeight();
            child->setVisited(false);
        }
    }

    if ((int)currentPath.size() == g->getNumVertex() + 1 && *bestDistance > currentDistance) {
        *bestDistance = currentDistance;
        *bestPath = currentPath;
    }
    currentPath.pop_back();
}


// Define your Graph and Vertex classes and other necessary declarations...

ReturnDataTSP Manager::backtrackingHeuristic() {
    auto start = std::chrono::high_resolution_clock::now();
    double totalDistance = std::numeric_limits<double>::max();
    std::vector<int> stops;
    std::vector<double> distances;
    std::vector<int> path;
    double current = 0;
    
    Vertex* initial = network.findVertex(0);

    for (auto v : network.getVertexSet()) {
        v->setVisited(false);
    }

    backtrack(&network, initial, path, current, &totalDistance, &stops);


for(int i = 0; i < (int)(stops.size())-1;i++ ){
		for(auto edge : network.findVertex(stops[i])->getAdj()){
			if(edge->getDest() == network.findVertex(stops[i+1])){
				distances.push_back(edge->getWeight());
			}
		
		}
	}
	auto end = std::chrono::high_resolution_clock::now();


    return {std::chrono::duration<double>(end - start).count(), stops, distances, totalDistance};
}