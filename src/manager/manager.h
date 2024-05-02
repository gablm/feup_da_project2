#ifndef MANAGER_H
#define MANAGER_H

#include "../graph/graph.h"
#include <string>

#ifdef __linux__
# define CLEAR (void)system("clear")
#else
# define CLEAR (void)system("cls")
#endif

enum DatasetType {
	none,
	toy_shipping,
	toy_stadiums,
	toy_tourism,
	real_world,
	extra
};

class Manager {
	private:
		DatasetType dType = none;
		int option = -1;
		long loadtime = -1;

		Graph network;
	public:
		std::string getCurrentDatasetType() const;
		long getLoadTime() const;

		void loadDataset(DatasetType type, int option = -1);
		void loadToy(int option);
};

#endif