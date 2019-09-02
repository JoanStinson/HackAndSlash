#include "RandomNumber.h"

int getRandomNumber(int mod){
	//returns random number between 0 and mod
	return rand() % mod;
}