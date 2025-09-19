#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
#include <iostream>

class Perceptron {
    public:
    Perceptron(int numInputs) //initialize weights and bias to zero
    {
        for (int i = 0; i < numInputs; i++) {
            weights.push_back(0.0);
        }
        bias = 0.0;
    }
    std::vector<double> weights;
    double bias;
    double learningRate = 0.001;

    //x >= 0 == 1, x < 0 == -1
    double stepFunction(double x) { if (x >= 0) { return 1.0; } else { return -1.0;} }

    double weightedSum(const std::vector<double> & inp, const std::vector<double> & w) const;

    void train(const std::vector<std::vector<double>>& trainingX, const std::vector<double> & trainingY, int maxIters);
    void printWeights() const;
};

#endif
