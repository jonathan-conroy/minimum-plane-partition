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
#include <filesystem>
#include <sstream>
#include <map>

#include "graphe.h"
#include "head.h"
#include "util/gfile.h"

bool kColor(char graphName[80], bool implicitRep, int nbColor, std::string instanceName);
void writeJsonColoring(Head* solver, std::string instanceName, std::string outputFilename);
void solveInstance(std::string instanceName, int startNumColor);

int main(int argc, char **argv) {
    //// USE EXPLICIT INTERSECTION GRAPH ////

    // Read in all instance names from directory
    vector<string> intersectionGraphs = {//"reecn3382",
                                        "rvisp3499",
                                        "sqrp9831",
                                        "reecn3988",
                                        "rvisp5013",
                                        "sqrpecn3020",
                                        "reecn6910",
                                        "rvisp7648",
                                        "sqrpecn3218",
                                        "reecn7847",
                                        "rvisp8404",
                                        "sqrpecn5276",
                                        "reecn9674",
                                        "rvisp8432",
                                        "sqrpecn8508",
                                        "rsqrp4637",
                                        "rvisp9770",
                                        "vispecn2518",
                                        "rsqrp4641",
                                        "rvispecn2615",
                                        "vispecn5478",
                                        "rsqrp7320",
                                        "rvispecn6048",
                                        "vispecn7028",
                                        "rsqrpecn8051",
                                        "sqrp7730"};
    
    map<string, int> startNumMap = {{"rvisp5013", 62},
                                {"reecn3382", 79},
                                {"rvisp7648", 55},
                                {"reecn3988", 77},
                                {"rvisp8404", 59},
                                {"reecn6910", 138},
                                {"sqrp9831", 140},
                                {"reecn7847", 95},
                                {"sqrpecn3020", 135},
                                {"reecn9674", 168},
                                {"sqrpecn3218", 141},
                                {"rvisp3499", 35}};
    // for (const auto &file : filesystem::directory_iterator("../../intersection_graphs/")) {
    //     int extensionLen = string(".instance.txt").length();
    //     string filename = string(file.path().filename());
    //     string instanceName = filename.substr(0, filename.length() - extensionLen);
    //     intersectionGraphs.push_back(instanceName);
    // }

    for (int i = 0; i < intersectionGraphs.size(); i++) {
        cout << "-------------------------------" << endl;
        cout << "    SOLVING instance " << i << " = " << intersectionGraphs[i] << endl;
        int startNumColor;
        if (startNumMap.find(intersectionGraphs[i]) == startNumMap.end()) {
            startNumColor = 175;
            solveInstance(intersectionGraphs[i], startNumColor);
        } else {
            startNumColor = startNumMap.at(intersectionGraphs[i]);
            // solveInstance(intersectionGraphs[i], startNumColor);
        }
        // solveInstance(intersectionGraphs[i], startNumColor);
        cout << "-------------------------------" << endl;
    }


    // std::string instanceNames[] = {"reecn9674"};//{"rvisp3499"}; //{"rvisp3499", "reecn3382", "reecn3988", "vispecn5478"};
    // for (int i = 0; i < sizeof(instanceNames)/sizeof(instanceNames[0]); i++) {
    //     solveInstance(instanceNames[i]);
    // }
    return 0;
}

void solveInstance(std::string instanceName, int startNumColor) {
    std::string fileJSON = "../instances/" + instanceName + ".instance.json";
    std::ifstream inStream(fileJSON);
    nlohmann::json inJSON;
    inStream >> inJSON;

    char graphName[80];
    strcpy(graphName, ("../intersection_graphs/" + instanceName + ".txt").c_str());
    bool implicitRep = false;
    // strcpy(graphName, fileJSON.c_str());
    // bool implicitRep = true;

    int numVert = inJSON["n"].get<int>();
    int numSeg = inJSON["m"].get<int>();

    // int bestColor = numVert;
    // if (numSeg < numVert)
    //     bestColor = numSeg;
    int bestColor = startNumColor;

    std::cout << "Finding " + std::to_string(bestColor) + "-coloring." << std::endl;
    while (kColor(graphName, implicitRep, bestColor, instanceName)) {
        bestColor--;
        std::cout << "Finding " + std::to_string(bestColor) + "-coloring." << std::endl;
    }
}

bool kColor(char graphName[80], bool implicitRep, int nbColor, std::string instanceName) {
    std::cout << "Entering kColor" << std::endl;
    Graph g(graphName, implicitRep);
    std::string outputFilename = "../output/" + instanceName + "_HEAD_"  + std::to_string(nbColor) + ".json";
    std::string oldFilename = "../output/" + instanceName + "_HEAD_"  + std::to_string(nbColor + 1) + ".json";
    int nbRun = 3;

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
            if (outputFilename.length() != 0) {
                writeJsonColoring(solver, instanceName, outputFilename);
                std::remove(oldFilename.c_str());
            }
            return true;
        }
    }
    return false;

}

void writeJsonColoring(Head* solver, std::string instanceName, std::string outputFilename) {
    nlohmann::json output;

    // Record colors
    nlohmann::json colorArr = nlohmann::json::array();
    for (int i = 0; i < solver->graph->nbSommets; i++) {
        colorArr.push_back(solver->bestSol.tColor[i]);
    }
    output["colors"] = colorArr;

    // Record metadata
    output["num_colors"] = solver->nbColors;
    output["instance"] = instanceName.c_str();
    output["type"] = "Solution_CGSHOP2022";

    std::ofstream outputFile(outputFilename);
    outputFile << output;
}