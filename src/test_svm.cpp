

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

using namespace std;

int main()
{
    // The dataset
    vector<vector<double>> data = {
        {1.0, 0.5, 0.2, 0.1, 1.0, 0.8, 0.7, 0.6, 0.9, 0.2, 0.1, 0.0, 0.0, 1.0},
        {0.5, 0.8, 0.4, 0.3, 0.6, 0.7, 0.5, 0.4, 0.6, 0.3, 0.4, 0.0, 1.0, 2.0},
        {0.2, 0.4, 0.9, 0.7, 0.2, 0.1, 0.8, 0.6, 0.5, 0.9, 0.7, 0.0, 0.0, 3.0},
        {0.3, 0.1, 0.7, 0.5, 0.8, 0.9, 0.3, 0.2, 0.1, 0.8, 0.6, 0.0, 1.0, 4.0},
        {0.9, 0.6, 0.2, 0.1, 0.7, 0.5, 0.4, 0.3, 0.2, 0.1, 0.3, 0.0, 1.0, 0.0},
        {0.4, 0.2, 0.1, 0.9, 0.5, 0.6, 0.7, 0.8, 0.9, 0.7, 0.5, 0.0, 0.0, 1.0},
        {0.7, 0.3, 0.5, 0.8, 0.3, 0.1, 0.9, 0.6, 0.7, 0.8, 0.9, 0.0, 1.0, 2.0},
        {0.1, 0.9, 0.6, 0.4, 0.4, 0.8, 0.6, 0.5, 0.4, 0.5, 0.6, 0.0, 1.0, 3.0},
        {0.8, 0.7, 0.3, 0.2, 0.9, 0.4, 0.3, 0.2, 0.3, 0.6, 0.8, 0.0, 1.0, 4.0},
        {0.6, 0.5, 0.8, 0.6, 0.1, 0.2, 0.5, 0.7, 0.8, 0.9, 0.7, 0.0, 0.0, 0.0}};

    // The selected columns to be used
    vector<int> selectedCols = {1, 3, 4, 5, 8, 9, 10, 11, 12}; // for example, select columns 1, 3, 4, 5, 8, 9, 10, 11, and 12

    // Prepare the dataset using only the selected columns
    vector<svm_node*> nodes;
    vector<double> labels;
    for (int i = 0; i < data.size(); i++) {
        svm_node *x = new svm_node[selectedCols.size()+1];
        for (int j = 0; j < selectedCols.size(); j++) {
            x[j].index = j+1;
            x[j].value = data[i][selectedCols[j]];
        }
        x[selectedCols.size()].index = -1;
        nodes.push_back(x);
        labels.push_back(data[i][data[i].size()-1]); // last column is the label
    }

    // Prepare the SVM problem
    svm_problem problem;
    problem.l = nodes.size();
    problem.x = new svm_node*[problem.l];
    problem.y = new double[problem.l];
    for (int i = 0; i < nodes.size(); i++) {
        problem.x[i] = nodes[i];
        problem.y[i] = labels[i];
    }

    // Set SVM parameters
    svm_parameter param;
    param.svm_type = C_SVC;
    param.kernel_type = RBF;
    param.gamma = 0.5;
    param.C = 1;

    // Train the SVM
    svm_model *model = svm_train(&problem, &param);

    // Free allocated memory
    for (int i = 0; i < nodes.size(); i++) {
        delete[] nodes[i];
    }
    delete[] problem.x;
    delete[] problem.y;
    svm_free_and_destroy_model(&model);

    return 0;
}

