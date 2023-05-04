

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
        {63,1,1,145,233,1,2,150,0,2.3,3,0,6,0},
        {67,1,4,160,286,0,2,108,1,1.5,2,3,3,2},
        {67,1,4,120,229,0,2,129,1,2.6,2,2,7,1},
        {37,1,3,130,250,0,0,187,0,3.5,3,0,3,0},
        {41,0,2,130,204,0,2,172,0,1.4,1,0,3,0},
        {56,1,2,120,236,0,0,178,0,0.8,1,0,3,0},
        {62,0,4,140,268,0,2,160,0,3.6,3,2,3,3},
        {57,0,4,120,354,0,0,163,1,0.6,1,0,3,0},
        {63,1,4,130,254,0,2,147,0,1.4,2,1,7,2}
    };


    // The selected columns to be used
    vector<int> selectedCols = {3, 4, 5,6, 8, 9, 10,  12}; // for example, select columns 1, 3, 4, 5, 8, 9, 10, 11, and 12

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

