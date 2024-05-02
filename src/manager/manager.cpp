#include "manager.h"

DatasetType Manager::getCurrentDatasetType() const {
	return dType;
}

long Manager::getLoadTime() const {
	return loadtime;
}
