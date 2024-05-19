#include "ui.h"

/**
 * Shows the main menu.
 * It is the entry interface for the application.
*/
void UI::mainMenu()
{
	while (1)
    {
		bool notFullyConnected = !manager.isFullyConnected() && manager.isAnyDataSetLoaded();

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
		
		if (notFullyConnected)
		{
			std::cout << "\nWARNING: The graph is not fully connected.\n";
		}
		

		std::cout
		<< "\n"
		<< "[C] Change Dataset\n"
		<< (notFullyConnected ? "[F] Fully connect graph\n" : "")
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
				case '2':
				case '3':
				case '4':
					if (manager.isAnyDataSetLoaded())
						resultMenu(static_cast<HeuristicType>(str[0] - '0' - 1));
					else
						showMessage(
							"NO DATASET WAS SELECTED",
							"Select a dataset in the main menu with the 'c' command."
						);
					break;
				case 'c':
				case 'C':
					selectorMenu();
					break;
				case 't':
				case 'T':
					testManager();
					break;
				case 'f':
				case 'F':
					manager.fullyConnectGraph();
					break;
			}
		}
    }
}

/**
 * Hidden menu.
 * Outputs the information stored in a graph for testing purposes.
 * @note Complexity: O(V + E)
*/
void UI::testManager() 
{
	CLEAR;
	std::string str;
	
	Graph network = manager.getNetwork();
	for (auto m : network.getVertexSet())
	{
		std::cout << m->getId() << " " << m->getInfo().toStr() 
			<< " (" << m->getAdj().size() << " edges)\n";
		for (auto e : m->getAdj())
		{
			std::cout << " - " << e->getDest()->getId() 
			<< " (" << e->getWeight() << ")\n";
		}
	}

	while (std::cin.get() != '\n') { }
}

/**
 * Clears the screen and shows a message to the user.
 * After waits for the user to press enter.
 * @param header Message header. Separated from the body by an empty line.
 * @param body Message body. Separated below by a empty line from a "CLICK ENTER TO CONTINUE..." message.
*/
void UI::showMessage(std::string header, std::string body)
{
	CLEAR;
	std::cout 
	<< header << "\n"
	<< "\n"
	<< body << "\n"
	<< "\n"
	<< "CLICK ENTER TO CONTINUE...\n";
	while (std::cin.get() != '\n') { }
}
