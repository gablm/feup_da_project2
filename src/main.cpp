#include "ui/ui.h"

/**
 * Sets the console output mode to UTF8 if the platform is windows.
 * Calls the dataset selection menu.
*/
int main(void)
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif
    UI().mainMenu();
    return 0;
}
