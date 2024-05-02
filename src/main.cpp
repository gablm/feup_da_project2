#include "ui/ui.h"

#ifdef _WIN32
#include "windows.h"
#endif

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
