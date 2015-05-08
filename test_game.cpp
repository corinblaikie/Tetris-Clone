#include <string>
#include <iostream>
#include <map>
#include "game.h"


using namespace std;

void quit()
{
     cout << "Press any key to exit...";
     getchar();
}


void testRandomness()
{
	map<string, int>::iterator colorIterator;
	map<string, int> colorMap;

	Game g = Game();
	for(int x = 0; x < 50000; x++)
	{
		g.NextTetromino();
		string currentColor = string(g.GetCurrentTetromino()->GetColor().ToHex());
		if(colorMap.find(currentColor) != colorMap.end())
		{
			colorMap[currentColor] += 1;
		}
		else
		{
			colorMap[currentColor] = 1;
		}
	}

	for(colorIterator = colorMap.begin(); colorIterator != colorMap.end(); ++colorIterator){
		cout << "Key: " << (*colorIterator).first << " Value: " << (*colorIterator).second << std::endl;
	}
}

int main()
{
	
	//std::map <string, char> grade_list;
	//grade_list["John"] = 'A';
	//// Should be John
	//std::cout << grade_list.begin()->first << std::endl;
	//// Should be A
	//std::cout << grade_list.begin()->second << std::endl;


	 testRandomness();

	 quit();
}
