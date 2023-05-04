#include <vector>
#include <iostream>
#include <random>
#include "common.h"
#include "timing.h"

using namespace std;

vector<double> GWO(double (*objf)(double[], int,vector<vector<double>>&), int dim, int SearchAgents_no, int Max_iter,vector<vector<double>>& data)
{

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
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < SearchAgents_no; j++)
        {
            Positions[j][i] = dis(gen);
        }
    }

    Timer totalSimulationTimer;
    // Main loop
    for (int l = 0; l < Max_iter; l++)
    {
        // define alpha beta and delta wolves
        // TODO: can we have more wolves? so best 4 solutions instead
        for (int i = 0; i < SearchAgents_no; i++)
        {
            // Return back the search agents that go beyond the boundaries of the search space
            for (int j = 0; j < dim; j++)
            {
                Positions[i][j] = max(0.0, min(Positions[i][j], 1.0));
            }

            // Calculate objective function for each search agent
            double fitness = objf(Positions[i], dim,data);

            // Update Alpha, Beta, and Delta
            if (fitness < Alpha_score)
            {
                Alpha_score = fitness; // Update alpha
                copy(Positions[i], Positions[i] + dim, Alpha_pos);
            }

            if (fitness > Alpha_score && fitness < Beta_score)
            {
                Beta_score = fitness; // Update beta
                copy(Positions[i], Positions[i] + dim, Beta_pos);
            }

            if (fitness > Alpha_score && fitness > Beta_score && fitness < Delta_score)
            {
                Delta_score = fitness; // Update delta
                copy(Positions[i], Positions[i] + dim, Delta_pos);
            }
        }

        double a = 2 - l * (2.0 / Max_iter); // a decreases linearly fron 2 to 0

        // Update the position of search agents including omegas
        for (int i = 0; i < SearchAgents_no; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                double D_alpha = abs(getC(dis, gen) * Alpha_pos[j] - Positions[i][j]); // Equation (3.5)-part 1
                double X1 = Alpha_pos[j] - getA(a, dis, gen) * D_alpha;                // Equation (3.6)-part 1

                double D_beta = abs(getC(dis, gen) * Beta_pos[j] - Positions[i][j]); // Equation (3.5)-part 2
                double X2 = Beta_pos[j] - getA(a, dis, gen) * D_beta;                // Equation (3.6)-part 2

                double D_delta = abs(getC(dis, gen) * Delta_pos[j] - Positions[i][j]); // Equation (3.5)-part 3
                double X3 = Delta_pos[j] - getA(a, dis, gen) * D_delta;                // Equation (3.5)-part 3

                Positions[i][j] = (X1 + X2 + X3) / 3; // Equation (3.7)
            }
        }
        cout << "--------" << endl;
        cout << l << " " << Alpha_score << endl;
        for (int j = 0; j < dim; j++)
        {
            cout << Alpha_pos[j] << " ";
        }
        cout << endl;
    }
    
    double totalSimulationTime = totalSimulationTimer.elapsed();
    printf("total simulation time: %.6fs\n", totalSimulationTime);

    vector<double> res;
    for (int i = 0; i < dim; i++)
    {
        res.emplace_back(Alpha_pos[i]);
    }
    return res;
}

int main()
{
    vector<vector<double>> data;
    read_data("cleavland-more.csv", data);
    // print_data(data);
    // standardize(data);
    // print_data(data);
    vector<double> res = GWO(fitness_func, data[0].size(), 5, 10,data);
    return 0;
}