#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>

#include <dlib/matrix.h>

using namespace std;
using namespace mlpack;

void read_data(string filename, vector<vector<float>> &data) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip the first line (header)
    while (getline(file, line)) {
        vector<float> row;
        string cell;
        stringstream lineStream(line);

        while (getline(lineStream, cell, ',')) {
            row.push_back(stof(cell));
        }

        data.push_back(row);
    }
}

int main() {
    vector<vector<float>> data;
    read_data("test.csv", data);

    return 0;
}