#include "ui.h"
#include <cmath>

void UI::resultMenu(int type)
{
	// TODO: CALL FUNCTION FROM MANAGER TO RETURN A ReturnDataTSP
	std::vector<int> stops = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
	std::vector<double> distances = {200, 300, 100, 20, 1, 1, 1, 1, 1, 1, 1, 11, 1, 1, 1, 1, 1};
	ReturnDataTSP ret = {0, stops, distances, 0};

	size_t count = 0;
	while (1)
	{
		CLEAR;
		std::cout 
		<< "Results for heuristic " << type << "\n"
		<< "\n";

		size_t i = count;
		if (i != 0) std::cout << "("<< i << " ENTRIES ABOVE)\n...\n↓\n";

		for (; i < std::min(count + 5, ret.stops.size() - 1); i++)
		{
			std::cout
			<< stops[i] << "\n"
			<< "↓ " << distances[i] << "km\n";
		}
		std::cout << stops[i] << "\n";
		if (i != ret.stops.size() - 1) std::cout << "↓\n...\n("<< ret.stops.size() - i << " ENTRIES BELOW)\n";

		std::cout
		<< "\n" 
		<< "Total Stops: " << ret.stops.size() - 2 << " (" << ret.stops.size() << " stops including the initial/final point)\n"
		<< "Total Distance Travelled: " << ret.totalDistance << "km\n"
		<< "Time taken to run the heuristic: " << ret.processingTime << "s\n"
		<< "\n"
		<< "[S] - Push list down\t[W] - Push list up\n"
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
        << "$> ";

		std::string str;
		std::getline(std::cin, str);

		if (str == "b" || str == "B") return;

		if (str == "q" || str == "Q")
		{
			CLEAR;
			exit(0);
		}

		if (str == "s" || str == "S")
		{
			count = count + 5 < ret.stops.size() - 1 ? count + 5 : count;
			continue;
		}

		if (str == "w" || str == "W")
		{
			count = count < 5 ? 0 : count - 5;
			continue;
		}
	}
	
}
