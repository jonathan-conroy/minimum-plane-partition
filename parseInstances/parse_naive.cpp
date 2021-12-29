// Compile with:  g++ -o parse -std=c++17 -lgmp -lmpfr parse_naive.cpp


// Computing intersection points among curves using the surface-sweep alg.
#include <list>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Surface_sweep_2_algorithms.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef Kernel::Point_2                                         Point_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;
typedef Traits_2::Curve_2                                       Segment_2;
int main()
{

    // Filename parameters
    std::string completed[] = {"rvisp3499", "reecn3382", "reecn3988"};
    
    std::string instanceName = "vispecn5478";
    std::string fileInput = "../instances/" + instanceName + ".instance.json";
    std::string fileOutput = "../intersection_graphs/" + instanceName + ".txt";

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
    std::vector<Segment_2> segments(numSegments);
    for (int i = 0; i < numSegments; i++) {
        segments[i] = Segment_2(Point_2(x[edges1[i]], y[edges1[i]]),
                                Point_2(x[edges2[i]], y[edges2[i]]));
    }

    // Naive O(n^2) approach to finding intersections:
    //     check every pair and record any intersections found
    int numEdges = 0;
    std::string output = "";

    for (int i = 0; i < numSegments; i++) {
        std::cout << "Processing " << i << std::endl;
        for (int j = 0; j < i; j++) {
            Segment_2 curr_pair[] = {segments[i], segments[j]};
            bool curr_intersect = CGAL::do_curves_intersect (curr_pair, curr_pair + 2);
            if (curr_intersect) {
                // std::cout << "Lines " << i << " and " << j << " DO intersect." << std::endl;
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