#include "genetic_algorithm.h"
#include <cstring>
#include <cstdio>
#include <ctime>

namespace genetic {
    GeneticAlgorithm::GeneticAlgorithm(Initializer *initalizer,
        FitnessCalculator *fitnessCalculator,
        Selector *selector,
        CrossoverCalculator *crossoverCalculator,
        Mutator *mutator,
        BestIndividualKeeper *bestIndividualKeeper) : initalizer(initalizer), fitnessCalculator(fitnessCalculator),
                            selector(selector), crossoverCalculator(crossoverCalculator), mutator(mutator),
                            bestIndividualKeeper(bestIndividualKeeper) {
            individuals[0] = new IndividualArray();
            individuals[1] = new IndividualArray();
        }

        Individual *GeneticAlgorithm::run(int generationSize, int generationCount, double maxDuration, int printGenerations) {
            double startTime = time(NULL); 

            fitness = new double[generationSize];
            memset(fitness, 0, generationSize * sizeof(fitness));

            initalizer->initialize(generationSize, *individuals[0]);
            initalizer->initialize(generationSize, *individuals[1]);

            vector<pair<int, int>> parents;
            for (int currGen = 0; currGen < generationCount; currGen++) {
                double currTime = time(NULL); 
                if (currTime - startTime > maxDuration) {
                    break;
                }

                IndividualArray *individualsCurrentGen = individuals[currGen % 2];
                IndividualArray *individualsNextGen = individuals[(currGen + 1)% 2];

                fitnessCalculator->calculate(*individualsCurrentGen, fitness);
                parents = selector->select(*individualsCurrentGen, fitness);

                crossoverCalculator->crossover(*individualsCurrentGen, *individualsNextGen, parents);
                mutator->mutate(*individualsCurrentGen);

                if (currGen % printGenerations == 0) {
                    printf("Gen %d: best fitness = %lf\n\n", currGen, bestIndividualKeeper->getFitness());
                }
            }

            return bestIndividualKeeper->getBest();
        }
}
