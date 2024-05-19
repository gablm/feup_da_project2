#ifndef UI_H
#define UI_H

#include "../manager/manager.h"
#include <chrono>
#include <iomanip>

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

		bool isTSPInvalid(ReturnDataTSP data);
		bool isTSPFailed(ReturnDataTSP data);

		Vertex *getBaseVertex();
};

#endif