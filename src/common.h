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
using namespace std;

void read_data(string filename, vector<vector<double>> &data);
void standardize(std::vector<std::vector<double>>& input);
double fitness_func(double input[], int dim);
void print_data(vector<vector<double>> &data);

#endif
