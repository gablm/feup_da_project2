#include "manager.h"

void Manager::loadDataset(DatasetType type, int option) {
	this->dType = type;
	this->option = option;
	switch (dType) {
		case none:
			return;
		case toy_shipping:
			break;
		case toy_stadiums:
			break;
		case toy_tourism:
			break;
		case real_world:
			break;
		case extra:
			break;
		default:
			return;
	};
}
