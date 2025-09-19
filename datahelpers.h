#ifndef DATAHELPERS_H
#define DATAHELPERS_H

#include <vector>
#include <string>
#include <iostream>
#include "perceptron.h"

std::vector<double> parseString(const std::string & stringToParse, std::vector<double>& nameVector);

void print2dVector(const std::vector<std::vector<double>> & vecToPrint);

void test(const std::vector<std::vector<double>> & testX,const std::vector<double> & testY, Perceptron & p);
#endif
