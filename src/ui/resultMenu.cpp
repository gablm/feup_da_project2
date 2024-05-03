#include "ui.h"

void UI::resultMenu(int type)
{
	while (1)
	{
		CLEAR;
		
		std::string str;
		std::getline(std::cin, str);
		
		if (str == "b" || str == "B") return;
		
		if (str == "q" || str == "Q")
		{
			CLEAR;
			exit(0);
		}
	}
	
}
