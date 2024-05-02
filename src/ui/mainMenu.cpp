#include "ui.h"

/**
 * Shows the main menu.
 * It is the entry interface for the application.
*/
void UI::mainMenu()
{
	while (1)
    {
        CLEAR;
        std::cout 
        << "Welcome!\n"
		<< "Select an option to continue:\n"
        << "\n"
		<< ">> TSP Heuristics\n"
        << " [1] Backtracking\n"
		<< " [2] Triangular approximation\n"
		<< " [3] Other\n"
		<< " [4] Real World\n"
		<< "\n"
		<< ">> Currently selected dataset: " << manager.getCurrentDatasetType();
		
		if (manager.getLoadTime() != -1)
			std::cout << " (loaded in " << manager.getLoadTime() << "s)";

		std::cout
		<< "\n"
		<< "[C] Change Dataset\n"
		<< "\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
		
        std::string str;
		std::getline(std::cin, str);

		if (str.size() == 1)
		{
			switch (str[0])
			{
				case 'q':
				case 'Q':
					CLEAR;
					exit(0);
					break;
				case '1':
					break;
				case '2':
					break;
				case '3':
					break;
				case '4':
					break;
				case 'c':
				case 'C':
					selectorMenu();
					break;
				case 't':
				case 'T':
					testManager();
					break;
			}
		}
    }
}

void UI::testManager() 
{
	CLEAR;
	std::string str;
		
	for (auto m : manager.getNetwork().getVertexSet())
	{
		std::cout << m->getId() << " " << m->getInfo() << "\n";
		for (auto e : m->getAdj())
		{
			std::cout << " - " << e->getDest()->getId() 
			<< " (" << e->getWeight() << ")\n";
		}
	}

	while (std::cin.get() != '\n') { }
}
