#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "svm.h" //libsvm header file

#include <numeric>
#include <random>

using namespace std;

const int DEFAULT_AGENT_NUM = 5;
const int DEFAULT_ITER_NUM = 10;
const string DEFAULT_DATA = "cleavland.csv";
const double THRESHOLD = 0.5;

class GWOArgs {
public:
    int agentNum;
    int iterNum;
    string dataSource;

    GWOArgs();
    GWOArgs(int agents, int iterations, string dataSource);
    string toString() const;
};

GWOArgs parse_arguments(int argc, char* argv[]);
void read_data(string filename, vector<vector<double>> &data);
void standardize(std::vector<std::vector<double>> &input);
double fitness_func(double input[], int dim, vector<vector<double>>& data);
void print_data(vector<vector<double>> &data);
void writeResult(string method, double alphaScore, double time, GWOArgs & args);
inline double getA(double &a, uniform_real_distribution<> &dis, mt19937 &gen)
{
    return 2 * a * dis(gen) - a; // Equation (3.3)
}
inline double getC(uniform_real_distribution<> &dis, mt19937 &gen)
{
    return 2 * dis(gen); // Equation (3.4)
}
#endif
