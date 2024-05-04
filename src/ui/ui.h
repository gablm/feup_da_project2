#ifndef UI_H
#define UI_H

#include "../manager/manager.h"
#include <chrono>

/**
 * Identifies the heuristic for the 
 * Travelling Salesman Problem "solution".
*/
enum HeuristicType {
	backtracking_2_1,
	triangular_approximation_2_2,
	other_2_3,
	real_world_2_4
};

/**
 * Contains the methods to display the CLI 
 * and the manager contains all the information.
*/
class UI {
	private:
		Manager manager;
	public:
		void testManager();
		static void showMessage(std::string header, std::string body);

		void mainMenu();
		void selectorMenu();
		void loaderScreen(DatasetType type, int option);
		void resultMenu(HeuristicType type);
};

#endif