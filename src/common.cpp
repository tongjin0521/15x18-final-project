#include "common.h"
using namespace std;

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

void standardize(vector<vector<double>> &input)
{

    for (size_t i = 0; i < input[0].size(); i++)
    {
        double sum = accumulate(input.begin(), input.end(), 0.0f, [i](double a, const vector<double> &b)
                                { return a + b[i]; });
        double mean = sum / input.size();

        // Calculate the standard deviation of the column
        double sq_sum = inner_product(input.begin(), input.end(), input.begin(), 0.0f, plus<>(), [i, mean](const vector<double> &a, const vector<double> &b)
                                      { return (b[i] - mean) * (b[i] - mean); });
        double stdev = sqrt(sq_sum / input.size());

        // Subtract the mean and divide by the standard deviation for each element in the column
        for (size_t j = 0; j < input.size(); j++)
        {
            input[j][i] = (input[j][i] - mean) / stdev;
        }
    }
}

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

double fitness_func(double input[], int dim)
{
    // TODO: you should be using the input as labels to know which data you want
    // then use data to calculate objf

    double o = 0;
    for (int i = 0; i < dim; i++)
    {
        o = max(o, abs(input[i]));
    }
    return o;
}

inline double getA(double &a, uniform_real_distribution<> &dis, mt19937 &gen)
{
    return 2 * a * dis(gen) - a; // Equation (3.3)
}

inline double getC(uniform_real_distribution<> &dis, mt19937 &gen)
{
    return 2 * dis(gen); // Equation (3.4)
}