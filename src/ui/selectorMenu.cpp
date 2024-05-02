#include "ui.h"

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
				return manager.loadDataset(extra, num);
		}

		if (str == "1 1")
			return manager.loadDataset(toy_shipping, -1);
		if (str == "1 2")
			return manager.loadDataset(toy_stadiums, -1);
		if (str == "1 3")
			return manager.loadDataset(toy_tourism, -1);
		if (str == "3 1")
			return manager.loadDataset(real_world, 1);
		if (str == "3 2")
			return manager.loadDataset(real_world, 2);
		if (str == "3 3")
			return manager.loadDataset(real_world, 3);
		
		if (str == "q" || str == "Q")
		{
			CLEAR;
			exit(0);
		}

		if (str == "b" || str == "B") return;
    }
}