%module graph
%{
#include "graph.h"
%}

DirectedGraph* create();
void destroy(DirectedGraph* graph);
DirectedGraph* addVertex(DirectedGraph *graph, const char* v);
DirectedGraph* addEdge(DirectedGraph *graph, const char* v1, const char* v2);
void disp(DirectedGraph *graph);

DirectedGraph* graphUnion(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out);
DirectedGraph* graphIntersection(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out);
DirectedGraph* graphDifference(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out);
DirectedGraph* graphProduct(DirectedGraph *graph_in1, DirectedGraph *graph_in2, DirectedGraph *graph_out);
DirectedGraph* graphComplement(DirectedGraph *graph_in, DirectedGraph *graph_out);
