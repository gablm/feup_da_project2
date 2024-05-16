#ifndef MANAGER_H
#define MANAGER_H

#include "../graph/graph.h"
#include <string>
#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>
#include <iomanip>
#include <set>

#ifdef __linux__
# include <unistd.h>
# ifdef DEBUG
#  define CLEAR std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
# else
#  define CLEAR (void)system("clear")
# endif
# define PAUSE(x) usleep(x * 1000)
# define RESET "\33[2K\r"
#else
# include <windows.h>
# ifdef DEBUG
#  define CLEAR std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
# else
# define CLEAR (void)system("cls")
# endif
# define PAUSE(x) Sleep(x);
# define RESET "\r"
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
	double totalDistance;
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
		bool fullyConnected = true;

		// Loaders

		void loadToy(unsigned option);
		void loadExtra(unsigned option);
		void loadRealWorld(unsigned option);

		void loadBig(std::string nodes, std::string edges, bool skipFirstRow, 
			bool assumeBidirectional, long rowCount);

		// Heuristics

		ReturnDataTSP backtrackingHeuristic();
		
		ReturnDataTSP triangularApproximationHeuristic();
		Graph PrimMST(Vertex *base);
		void trianApproxDfs(Vertex *vtx, Vertex *last, std::vector<int> &stops, 
			std::vector<double> &distances, double *total);

		ReturnDataTSP otherHeuristic();
		ReturnDataTSP realWorldHeuristic();

	public:
		~Manager();

		std::string getCurrentDatasetType() const;
		bool isAnyDataSetLoaded() const;
		bool isFullyConnected() const;
		long getLoadTime() const;
		Graph getNetwork() const;

		void loadDataset(DatasetType type, int option = -1);
		ReturnDataTSP tspCaller(HeuristicType type);

		// HeuristicUtils

		static double haversineDistance(Info pos1, Info pos2);
		static double haversineDistance(double lat1, double lon1,
			double lat2, double lon2);
		static double to_radians(double num);
		bool isGraphFullyConnected();
		void fullyConnectGraph();
};

#endif