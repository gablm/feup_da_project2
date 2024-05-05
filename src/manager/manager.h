#ifndef MANAGER_H
#define MANAGER_H

#include "../graph/graph.h"
#include <string>

#ifdef __linux__
# include <unistd.h>
# define CLEAR (void)system("clear")
# define PAUSE(x) usleep(x * 1000)
#else
# include <windows.h>
# define CLEAR (void)system("cls")
# define PAUSE(x) Sleep(x);
#endif

/**
 * Dataset types
*/
enum DatasetType {
	none,
	toy_shipping,
	toy_stadiums,
	toy_tourism,
	real_world,
	extra
};

/**
 * Identifies the heuristic for the 
 * Travelling Salesman Problem "solution".
*/
enum HeuristicType {
	backtracking_2_1,
	triangular_approximation_2_2,
	other_2_3,
	real_world_2_4
};

/**
 * Represents the return struct for the TSP heuristics.
*/
struct ReturnDataTSP {
	/** Time it took to generate final path. */
	double processingTime;
	/** Stops by order of the path. */
	std::vector<int> stops;
	/** 
	 * Distances between stops in order. 
	 * Its lenght should be less one than the stop's lenght.
	*/
	std::vector<double> distances;
	/** Total distance of the path. */
	long totalDistance;
};

/**
 * Contains the methods to load the datasets into a graph 
 * and operate over it in order to (try to) generate 
 * the best path passing in all its vertexes (TSP) 
*/
class Manager {
	private:
		DatasetType dType = none;
		int option = -1;
		long loadtime = -1;

		Graph network;
	public:
		~Manager();

		std::string getCurrentDatasetType() const;
		bool isAnyDataSetLoaded() const;
		long getLoadTime() const;
		Graph getNetwork() const;

		// Loaders

		void loadDataset(DatasetType type, int option = -1);
		void loadToy(unsigned option);
		void loadExtra(unsigned option);
		void loadRealWorld(unsigned option);

		void loadBig(std::string nodes, std::string edges, bool skipFirstRow, 
			bool assumeBidirectional, long rowCount);

		// Heuristics

		ReturnDataTSP tspCaller(HeuristicType type);

		ReturnDataTSP backtrackingHeuristic();
		ReturnDataTSP triangularApproximationHeuristic();
		ReturnDataTSP otherHeuristic();
		ReturnDataTSP realWorldHeuristic();
};

#endif