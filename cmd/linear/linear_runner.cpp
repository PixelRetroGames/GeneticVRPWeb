#include "../../genetic/genetic_algorithm.h"

#include "../../VRP/linear/linear_crossover_calculator.h"
#include "../../VRP/linear/linear_fitness_calculator.h"
#include "../../VRP/linear/linear_initializer.h"
#include "../../VRP/linear/linear_mutator.h"
#include "../../VRP/linear/linear_selector.h"
#include "../../VRP/VRP_individual.h"
#include "../../VRP/best_individual_keeper.h"

#include "../../VRP/input_parser.h"
#include "../../VRP/solution_writer.h"

#include "../parameter_parser.h"

#include "linear_runner.h"

#include <cstdio>

using namespace VRP;

int runLinear(string inputPath, string solutionPath, string parameterPath) {
    ParameterParser params;
    params.Parse(parameterPath);    

    InputParser inputParser;
    Problem *problem;
    problem = inputParser.Parse(inputPath);

    printf("Entry point\n");

    int numVehicles = problem->vehicleNumber;
    int genomeLength = problem->dimension - 1;

    BestIndividualKeeper keeper(genomeLength);
    LinearInitializer initializer(genomeLength, numVehicles);
    LinearFitnessCalculator fitnessCalculator(problem, numVehicles, &keeper);
    LinearSelector selector(problem->generationSize, params.parentsPercentage, params.tournamentSizePercentage, params.elitismPercentage);
    LinearCrossoverCalculator crossoverCalculator(problem->generationSize, genomeLength, params.crossoverElite, params.crossoverNewRandom, &initializer);
    LinearMutator mutator(genomeLength, params.mutationIndividual, params.mutationGene, 0.5 * (numVehicles - 1.0), params.mutationGlobalRate, params.mutationGlobalLikeness);

    genetic::GeneticAlgorithm algo(&initializer, &fitnessCalculator, &selector, &crossoverCalculator, &mutator, &keeper);
    genetic::Individual *best = algo.run(problem->generationSize, problem->generationCount, params.maxDuration, params.printGenerations);
    VRP::SolutionWriter solWriter;
    solWriter.Write((VRPIndividual*) best, numVehicles, genomeLength, problem, solutionPath);
    return 0;
}

