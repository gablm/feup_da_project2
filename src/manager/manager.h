#ifndef MANAGER_H
#define MANAGER_H

#include "../graph/graph.h"
#include <string>

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
		long loadtime = 0;

		Graph network;
	public:
		std::string getCurrentDatasetType() const;
		long getLoadTime() const;

		void loadDataset(DatasetType type, int option = -1);
};

#endif