#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

void read_data(string filename, vector<vector<double>> &data);
void standardize(std::vector<std::vector<double>> &input);
double fitness_func(double input[], int dim);
void print_data(vector<vector<double>> &data);
inline double getA(double &a, uniform_real_distribution<> &dis, mt19937 &gen);
inline double getC(double &a, uniform_real_distribution<> &dis, mt19937 &gen);

#endif
