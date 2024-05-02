#include "manager.h"
#include <sstream>

std::string Manager::getCurrentDatasetType() const {
	std::ostringstream out;
	switch (dType) {
		case none:
			return "None";
		case toy_shipping:
			return "Tou - Shipping";
		case toy_stadiums:
			return "Toy - Stadiums";
		case toy_tourism:
			return "Toy - Tourism";
		case real_world:
			out << "Real World - " << option; 
			return out.str();
		default:
			return "None";
	};
}

long Manager::getLoadTime() const {
	return loadtime;
}
