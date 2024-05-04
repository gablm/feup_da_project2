#include "ui.h"
#include <thread>
#include <iomanip>

/**
 * Shows a menu for dataset selection. Upon selection, the manager is called to load it.
*/
void UI::selectorMenu()
{
	while (1)
    {
        CLEAR;
        std::cout
		<< "Dataset Selection - Available datasets and their options\n"
		<< "\n"
		<< ">> ID 1 - Toy / Small sets\n"
		<< " Available options:\n"
        << " 1 - Shipping\n"
		<< " 2 - Stadiums\n"
		<< " 3 - Tourism\n"
		<< "\n"
		<< ">> ID 2 - Extra\n"
		<< " The option is the number of nodes.\n"
		<< " Available options:\n"
		<< " 25, 50, 75, 100, 200, 300,\n"
		<< " 400, 500, 600, 700, 800, 900\n"
		<< "\n"
		<< ">> ID 3 - Real World\n"
		<< " Available options:\n"
		<< " 1, 2, 3\n"
		<< "\n"
		<< ">> Currently selected dataset: " << manager.getCurrentDatasetType() << "\n"
		<< "\n"
		<< "Select a dataset using the syntax:\n"
        << "<dataset id> <dataset option>\n"
		<< "\n"
		<< "[B] Back\t[Q] Exit\n"
		<< "\n"
        << "$> ";
		
        std::string str;
		std::getline(std::cin, str);

		if (str.size() >= 4 && str.size() <= 5 && str.substr(0, 2) == "2 ")
		{
			int num = atoi(str.substr(2).c_str());
			if (num == 0) continue;
			if (num % 100 == 0 || (num < 100 && num % 25 == 0))
				return loaderScreen(extra, num);
		}

		if (str == "1 1")
			return loaderScreen(toy_shipping, -1);
		if (str == "1 2")
			return loaderScreen(toy_stadiums, -1);
		if (str == "1 3")
			return loaderScreen(toy_tourism, -1);
		if (str == "3 1")
			return loaderScreen(real_world, 1);
		if (str == "3 2")
			return loaderScreen(real_world, 2);
		if (str == "3 3")
			return loaderScreen(real_world, 3);
		
		if (str == "q" || str == "Q")
		{
			CLEAR;
			exit(0);
		}

		if (str == "b" || str == "B") return;
    }
}

/**
 * Function used in a secondary thread to show the time elapsed during load.
 * @param active Bool used to indicated if the load is still ongoing. The function returns when active is set to false.
*/
void thrLoadScreen(bool *active)
{
	auto start = std::chrono::high_resolution_clock::now();
	CLEAR;
	std::cout 
	<< "Loading Dataset...\n";
	while (*active)
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto elapsed = std::chrono::duration<double>(now - start).count();
		std::cout << "\r\rTime elapsed: " << std::fixed << std::setprecision(2) << elapsed << "s\n";
		PAUSE(100);
	}

}

/**
 * Shows a loading screen via a thread and calls the manager to load a dataset.
 * @param type Dataset type
 * @param option Required by some types to identify a single dataset.
*/
void UI::loaderScreen(DatasetType type, int option)
{
	bool active = true;
	std::thread loader = std::thread(thrLoadScreen, &active);
	manager.loadDataset(type, option);
	active = false;
	loader.join();
}
