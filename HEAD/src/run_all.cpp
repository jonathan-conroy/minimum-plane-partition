#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <iomanip>
#include <getopt.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

#include "graphe.h"
#include "head.h"
#include "util/gfile.h"

bool kColor(char graphName[80], int nbColor, std::string instanceName);
void solveInstance(std::string instanceName);

int main(int argc, char **argv) {
    std::string instanceNames[] = {"rvisp3499"}; //{"rvisp3499", "reecn3382", "reecn3988", "vispecn5478"};
    for (int i = 0; i < sizeof(instanceNames)/sizeof(instanceNames[0]); i++) {
        solveInstance(instanceNames[i]);
    }
    return 0;
}

void solveInstance(std::string instanceName) {
    std::string fileJSON = "../instances/" + instanceName + ".instance.json";
    std::ifstream inStream(fileJSON);
    nlohmann::json inJSON;
    inStream >> inJSON;

    char graphName[80];
    strcpy(graphName, ("../intersection_graphs/" + instanceName + ".txt").c_str());
    // Graph g(graphName);

    int numVert = inJSON["n"].get<int>();
    int numSeg = inJSON["m"].get<int>();

    int bestColor = numVert;
    if (numSeg < numVert)
        bestColor = numSeg;
    bestColor = 35;

    std::cout << "Finding " + std::to_string(bestColor) + "-coloring." << std::endl;
    while (kColor(graphName, bestColor, instanceName)) {
        bestColor--;
        std::cout << "Finding " + std::to_string(bestColor) + "-coloring." << std::endl;
    }
}

bool kColor(char graphName[80], int nbColor, std::string instanceName) {
    std::cout << "Entering kColor" << std::endl;
    Graph g(graphName);
    char outputFile[255];
    strcpy(outputFile, ("../output/" + instanceName + "_HEAD_"  + std::to_string(nbColor) + ".txt").c_str());
    char previousOutputFile[255];
    strcpy(previousOutputFile, ("../output/" + instanceName + "_HEAD_"  + std::to_string(nbColor + 1) + ".txt").c_str());
    int nbRun = 10;

    long long nbLocalSearch = 30000;
    int nbGeneration = -1;
    int randSeed = 0;
    int nbRandCross = 0;
    int swapIter = -1;
    double swapingRate = 99.0;
    bool quite = false;
    bool tabucol = false;
    double tauxAcceptWorst = 1.0;
    double weightParent = -1;
    int maxseconds = -1;

    double totalCpuTime=0;
    double totalHumanTime=0;
    unsigned long long totalIterations=0;
    unsigned long long totalIterationsCross=0;
    unsigned long long totalIterationsCrossWithWrongRun=0;
    int nbFound=0;
    Head* solver=new Head();

    // >> Solver Parameters
    solver->graph=&g;
    solver->nbColors=nbColor;
    solver->nbLocalSearch=nbLocalSearch;
    solver->nbGeneration=nbGeneration;
    solver->userRandSeed=randSeed;
    solver->nbRandCross=nbRandCross;
    solver->swapIter=swapIter;
    solver->swapingRate=swapingRate/100.0;
    solver->debug=!quite;
    solver->tauxAcceptWorst=tauxAcceptWorst;
    solver->weightParent=weightParent/100.0;
    solver->initSol = NULL;
    solver->maxsecondes=maxseconds;
    solver->tabucol = tabucol;
    // << solver parameters

    
    // Attempt to solve
    int runId;
    for (runId=0; (runId<nbRun) || ((nbRun==-1) && (nbFound==0)) ; runId++) {
        std::cout << "    Attempting to solve (run " << std::to_string(runId) << ")" << std::endl;
        solver->compute();
    
        // Save successful coloring
        if (solver->bestSol.nbEdgesConflict==0) {
            std::cout << "    Attempting to save coloring (run " << std::to_string(runId) << ")" << std::endl;
            if (strlen(outputFile) != 0)
                solver->saveBestColoring(outputFile);

            // TODO: if previousOutputFile exists, remove it
            return true;
            
        }
    }
    return false;

}