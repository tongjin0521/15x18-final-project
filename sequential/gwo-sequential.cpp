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
#include <random>


using namespace std;

void read_data(string filename, vector<vector<double>> &data) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip the first line (header)
    while (getline(file, line)) {
        vector<double> row;
        string cell;
        stringstream lineStream(line);

        while (getline(lineStream, cell, ',')) {
            row.push_back(stof(cell));
        }

        data.push_back(row);
    }
}

void standardize(std::vector<std::vector<double>>& input) {
    for (int i = 0; i < input[0].size(); i++) {
        // Calculate the mean of the column
        double sum = std::accumulate(input.begin(), input.end(), 0.0f, [i](double a, const std::vector<double>& b) { return a + b[i]; });
        double mean = sum / input.size();

        // Calculate the standard deviation of the column
        double sq_sum = std::inner_product(input.begin(), input.end(), input.begin(), 0.0f, std::plus<>(), [i, mean](const std::vector<double>& a, const std::vector<double>& b) { return (b[i] - mean) * (b[i] - mean); });
        double stdev = std::sqrt(sq_sum / input.size());

        // Subtract the mean and divide by the standard deviation for each element in the column
        for (int j = 0; j < input.size(); j++) {
            input[j][i] = (input[j][i] - mean) / stdev;
        }
    }
}

void print_data(vector<vector<double>> &data){
    std::cout <<"--------" <<std::endl;
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            std::cout << data[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

double fitness_func(double input[], int dim) {
    // TODO: you should be using the input as labels to know which data you want
    // then use data to calculate objf

    double o = 0;
    for (int i = 0; i <dim; i++) {
        o = max(o, abs(input[i]));
    }
    return o;
}




vector<double> GWO(double (*objf)(double[],int), int dim, int SearchAgents_no, int Max_iter) {
    
    // Initialize alpha, beta, and delta_pos
    double Alpha_pos[dim];
    double Alpha_score = INFINITY;
    
    double Beta_pos[dim];
    double Beta_score = INFINITY;
    
    double Delta_pos[dim];
    double Delta_score = INFINITY;

    // Initialize the positions of search agents
    double Positions[SearchAgents_no][dim];
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < SearchAgents_no; j++) {
            Positions[j][i] = dis(gen);
        }
    }
    
    double Convergence_curve[Max_iter];

    // Main loop
    for (int l = 0; l < Max_iter; l++) {
        // define alpha beta and delta wolves
        // TODO: can we have more wolves? so best 4 solutions instead
        for (int i = 0; i < SearchAgents_no; i++) {
            
            // Return back the search agents that go beyond the boundaries of the search space
            for (int j = 0; j < dim; j++) {
                Positions[i][j] = max(0.0, min(Positions[i][j], 1.0));
            }

            // Calculate objective function for each search agent
            double fitness = objf(Positions[i],dim);
            
            // Update Alpha, Beta, and Delta
            if (fitness < Alpha_score) {
                Alpha_score = fitness; // Update alpha
                copy(Positions[i], Positions[i] + dim, Alpha_pos);
            }
            
            if (fitness > Alpha_score && fitness < Beta_score) {
                Beta_score = fitness; // Update beta
                copy(Positions[i], Positions[i] + dim, Beta_pos);
            }
            
            if (fitness > Alpha_score && fitness > Beta_score && fitness < Delta_score) {
                Delta_score = fitness; // Update delta
                copy(Positions[i], Positions[i] + dim, Delta_pos);
            }
        }
        
        double a = 2 - l * (2.0 / Max_iter); // a decreases linearly fron 2 to 0
        
        // Update the position of search agents including omegas
        for (int i = 0; i < SearchAgents_no; i++) {
            for (int j = 0; j < dim; j++) {
                           
                double r1 = dis(gen); // r1 is a random number in [0,1]
                double r2 = dis(gen); // r2 is a random number in [0,1]
                
                double A1 = 2 * a * r1 - a; // Equation (3.3)
                double C1 = 2 * r2; // Equation (3.4)
                
                double D_alpha = abs(C1 * Alpha_pos[j] - Positions[i][j]); // Equation (3.5)-part 1
                double X1 = Alpha_pos[j] - A1 * D_alpha; // Equation (3.6)-part 1
                           
                r1 = dis(gen);
                r2 = dis(gen);

                double A2 = 2*a*r1-a; // Equation (3.3)
                double C2 = 2*r2; // Equation (3.4)

                double D_beta = abs(C2*Beta_pos[j]-Positions[i][j]); // Equation (3.5)-part 2
                double X2 = Beta_pos[j]-A2*D_beta; // Equation (3.6)-part 2       
                
                r1 = dis(gen);
                r2 = dis(gen);

                double A3 = 2*a*r1-a; // Equation (3.3)
                double C3 = 2*r2; // Equation (3.4)

                double D_delta = abs(C3*Delta_pos[j]-Positions[i][j]); // Equation (3.5)-part 3
                double X3 = Delta_pos[j]-A3*D_delta; // Equation (3.5)-part 3             

                Positions[i][j] = (X1+X2+X3)/3; // Equation (3.7)        
            }
        }
        cout<<"--------"<<endl;
        cout <<l <<" "<<Alpha_score << endl;
        for (int j = 0; j <dim; j++) {
            std::cout << Alpha_pos[j] << " ";
        }
        std::cout << std::endl;
    }
    vector<double> res;
    for (int i = 0; i < dim; i++){
        res.emplace_back(Alpha_pos[i]);
    }
    return res;
}


int main() {
    vector<vector<double>> data;
    read_data("test.csv", data);
    // print_data(data);
    standardize(data);
    // print_data(data);
    vector<double> res = GWO(fitness_func,data[0].size(),5,10);
    return 0;
}