#include "parameter_parser.h"

ParameterParser::ParameterParser() {
    parameterMap["mutationIndividual"]       = {"double", &mutationIndividual};
    parameterMap["mutationGene"]             = {"double", &mutationGene};
    parameterMap["mutationGlobalRate"]       = {"double", &mutationGlobalRate};
    parameterMap["mutationGlobalLikeness"]   = {"double", &mutationGlobalLikeness};
    parameterMap["crossoverElite"]           = {"double", &crossoverElite};
    parameterMap["crossoverNewRandom"]       = {"double", &crossoverNewRandom};
    parameterMap["parentsPercentage"]        = {"double", &parentsPercentage};
    parameterMap["tournamentSizePercentage"] = {"double", &tournamentSizePercentage};
    parameterMap["elitismPercentage"]        = {"double", &elitismPercentage};
    parameterMap["mutationIndividual"]       = {"double", &mutationIndividual};

    parameterMap["threadNumber"]    = {"int", &threadNumber};
    parameterMap["maxDuration"]     = {"double", &maxDuration};
    parameterMap["printGenerations"]   = {"int", &printGenerations};
}

void ParameterParser::Parse(string path) {
    char buffer[256] = {0};

    FILE *f = fopen(path.c_str(), "r");
    if (f == NULL) {
        fprintf(stderr, "Can't open file %s\n", path.c_str());
        return;
    }

    while (!feof(f)) {
        fscanf(f, "%s ", buffer);
        string name = buffer;
        if (parameterMap[name].first == "int") {
            fscanf(f, "%d ", parameterMap[name].second);
        } else if (parameterMap[name].first == "double") {
            fscanf(f, "%lf ", parameterMap[name].second);
        } else if (parameterMap[name].first == "bool") {
            fscanf(f, "%d ", parameterMap[name].second);
        } else if (parameterMap[name].first == "string") {
            fscanf(f, "%s ", buffer);
            *((string*)parameterMap[name].second) = buffer;
        }
    }

    for (auto &it : parameterMap) {
        if (it.second.first == "int") {
            printf("%s of type %s with value %d\n", it.first.c_str(), it.second.first.c_str(), *((int *)it.second.second));
        } else if (it.second.first == "double") {
            printf("%s of type %s with value %lf\n", it.first.c_str(), it.second.first.c_str(), *((double *)it.second.second));
        } else if (it.second.first == "bool") {
            printf("%s of type %s with value %d\n", it.first.c_str(), it.second.first.c_str(), *((bool *)it.second.second));
        } else if (it.second.first == "string") {
            printf("%s of type %s with value %s\n", it.first.c_str(), it.second.first.c_str(), ((string *)it.second.second)->c_str());
        }
    }
}
