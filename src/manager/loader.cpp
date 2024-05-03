#include "manager.h"
#include "../ui/ui.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

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
            network.addEdge(o, d, w);
        }
    }
}

void Manager::loadExtra(unsigned option)
{
	std::string nodes = "../datasets/extra_fully_connected/nodes.csv";
	std::ostringstream edges;
	edges << "../datasets/extra_fully_connected/edges_" << option << ".csv";
	loadBig(nodes, edges.str(), true, option);
}

void Manager::loadRealWorld(unsigned option)
{
	std::ostringstream path;
	path << "../datasets/real_world/graph" << option << "/";
	std::string nodes = path.str() + "nodes.csv";
	std::string edges = path.str() + "edges.csv";
	loadBig(nodes, edges, false, -1);
}

void Manager::loadBig(std::string nodes, std::string edges, bool skipFirstRow, long rowCount = -1)
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
			network.addEdge(s, d, w);
        }
    }
}

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
