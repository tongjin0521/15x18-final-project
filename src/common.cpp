#include "common.h"
using namespace std;

const double THRESHOLD = 0.5;

void read_data(string filename, vector<vector<double>> &data)
{
    ifstream file(filename);
    string line;
    getline(file, line); // Skip the first line (header)
    while (getline(file, line))
    {
        vector<double> row;
        string cell;
        stringstream lineStream(line);

        while (getline(lineStream, cell, ','))
        {
            row.push_back(stof(cell));
        }

        data.push_back(row);
    }
}

// todo: a is not used in sq_sum step
// void standardize(vector<vector<double>> &input)
// {

//     for (size_t i = 0; i < input[0].size(); i++)
//     {
//         double sum = accumulate(input.begin(), input.end(), 0.0f, [i](double a, const vector<double> &b)
//                                 { return a + b[i]; });
//         double mean = sum / input.size();

//         // Calculate the standard deviation of the column
//         double sq_sum = inner_product(input.begin(), input.end(), input.begin(), 0.0f, plus<>(), [i, mean](const vector<double> &a, const vector<double> &b)
//                                       { return (b[i] - mean) * (b[i] - mean); });
//         double stdev = sqrt(sq_sum / input.size());

//         // Subtract the mean and divide by the standard deviation for each element in the column
//         for (size_t j = 0; j < input.size(); j++)
//         {
//             input[j][i] = (input[j][i] - mean) / stdev;
//         }
//     }
// }

void print_data(vector<vector<double>> &data)
{
    cout << "--------" << endl;
    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t j = 0; j < data[i].size(); j++)
        {
            cout << data[i][j] << " ";
        }
        cout << endl;
    }
}

double fitness_func(double input[], int dim, vector<vector<double>>& data)
{   
    vector<int> selectedCols;
    for (int i = 0; i < dim; i++){
        if (input[i] < THRESHOLD){
            selectedCols.emplace_back(0);
        }else{
            selectedCols.emplace_back(1);
        }
    }
    
    // Prepare the dataset using only the selected columns
    vector<svm_node*> nodes;
    vector<double> labels;
    for (size_t i = 0; i < data.size(); i++) {
        svm_node *x = new svm_node[selectedCols.size()+1];
        for (size_t j = 0; j < selectedCols.size(); j++) {
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
    for (size_t i = 0; i < nodes.size(); i++) {
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

    // Predict the labels of the training data
    double *predictions = new double[problem.l];
    svm_predict_values(model,(const svm_node*) problem.x, predictions);

    // Calculate the accuracy
    int correct = 0;
    for (int i = 0; i < problem.l; i++) {
        if (predictions[i] == problem.y[i]) {
            correct++;
        }
    }
    double accuracy = (double) correct / problem.l;
    // Free allocated memory
    for (size_t i = 0; i < nodes.size(); i++) {
        delete[] nodes[i];
    }
    delete[] problem.x;
    delete[] problem.y;
    svm_free_and_destroy_model(&model);

    return accuracy;
}
