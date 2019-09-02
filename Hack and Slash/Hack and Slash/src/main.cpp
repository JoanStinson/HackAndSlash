#include <iostream>
#include "Screen.h"
using namespace std;

int main(int argCount, char *args[]) {

	Screen screen;

	if (!screen.Start()) {
		cout << "Error initialising SDL!" << endl;
	}
	else {
		screen.Update();
	}

	screen.Quit();

	return 0;
}