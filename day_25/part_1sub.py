from networkx import *

graph = Graph()

for line in open("test.txt"):
    left, right = line.split(":")
    for node in right.strip().split():
        graph.add_edge(left, node)
        graph.add_edge(node, left)

graph.remove_edges_from(minimum_edge_cut(graph))
a, b = connected_components(graph)

print(len(a) * len(b))
