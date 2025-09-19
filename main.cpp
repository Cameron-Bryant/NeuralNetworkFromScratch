#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include "perceptron.h"
#include "datahelpers.h"
#include "layer.h"


int main() {
    std::vector<std::vector<double>> trainX = { //AND
        {0,0},
        {0,1},
        {1,1},
        {1,0}
    };

    std::vector<double> trainY = {-1, -1, 1, -1};//AND

    std::vector<std::vector<double>> testX = { //AND
        {1,1},
        {0,1},
        {1,0},
        {0,0}
    };

    std::vector<double> testY = {1, -1, -1, -1};

    Perceptron neuron(2);
    neuron.train(trainX, trainY, 10);
    test(testX, testY, neuron);

    //IRIS - Setosa is linearly seperable from the others
    Perceptron binarySetosa(4);
    std::vector<std::string> irisCSV;
    std::vector<std::vector<double>> irisX; //stores all the input values: SepalLengthCM, SepalWidthCM, PetalLengthCM, PetalWidthCM
    std::vector<double> irisY;
    std::ifstream file("C:/Users/camer/Desktop/iris.csv");
    std::string line;
    while (std::getline(file, line)) {
        irisCSV.push_back(line);
    }
    //parseString datahelpers.h function. Returns the inputs in a vector, pushes back a binary label (is/not setosa) into irisY
    for (int i = 1; i < irisCSV.size(); i++) {
        line = irisCSV[i];
        irisX.push_back(parseString(line, irisY));
    }

    //train and test
    binarySetosa.train(irisX, irisY, 10);
    test(irisX, irisY, binarySetosa);

    return 0;
}
