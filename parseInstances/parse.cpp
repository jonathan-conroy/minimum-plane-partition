// Compile with:  g++ -o parse -std=c++17 parse.cpp

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// Code for intersection detection.
// Inspired by https://stackoverflow.com/a/3842240, which cites Joe O'Rourke's
//     "Computational Geometry in C"

#define X 0
#define Y 1
typedef long long Point[2];
typedef Point Segment[2];
#include<iostream>
#include <limits>
#include <stdexcept>

long long Area2(Point a, Point b, Point c);
bool intersects(Point start1, Point end1, Point start2, Point end2);
bool oppositeSign(long long a, long long b);

bool intersects(Point start1, Point end1, Point start2, Point end2) {
    bool with_seg1_anchor = oppositeSign(Area2(start1, end1, start2), Area2(start1, end1, end2));
    bool with_seg2_anchor = oppositeSign(Area2(start2, end2, start1), Area2(start2, end2, end1));
    return with_seg1_anchor && with_seg2_anchor;
}

bool oppositeSign(long long a, long long b) {
    if (a < 0)
        return (b > 0);
    else if (a > 0)
        return (b < 0);
    else
        // Returns false if either a == 0 or b == 0
        // This disallows improper intersections
        return false;
}

// calculates the triangle's size (formed by the "anchor" segment and additional point)
long long Area2(Point a, Point b, Point c)
{
    // Check for overflow
    // Note: we assume that the vertices are small enough that subtraction will not overflow
    // Note: we add/subtract 1 to avoid thinking about rounding issues
    if (((c[Y] - a[Y]) != 0 && 
        abs((b[X] - a[X])) + 1 > std::numeric_limits<long long>::max()/abs((c[Y] - a[Y]))) ||
        ((b[Y] - a[Y]) != 0 &&
        abs((c[X] - a[X])) + 1 > std::numeric_limits<long long>::max()/abs((b[Y] - a[Y])))) {
            throw std::overflow_error("Overflow with points ("
                                       + std::to_string(a[X]) + ", " + std::to_string(a[Y]) + "); ("
                                       + std::to_string(b[X]) + ", " + std::to_string(b[Y]) + "); ("
                                       + std::to_string(c[X]) + ", " + std::to_string(c[Y]) + ")");
    }
    return (b[X] - a[X]) * (c[Y] - a[Y]) -
           (c[X] - a[X]) * (b[Y] - a[Y]);
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// Computes intersecting segments with naive O(n^2) method

#include <list>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

void constructGraph(std::string instanceName, bool overwrite,
                    std::string instanceFolderPath, std::string outputPath);

int main()
{
    // Parameters
    bool overwrite = false; // Do not regenerate existing files
    std::string instanceFolderPath = "../instances/";
    std::string outputPath = "../intersection_graphs/";

    // Read in all instance names from directory
    std::vector<std::string> instances;
    for (const auto &file : std::filesystem::directory_iterator(instanceFolderPath)) {
        int extensionLen = std::string(".instance.json").length();
        std::string filename = std::string(file.path().filename());
        std::string instanceName = filename.substr(0, filename.length() - extensionLen);
        instances.push_back(instanceName);
    }

    // Construct intersection graph for each instance
    for (int i = 0; i < instances.size(); i++) {
        std::cout << "Instance " << instances[i] << std::endl;
        constructGraph(instances[i], overwrite, instanceFolderPath, outputPath);
    }

    return 0;
}

void constructGraph(std::string instanceName, bool overwrite,
                    std::string instanceFolderPath, std::string outputPath) {
    std::string fileInput = instanceFolderPath + instanceName + ".instance.json";
    std::string fileOutput = outputPath + instanceName + ".txt";

    // If the output file already exists (and "overwrite" is set to false),
    //     exit early
    std::ifstream f(fileOutput.c_str());
    if (f.good()) {
        std::cout << "File " << fileOutput << " already exists --- exiting early." << std::endl;
        return;
    }

    // Read in and parse instance from JSON file
    std::ifstream instanceStream(fileInput);
    nlohmann::json instanceJSON;
    instanceStream >> instanceJSON;
    std::vector<int> x = instanceJSON["x"].get<std::vector<int> >();
    std::vector<int> y = instanceJSON["y"].get<std::vector<int> >();
    std::vector<int> edges1 = instanceJSON["edge_i"].get<std::vector<int> >();
    std::vector<int> edges2 = instanceJSON["edge_j"].get<std::vector<int> >();

    // Construct a list of segments
    int numSegments = edges1.size();
    std::vector<std::vector<int> > segments(numSegments);
    for (int i = 0; i < numSegments; i++) {
        segments[i] = {x[edges1[i]], y[edges1[i]], x[edges2[i]], y[edges2[i]]};
    }

    // Naive O(n^2) approach to finding intersections:
    //     check every pair and record any intersections found
    long long numEdges = 0;
    std::string output = "";

    for (int i = 0; i < numSegments; i++) {
        if (i % 1000 == 0)
            std::cout << "    Processing " << i << std::endl;

        Point seg1_start = {segments[i][0], segments[i][1]};
        Point seg1_end = {segments[i][2], segments[i][3]};

        for (int j = 0; j < i; j++) {
            Point seg2_start = {segments[j][0], segments[j][1]};
            Point seg2_end = {segments[j][2], segments[j][3]};
            bool curr_intersect = intersects(seg1_start, seg1_end, seg2_start, seg2_end);
            if (curr_intersect) {
                output += ("e " + std::to_string(i+1) + " " + std::to_string(j+1) + "\n");
                output += ("e " + std::to_string(j+1) + " " + std::to_string(i+1) + "\n");
                numEdges += 2;
            }
        }
    }

    // Write output to file
    std::cout << "Writing output" << std::endl;
    output = "p edge " + std::to_string(numSegments) + " " + std::to_string(numEdges) + "\n" + output;
    std::ofstream out(fileOutput);
    out << output;


}