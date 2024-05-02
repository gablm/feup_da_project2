#include "manager.h"
#include <fstream>
#include <sstream>
#include <chrono>

void Manager::loadToy(int option)
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
	std::ifstream file(fileName);

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
            std::getline(ss, distance, option == 2 ? ',' : '\r');
            if (option == 2) 
			{
				std::getline(ss, label1, ',');
            	std::getline(ss, label2, '\r');
			}
			
			int o = atoi(origin.c_str());
			int d = atoi(dest.c_str());
			int w = atoi(distance.c_str());
			network.addVertex(o, label1);
			network.addVertex(d, label2);
            network.addEdge(o, d, w);
        }
    }
}

void Manager::loadDataset(DatasetType type, int option)
{
	auto start = std::chrono::high_resolution_clock::now();
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
			break;
		case extra:
			break;
	};
	auto end = std::chrono::high_resolution_clock::now();
	loadtime = std::chrono::duration<double>(end - start).count();
	this->dType = type;
	this->option = option;
}
