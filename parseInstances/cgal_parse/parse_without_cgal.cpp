// Compile with:  g++ -o parse_simple -std=c++17 -lgmp -lmpfr parse_without_cgal.cpp

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
    // std::cout << "Point 1: " << start1[0] << ", " << start1[1] << std::endl;
    // std::cout << "Point 2: " << end1[0] << ", " << end1[1] << std::endl;
    // std::cout << "Point 3: " << start2[0] << ", " << start2[1] << std::endl;
    // std::cout << "Point 4: " << end2[0] << ", " << end2[1] << std::endl;
    // std::cout << "point 3 area:" << Area2(start1, end1, start2) << std::endl;
    // std::cout << "point 4 area:" << Area2(start1, end1, end2) << std::endl;
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
    if (((c[Y] - a[Y]) != 0 && (
        (b[X] - a[X]) + 1 > std::numeric_limits<long long>::max()/abs((c[Y] - a[Y])) ||
        (b[X] - a[X]) - 1 < std::numeric_limits<long long>::min()/abs((c[Y] - a[Y])))) ||
        ((b[Y] - a[Y]) != 0 && (
        (c[X] - a[X]) + 1 > std::numeric_limits<long long>::max()/abs((b[Y] - a[Y])) ||
        (c[X] - a[X]) - 1 < std::numeric_limits<long long>::min()/abs((b[Y] - a[Y]))))) {
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

// Computing intersection points among curves using the surface-sweep alg.
#include <list>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>

int main()
{

    // Point s_1 = {16116660,11847197};
    // Point t_1 = {15877110,10993459};
    // Point s_2 = {15345257,12859555};
    // Point t_2 = {15105326,13051193};
    // bool doesInt = intersects(s_1, t_1, s_2, t_2);
    // std::cout << doesInt << " (should be false)" << std::endl;


    // Filename parameters
    std::string completed[] = {"reecn6910", "vispecn5478", "rvisp3499", "reecn3382", "reecn3988"};
    
    std::string instanceName = "rvisp3499";
    std::string fileInput = "../../instances/" + instanceName + ".instance.json";
    std::string fileOutput = "../../intersection_graphs/" + instanceName + "_TEST.txt";

    Point s_1 = {15877110,10993459};
    Point t_1 = {15877110,10993459};
    Point s_2 = {15345257,12859555};
    Point t_2 = {15105326,13051193};
    bool doesInt = intersects(s_1, t_1, s_2, t_2);
    std::cout << doesInt << " (should be false)" << std::endl;

    // Read in and parse instance from JSON file
    std::ifstream instanceStream(fileInput);
    nlohmann::json instanceJSON;
    instanceStream >> instanceJSON;
    std::vector<int> x = instanceJSON["x"].get<std::vector<int>>();
    std::vector<int> y = instanceJSON["y"].get<std::vector<int>>();
    std::vector<int> edges1 = instanceJSON["edge_i"].get<std::vector<int>>();
    std::vector<int> edges2 = instanceJSON["edge_j"].get<std::vector<int>>();

    // Construct a list of segments
    int numSegments = edges1.size();
    std::vector<std::vector<int>> segments(numSegments);
    for (int i = 0; i < numSegments; i++) {
        segments[i] = {x[edges1[i]], y[edges1[i]], x[edges2[i]], y[edges2[i]]};
    }

    // Naive O(n^2) approach to finding intersections:
    //     check every pair and record any intersections found
    long long numEdges = 0;
    std::string output = "";

    for (int i = 0; i < numSegments; i++) {
        std::cout << "Processing " << i << std::endl;

        Point seg1_start = {segments[i][0], segments[i][1]};
        Point seg1_end = {segments[i][2], segments[i][3]};

        for (int j = 0; j < i; j++) {
            Point seg2_start = {segments[j][0], segments[j][1]};
            Point seg2_end = {segments[j][2], segments[j][3]};
            bool curr_intersect = intersects(seg1_start, seg1_end, seg2_start, seg2_end);
            if (curr_intersect) {
                // std::cout << "  Segment1: (" << std::to_string(seg1_start[0]) << ", "
                //                       << std::to_string(seg1_start[1]) << ") and ("
                //                       << std::to_string(seg1_end[0]) << ", "
                //                       << std::to_string(seg1_end[1]) << ")" << std::endl;
                // std::cout << "  Segment2: (" << std::to_string(seg2_start[0]) << ", "
                //                       << std::to_string(seg2_start[1]) << ") and ("
                //                       << std::to_string(seg2_end[0]) << ", "
                //                       << std::to_string(seg2_end[1]) << ")" << std::endl;

                std::cout << "    Lines " << i << " and " << j << " DO intersect." << std::endl;

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

    return 0;
}