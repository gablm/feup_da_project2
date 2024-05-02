#ifndef UI_H
#define UI_H

#include "../manager/manager.h"
#include <chrono>

#ifdef __linux__
# define CLEAR (void)system("clear")
#else
# define CLEAR (void)system("cls")
#endif

enum HeuristicType {
	backtracking_2_1,
	triangular_approximation_2_2,
	other_2_3,
	real_world_2_4
};

class UI {
	private:
		Manager manager;
	public:
		void mainMenu();
		void selectorMenu();

};

#endif