CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS = Calculator.o DirectedEdge.o DirectedGraph.o Parser.o Vertex.o graph.o main.o
EXEC = gcalc
CPPFLAGS = -std=c++11 -Wall -pedantic-errors -Werror -fPIC

$(EXEC): $(OBJS)
	export LD_LIBRARY_PATH="/usr/local/lib64:${LD_LIBRARY_PATH}"
	$(CXX) $(CPPFLAGS) *.cpp -o $(EXEC)

Calculator.o: Calculator.cpp Calculator.h DirectedGraph.h Vertex.h Exceptions.h DirectedEdge.h
	$(CXX) $(CPPFLAGS) -c $*.cpp
DirectedEdge.o: DirectedEdge.cpp DirectedEdge.h Vertex.h Exceptions.h
	$(CXX) $(CPPFLAGS) -c $*.cpp
DirectedGraph.o: DirectedGraph.cpp DirectedGraph.h Vertex.h Exceptions.h DirectedEdge.h
	$(CXX) $(CPPFLAGS) -c $*.cpp
Parser.o: Parser.cpp Parser.h Calculator.h DirectedGraph.h Vertex.h Exceptions.h DirectedEdge.h Utilities.h
	$(CXX) $(CPPFLAGS) -c $*.cpp
Vertex.o: Vertex.cpp Vertex.h Exceptions.h
	$(CXX) $(CPPFLAGS) -c $*.cpp
graph.o: graph.cpp graph.h DirectedGraph.h Vertex.h Exceptions.h DirectedEdge.h
	$(CXX) $(CPPFLAGS) -c $*.cpp
main.o: main.cpp Parser.h Calculator.h DirectedGraph.h Vertex.h Exceptions.h DirectedEdge.h
	$(CXX) $(CPPFLAGS) -c $*.cpp

libgraph.a: $(OBJS)
	export LD_LIBRARY_PATH="/usr/local/lib64:${LD_LIBRARY_PATH}"
	ar -rs $@ $^

tar: design.pdf Makefile test_in.txt test_out.txt graph.i *.h *.cpp *.gc
	zip gcalc.zip $^

clean:
	rm -f $(OBJS) $(EXEC) libgraph.a