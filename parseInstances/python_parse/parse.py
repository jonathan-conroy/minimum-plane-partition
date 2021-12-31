import json
import os
from ground.base import get_context
from bentley_ottmann.planar import segments_intersections

INSTANCE_PATH = "instances"

context = get_context()
Point, Segment = context.point_cls, context.segment_cls
instance_files = os.listdir(INSTANCE_PATH)

filename = "instances/rvisp3499.instance.json"
with open(filename) as f:
    inst = json.load(f)
    print(inst.keys())
    x = inst["x"]
    y = inst["y"]
    edges = zip(inst["edge_i"], inst["edge_j"])
    # print(list(edges))
    segments = [Segment(Point(x[p1], y[p1]), Point(x[p2], y[p2])) for (p1, p2) in edges]
    print(len(segments))
    print("finding intersections")
    intersections = segments_intersections(segments[0:100])
    print("found intersections")
    print(intersections)