import json
import os
from poly_point_isect import isect_segments_include_segments

INSTANCE_PATH = "instances"

instance_files = os.listdir(INSTANCE_PATH)

filename = "instances/rvisp3499.instance.json"
with open(filename) as f:
    inst = json.load(f)
    print(inst.keys())
    x = inst["x"]
    y = inst["y"]
    edges = zip(inst["edge_i"], inst["edge_j"])
    # print(list(edges))
    segments = [((x[p1]/100000000, y[p1]/100000000), (x[p2]/100000000, y[p2]/100000000)) for (p1, p2) in edges]
    print(segments[0:100])
    print("finding intersections")
    intersections = isect_segments_include_segments(segments[0:100])
    print("found intersections")
    print(intersections)