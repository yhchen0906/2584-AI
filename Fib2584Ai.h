#ifndef __FIB2584AI_H__
#define __FIB2584AI_H__

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include "Fib2584/MoveDirection.h"
using namespace std;

const int FTSIZE = 6 * 22 * 22 * 22 * 22 * 22 * 22;

class Fib2584Ai
{
public:
	Fib2584Ai();
    ~Fib2584Ai();
	// initialize ai
	void initialize(int argc, char* argv[]);
	// generate one move
	MoveDirection generateMove(int board[4][4]);
	// do some action when game over
	void gameOver(int board[4][4], int iScore);
    double getVt(const vector<int>&);
    void updateFeature(const double&);

	/**********************************
	You can implement any additional functions
	or define any variables you may need.
	**********************************/
private:
    double* feature;
    double* delta_n;
    double* delta_d;
    vector<int> preVec;
};

#endif
