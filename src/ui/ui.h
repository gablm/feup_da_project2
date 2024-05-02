#ifndef UI_H
#define UI_H

#include "../manager/manager.h"
#include <chrono>

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
		void testManager();
		static void showMessage(std::string header, std::string body);

		void mainMenu();
		void selectorMenu();
		void pointSelMenu(int type);
		void resultMenu(int type);
};

#endif