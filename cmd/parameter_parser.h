#pragma once

#include <string>
#include <map>

using namespace std;

class ParameterParser {
    public:
    double mutationIndividual;
    double mutationGene;

    double mutationGlobalRate;
    double mutationGlobalLikeness;

    double crossoverElite;
    double crossoverNewRandom;

    double parentsPercentage;
    double tournamentSizePercentage;
    double elitismPercentage;

    int threadNumber;
    double maxDuration;
    int printGenerations;

    private:
    map<string, pair<string, void*>> parameterMap;

    public:
    ParameterParser();

    void Parse(string path);
};
