#include "datahelpers.h"


//take a line of a csv file in format "x1,x2,x3,x4,"label"". return the inputs as a double vector and push the label into empty double vector passed
std::vector<double> parseString(const std::string & stringToParse, std::vector<double>& nameVector) {
    //split
    std::vector<double> result;
    std::string token = "";
    int commaCount = 0;

    for (int i = 0; i < stringToParse.size(); i++) {
        if (stringToParse[i] != ',') { //if not comma put char in token
            token.push_back(stringToParse[i]);
        } else { //if comma
            commaCount += 1;
            if (commaCount < 4) { //if comma count < 4 when we hit comma, cast to double and push into result vector
                result.push_back(strtod(token.c_str(), NULL));
                token = "";
            } else if (commaCount == 4) { //strip the quotes and append the classification to the nameVector.
                //push_back that last token in between comma 3 and 4
                result.push_back(strtod(token.c_str(),NULL));

                token = stringToParse.substr(i + 2, stringToParse.size() - (i + 2) -1 );
                if (token == "Setosa") {
                    nameVector.push_back(1.0);
                } else if (token == "Versicolor" || token == "Virginica") {
                    nameVector.push_back(-1.0);
                }
                break;
            }
        }
    }
    return result;
}

void print2dVector(const std::vector<std::vector<double>> & vecToPrint) {
    for (int i = 0; i < vecToPrint.size(); i++) {
        for (int j = 0; j < vecToPrint[i].size(); j++) {
            std::cout << "vecToPrint[" << i << "][" << j << "]: " << vecToPrint[i][j] << std::endl;
        }
    }
}

void test(const std::vector<std::vector<double>> & testX,const std::vector<double> & testY, Perceptron & p) {
    for (int i = 0; i < testX.size(); i++) {
        std::cout << "Neuron Guessed: " << p.stepFunction(p.weightedSum(testX[i], p.weights)) << "\n";
        std::cout << "Correct Answer: " << testY[i] << "\n";
    }
    p.printWeights();
}
