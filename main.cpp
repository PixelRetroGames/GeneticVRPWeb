#include "cmd/linear/linear_runner.h"

#include "cmd/pthread/pthread_runner.h"

#include "cmd/openmp/openmp_runner.h"

#include "cmd/parameter_parser.h"

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <string>

int main(int argc, char** argv) {
    if(argc != 5) {
        printf("Usage: %s <input_path> <solution_path> <parameters_path> <openmp|pthread|linear>\n", argv[0]);
        return 0;
    }

    std::string inputPath = argv[1];
    std::string solutionPath = argv[2];
    std::string paramPath = argv[3];
    std::string type = argv[4];

    int start = time(NULL);
    
    if(type == "linear") {
       runLinear(inputPath, solutionPath, paramPath);
    } else if(type == "openmp") {
        runOpenMP(inputPath, solutionPath, paramPath);
    } else if(type == "pthread") {
        runPthread(inputPath, solutionPath, paramPath);
    } else {
        printf("Invalid implementation!\n");
    }

    printf("Duration: %d seconds\n", time(NULL) - start);
}
