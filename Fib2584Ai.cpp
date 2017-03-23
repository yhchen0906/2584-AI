#include "Fib2584Ai.h"
#include <fstream>
#include <iostream>
#include "Fib2584/MyGameBoard.h"
using namespace std;

Fib2584Ai::Fib2584Ai() {
    {
        ifstream fin("feature.dat", ifstream::binary);
        if (fin) {
            feature = new double [FTSIZE];
            fin.read(reinterpret_cast<char*>(feature), sizeof(double) * FTSIZE);
            fin.close();
        } else
            feature = new double [FTSIZE] ();
    }
    /*
    {
        ifstream fin("delta_n.dat", ifstream::binary);
        if (fin) {
            delta_n = new double [FTSIZE];
            fin.read(reinterpret_cast<char*>(delta_n), sizeof(double) * FTSIZE);
            fin.close();
        } else
            delta_n = new double [FTSIZE] ();
    }
    {
        ifstream fin("delta_d.dat", ifstream::binary);
        if (fin) {
            delta_d = new double [FTSIZE];
            fin.read(reinterpret_cast<char*>(delta_d), sizeof(double) * FTSIZE);
            fin.close();
        } else
            delta_d = new double [FTSIZE] ();
    }
    */
}

Fib2584Ai::~Fib2584Ai() {
    {
        /*
        ofstream fout("feature.dat", ifstream::binary);
        fout.write(reinterpret_cast<char*>(feature), sizeof(double) * FTSIZE);
        fout.close();
        */
        delete [] feature;
    }
    /*
    {
        ofstream fout("delta_n.dat", ifstream::binary);
        fout.write(reinterpret_cast<char*>(delta_n), sizeof(double) * FTSIZE);
        fout.close();
        delete [] delta_n;
    }
    {
        ofstream fout("delta_d.dat", ifstream::binary);
        fout.write(reinterpret_cast<char*>(delta_d), sizeof(double) * FTSIZE);
        fout.close();
        delete [] delta_d;
    }
    */
}

void Fib2584Ai::initialize(int argc, char* argv[])
{
    srand(time(NULL));
    return;
}


MoveDirection Fib2584Ai::generateMove(int board[4][4])
{
    double bestVt = -9999;
    vector<int> bestVec;
    MoveDirection bestDir = static_cast<MoveDirection>(rand() % 4);
    MyGameBoard mb(board);
    for (int i = 0; i < 4; ++i) {
        MyGameBoard tmp = mb;
        int reward = tmp.move(static_cast<MoveDirection>(i));
        if (tmp == mb) continue;
        vector<int> tVec = tmp.getVec();
        double tVt = getVt(tVec);
        if (tVt + reward > bestVt) {
            bestVec = tVec;
            bestVt = tVt + reward;
            bestDir = static_cast<MoveDirection>(i);
        }
    }
    /*
    updateFeature(bestVt);
    preVec = bestVec;
    */
    return bestDir;
}

void Fib2584Ai::gameOver(int board[4][4], int iScore)
{
    /*
    updateFeature(0.0);
    preVec.clear();
    */
    return;
}

double Fib2584Ai::getVt(const vector<int>& vec) {
    double vt = 0.0;
    for (auto&& v: vec)
        vt += feature[v];
    return vt;
}

void Fib2584Ai::updateFeature(const double& vt) {
    const double delta = (vt - getVt(preVec)) * 0.05;
    for (auto&& v: preVec) {
        double alpha = 1.0;
        if (abs(delta_d[v]) > 1e-3)
            alpha = abs(delta_n[v]) / delta_d[v];
        feature[v] += delta * alpha;
        delta_n[v] += delta;
        delta_d[v] += abs(delta);
    }
}

/**********************************
  You can implement any additional functions
  you may need.
 **********************************/
