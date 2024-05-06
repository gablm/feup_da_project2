#include "manager.h"
#include "../ui/ui.h"

/**
 * Loads a dataset of type Toy
 * @param option Which toy dataset [1 = shipping, 2 = stadiums, 3 = tourism]
 * @note Complexity: O(E)
*/
void Manager::loadToy(unsigned option)
{
	std::string fileName;
	switch (option)
	{
		case 1:
			fileName = "../datasets/toy/shipping.csv";
			break;
		case 2:
			fileName = "../datasets/toy/stadiums.csv";
			break;
		case 3:
			fileName = "../datasets/toy/tourism.csv";
			break;
	}
	std::ifstream file = std::ifstream(fileName);

    if(!file.is_open())
	{
		CLEAR;
        std::cout << "Error while opening file: " << fileName << std::endl;
		exit(0);
    }
	else
	{
        std::string line;
        std::getline(file, line);

        while (getline(file, line))
		{
            std::string origin, dest, distance, label1 = "", label2 = "";
            std::stringstream ss(line);

            std::getline(ss, origin, ',');
            std::getline(ss, dest, ',');
            std::getline(ss, distance, option == 3 ? ',' : '\r');
            if (option == 3) 
			{
				std::getline(ss, label1, ',');
            	std::getline(ss, label2, '\r');
			}
			
			int o = std::stoi(origin);
			int d = std::stoi(dest);
			double w = std::stod(distance);
			network.addVertex(o, Info(label1));
			network.addVertex(d, Info(label2));
            network.addBidirectionalEdge(o, d, w);
        }
    }
}

/**
 * Loads a dataset of type extra.
 * @note Complexity: O(V + E)
 * @param option Amount of edges
*/
void Manager::loadExtra(unsigned option)
{
	std::string nodes = "../datasets/extra_fully_connected/nodes.csv";
	std::ostringstream edges;
	edges << "../datasets/extra_fully_connected/edges_" << option << ".csv";
	loadBig(nodes, edges.str(), true, true, option);
}

/**
 * Loads a dataset of type real_world
 * @note Complexity: O(V + E)
 * @param option Graph number [1-3]
*/
void Manager::loadRealWorld(unsigned option)
{
	std::ostringstream path;
	path << "../datasets/real_world/graph" << option << "/";
	std::string nodes = path.str() + "nodes.csv";
	std::string edges = path.str() + "edges.csv";
	loadBig(nodes, edges, false, false, -1);
}

/**
 * Reads and loads a graph based in two files containing edges and nodes.
 * @note Complexity: O(V + E)
 * @param nodes Path to the Nodes file.
 * @param edges Path to the Edges file.
 * @param skipFirstRow If false, the first line of the edges file won't be skipped.
 * @param assumeBidirectional If true, each edge is assumed as bidirectional.
 * @param rowCount Amount of nodes to read from nodes file. -1 means all.
*/
void Manager::loadBig(std::string nodes, std::string edges, bool skipFirstRow, 
	bool assumeBidirectional, long rowCount = -1)
{
	std::ifstream file = std::ifstream(nodes);

    if(!file.is_open())
	{
		CLEAR;
        std::cout << "Error while opening file: " << nodes << std::endl;
		exit(0);
    }
	else
	{
        std::string line;
        std::getline(file, line);

		int count = 0;
        while (getline(file, line) && (count < rowCount || rowCount == -1))
		{
            std::string origin, longi, lat;
            std::stringstream ss(line);

            std::getline(ss, origin, ',');
            std::getline(ss, longi, ',');
            std::getline(ss, lat, '\r');
			
			int o = std::stoi(origin);
			double lo = std::stod(longi);
			double la = std::stod(lat);
			network.addVertex(o, Info(lo, la));
			count++;
        }
    }

	file = std::ifstream(edges);

    if(!file.is_open())
	{
		CLEAR;
        std::cout << "Error while opening file: " << edges << std::endl;
		exit(0);
    }
	else
	{
        std::string line;
        if (!skipFirstRow) std::getline(file, line);

        while (getline(file, line))
		{
            std::string sourc, dest, dist;
            std::stringstream ss(line);

            std::getline(ss, sourc, ',');
            std::getline(ss, dest, ',');
            std::getline(ss, dist, '\r');
			
			int s = std::stoi(sourc);
			int d = std::stod(dest);
			double w = std::stod(dist);
			if (assumeBidirectional)
				network.addBidirectionalEdge(s, d, w);
			else
				network.addEdge(s, d, w);
        }
    }
}

/**
 * Calls the appropriate loader function given the dataset.
 * Also calculates the time the system took to load it.
 * @note Complexity: O(V + E)
 * @param type Type of the dataset to load
 * @param option defaults to -1. It is required to select a specific dataset of types RealWorld and Extra
*/
void Manager::loadDataset(DatasetType type, int option)
{
	auto start = std::chrono::high_resolution_clock::now();
	network.resetGraph();
	switch (type) {
		case none:
			return;
		case toy_shipping:
			loadToy(1);
			break;
		case toy_stadiums:
			loadToy(2);
			break;
		case toy_tourism:
			loadToy(3);
			break;
		case real_world:
			if (option < 0)
			{
				UI::showMessage("INVALID OPTION", "This should not have happened.\nPlease try again!");
				return;
			}
			loadRealWorld(option);
			break;
		case extra:
			if (option < 0)
			{
				UI::showMessage("INVALID OPTION", "This should not have happened.\nPlease try again!");
				return;
			}
			loadExtra(option);
			break;
	};
	auto end = std::chrono::high_resolution_clock::now();
	loadtime = std::chrono::duration<double>(end - start).count();
	this->dType = type;
	this->option = option;
}
