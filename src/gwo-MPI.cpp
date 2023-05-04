#include <vector>
#include <iostream>
#include <random>
#include "common.h"
#include "timing.h"
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[])
{
    int pid;
    int nproc;

    GWOArgs args = parse_arguments(argc, argv);

    vector<vector<double>> data;
    read_data(args.dataSource, data);
    const int dim = data[0].size();

    // Initialize MPI
    MPI_Init(&argc, &argv);
    // Get process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    // Get total number of processes specificed at start of run
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    // Initialize alpha, beta, and delta_pos
    // All agents need to do this since they need to store lead wolf positions
    // TODO: can we have more wolves? so best 4 solutions instead
    double Alpha_pos[dim];
    double Alpha_score = INFINITY;

    double Beta_pos[dim];
    double Beta_score = INFINITY;

    double Delta_pos[dim];
    double Delta_score = INFINITY;

    // Initialize the positions of this search agent
    double pos[dim];
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);
    for (int i = 0; i < dim; i++)
    {
        pos[i] = dis(gen);
    }

    Timer totalSimulationTimer;
    // Main loop
    for (int l = 0; l < args.iterNum; l++)
    {
        // define alpha beta and delta wolves

        // Return back the search agents that go beyond the boundaries of the search space
        for (int i = 0; i < dim; i++)
        {
            pos[i] = max(0.0, min(pos[i], 1.0));
        }

        // Calculate objective function for each search agent
        double fitness = fitness_func(pos, dim, data);

        // for master
        double fitnesses[nproc];
        double positions[nproc][dim];

        // The requests are not tracked
        MPI_Request fitRequest;
        MPI_Request posRequest;
        MPI_Request alpRequest;
        MPI_Request betRequest;
        MPI_Request delRequest;

        if (pid != 0)
        {
            // send fitness to master
            MPI_Isend(&fitness, 1, MPI_DOUBLE, 0, pid * 1000, MPI_COMM_WORLD, &fitRequest);
            // send my postion to master
            MPI_Isend(pos, dim, MPI_DOUBLE, 0, pid * 1000 + 1, MPI_COMM_WORLD, &posRequest);
        }
        else
        {
            // master receives fitness from all other processes
            fitnesses[0] = fitness;
            for (int i = 1; i < nproc; i++)
            {
                MPI_Recv(&fitnesses[i], 1, MPI_DOUBLE, i, i * 1000, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            // master receives positions from all other processes
            copy(pos, pos + dim, positions[0]);
            for (int i = 1; i < nproc; i++)
            {
                MPI_Recv(positions[i], dim, MPI_DOUBLE, i, i * 1000 + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }

        // master update Alpha, Beta, and Delta positions based on fitnesses
        if (pid == 0)
        {
            // Update Alpha, Beta, and Delta
            for (int i = 0; i < nproc; i++)
            {
                if (fitnesses[i] < Alpha_score)
                {
                    Alpha_score = fitnesses[i];
                    copy(positions[i], positions[i] + dim, Alpha_pos);
                }

                if (fitnesses[i] > Alpha_score && fitnesses[i] < Beta_score)
                {
                    Beta_score = fitnesses[i];
                    copy(positions[i], positions[i] + dim, Beta_pos);
                }

                if (fitnesses[i] > Alpha_score && fitnesses[i] > Beta_score && fitnesses[i] < Delta_score)
                {
                    Delta_score = fitnesses[i];
                    copy(positions[i], positions[i] + dim, Delta_pos);
                }
            }
            // send new Alpha, Beta, and Delta to all other processes asynchly
            for (int i = 1; i < nproc; i++)
            {
                MPI_Isend(Alpha_pos, dim, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &alpRequest);
                MPI_Isend(Beta_pos, dim, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &betRequest);
                MPI_Isend(Delta_pos, dim, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &delRequest);
            }
        }
        else
        {
            // receive new Alpha, Beta, and Delta from master
            MPI_Recv(Alpha_pos, dim, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(Beta_pos, dim, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(Delta_pos, dim, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        double a = 2 - l * (2.0 / args.iterNum); // a decreases linearly fron 2 to 0

        // Update the position of this search agent

        for (int j = 0; j < dim; j++)
        {
            double D_alpha = abs(getC(dis, gen) * Alpha_pos[j] - pos[j]); // Equation (3.5)-part 1
            double X1 = Alpha_pos[j] - getA(a, dis, gen) * D_alpha;       // Equation (3.6)-part 1

            double D_beta = abs(getC(dis, gen) * Beta_pos[j] - pos[j]); // Equation (3.5)-part 2
            double X2 = Beta_pos[j] - getA(a, dis, gen) * D_beta;       // Equation (3.6)-part 2

            double D_delta = abs(getC(dis, gen) * Delta_pos[j] - pos[j]); // Equation (3.5)-part 3
            double X3 = Delta_pos[j] - getA(a, dis, gen) * D_delta;       // Equation (3.5)-part 3

            pos[j] = (X1 + X2 + X3) / 3; // Equation (3.7)
        }

        // if (pid == 0)
        // {
        //     cout << "--------" << endl;
        //     cout << l << " " << Alpha_score << endl;
        //     for (int j = 0; j < dim; j++)
        //     {
        //         cout << Alpha_pos[j] << " ";
        //     }
        //     cout << endl;
        // }
    }

    if (pid == 0)
    {
        double totalSimulationTime = totalSimulationTimer.elapsed();
        // printf("\n%.6f\n", now / options.numIterations);
        printf("total simulation time: %.6fs\n", totalSimulationTime);
        vector<double> res;
        for (int i = 0; i < dim; i++)
        {
            res.emplace_back(Alpha_pos[i]);
        }
    }

    MPI_Finalize();
    return 0;
}