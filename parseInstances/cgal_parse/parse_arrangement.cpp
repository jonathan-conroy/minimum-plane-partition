// Compile with:  g++ -o parse_arr -std=c++17 -lgmp -lmpfr parse_arrangement.cpp

// Constructing an arrangement with curve history.
#include <CGAL/Cartesian.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_on_surface_with_history_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_default_overlay_traits.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>

typedef CGAL::Cartesian<CGAL::Exact_rational>             Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                Traits_2;
typedef Traits_2::Point_2                                 Point_2;
typedef Traits_2::Curve_2                                 Segment_2;
typedef CGAL::Arrangement_with_history_2<Traits_2>        Arr_with_hist_2;
typedef Arr_with_hist_2::Curve_handle                     Curve_handle;
typedef CGAL::Arr_default_overlay_traits<Arr_with_hist_2> Overlay_traits;

Arr_with_hist_2 recursivelyInsert(std::vector<Segment_2> segments);

int main()
{
    std::string instanceName = "reecn6910";
    std::string fileInput = "../../instances/" + instanceName + ".instance.json";
    std::string fileOutput = "../../intersection_graphs/" + instanceName + ".txt";

    // Read in and parse instance from JSON file
    std::ifstream instanceStream(fileInput);
    nlohmann::json instanceJSON;
    instanceStream >> instanceJSON;
    std::vector<int> x = instanceJSON["x"].get<std::vector<int>>();
    std::vector<int> y = instanceJSON["y"].get<std::vector<int>>();
    std::vector<int> edges1 = instanceJSON["edge_i"].get<std::vector<int>>();
    std::vector<int> edges2 = instanceJSON["edge_j"].get<std::vector<int>>();

    int numSegments = edges1.size();
    std::vector<Segment_2> segments(numSegments);
    for (int i = 0; i < numSegments; i++) {
        segments[i] = Segment_2(Point_2(x[edges1[i]], y[edges1[i]]),
                                Point_2(x[edges2[i]], y[edges2[i]]));
        std::cout << i << std::endl;
        // insert(arr, segments[i]);
                    
    }


    Arr_with_hist_2 arr = recursivelyInsert(segments);
    std::cout << "Done constructing arrangement" << std::endl;
    std::cout << "Arrangement has " << std::to_string(arr.number_of_edges()) << " edges." << std::endl;

//   // Insert s1, s2 and s3 incrementally:
//   Segment_2 s1(Point_2(0, 3), Point_2(4, 3));
//   insert(arr, s1);
//   Segment_2 s2(Point_2(3, 2), Point_2(3, 5));
//   insert(arr, s2);
//   Segment_2 s3(Point_2(2, 3), Point_2(5, 3));
//   insert(arr, s3);
//   // Insert three additional segments aggregately:
//   Segment_2 segs[3];
//   segs[0] = Segment_2(Point_2(2, 6), Point_2(7, 1));
//   segs[1] = Segment_2(Point_2(0, 0), Point_2(2, 6));
//   segs[2] = Segment_2(Point_2(3, 4), Point_2(6, 4));
//   insert(arr, segs, segs + 3);
//   // Print out the curves and the number of edges each one induces.
//   Arr_with_hist_2::Curve_iterator            cit;
//   std::cout << "The arrangement contains "
//             << arr.number_of_curves() << " curves:" << std::endl;
//   for (cit = arr.curves_begin(); cit != arr.curves_end(); ++cit)
//     std::cout << "Curve [" << *cit << "] induces "
//               << arr.number_of_induced_edges(cit) << " edges." << std::endl;
//   // Print the arrangement edges, along with the list of curves that
//   // induce each edge.
//   Arr_with_hist_2::Edge_iterator                  eit;
//   Arr_with_hist_2::Originating_curve_iterator     ocit;
//   std::cout << "The arrangement is comprised of "
//             << arr.number_of_edges() << " edges:" << std::endl;
//   for (eit = arr.edges_begin(); eit != arr.edges_end(); ++eit) {
//     std::cout << "[" << eit->curve() << "]. Originating curves: ";
//     for (ocit = arr.originating_curves_begin(eit);
//          ocit != arr.originating_curves_end(eit); ++ocit)
//       std::cout << " [" << *ocit << "]" << std::flush;
//     std::cout << std::endl;
//   }

  return 0;
}

Arr_with_hist_2 recursivelyInsert(std::vector<Segment_2> segments) {
    if (segments.size() == 0) {
        Arr_with_hist_2 empty_arr;
        return empty_arr;
    } else if (segments.size() == 1) {
        Arr_with_hist_2 arr;
        insert(arr, segments[0]);
        return arr;
    } else {
        int half = floor(segments.size() / 2);
        std::vector<Segment_2> segments1 = std::vector<Segment_2>(segments.begin(),
                                                                  segments.begin() + half);
        std::vector<Segment_2> segments2 = std::vector<Segment_2>(segments.begin() + half,
                                                                  segments.end());
        // for (int i = 0; i < segments1.size(); i++)
        //     std::cout << segments1[i] << std::endl;
        // std::cout << "------" << std::endl;
        // for (int i = 0; i < segments1.size(); i++)
        //     std::cout << segments2[i] << std::endl;
        Arr_with_hist_2          overlay_arr;
        Overlay_traits           overlay_traits;
        Arr_with_hist_2 arr1 = recursivelyInsert(segments1);
        Arr_with_hist_2 arr2 = recursivelyInsert(segments2);
        overlay(arr1, arr2, overlay_arr, overlay_traits);
        std::cout << "Merged array of size " << std::to_string(segments.size()) << std::endl;
        return overlay_arr;
    }
}