#ifndef MANAGER_H
#define MANAGER_H

#include "../graph/graph.h"

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
		DatasetType dType = DatasetType::none;
		long loadtime = 0;

		Graph network;
	public:
		DatasetType getCurrentDatasetType() const;
		long getLoadTime() const;

		void loadDataset(DatasetType type, int option = -1);
};

#endif