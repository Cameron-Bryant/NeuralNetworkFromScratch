#include "perceptron.h"

double Perceptron::weightedSum(const std::vector<double> &inp, const std::vector<double> &w) const {
    double ws = 0.0;
    for (int i = 0; i < w.size(); i++) { // y = wx + b
        ws += inp[i] * w[i];
    }
    return ws + bias;
}

void Perceptron::train(const std::vector<std::vector<double>> & trainingX, const std::vector<double> & trainingY, int maxIters) {
    for (int i = 0; i < maxIters; i++) { //for all iterations
        double activation = 0.0;
        for (int j = 0; j < trainingX.size(); j++) {//for all D
            activation = weightedSum(trainingX[j], weights);
            if (trainingY[j] * activation <= 0.0) {
                //update weights
                for (int k = 0; k < trainingX[j].size(); k++) { //w <- Wd + Y*Xd
                    weights[k] += learningRate * (trainingY[j] - stepFunction(activation)) * trainingX[j][k];
                } //b <- b + Y
                bias += learningRate * (trainingY[j] - stepFunction(activation));
            }
        }
    }
}

void Perceptron::printWeights() const {
    for (int i = 0; i < weights.size(); i++) {
        std::cout << "Weight " << i << ":" << weights[i] << std::endl;
    }
    std::cout << "Bias:" << bias << "\n";
}
