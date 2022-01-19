#ifndef GRAPHE_H
#define GRAPHE_H


#include<vector>
#include<string>

#include "util/gfile.h"

using namespace std;

class Graph {
	void buildVoisins(); // cree le vecteur des voisins de chaque sommet

public:
	Graph(){filename=""; nbSommets=0; nbArretes=0; tConnect=NULL; tVoisins=NULL; implicitRep=false;}
	Graph(string fname){filename=fname; loadGraph(); implicitRep=false;}
	Graph(string fname, bool implicit);
	~Graph();
	void loadGraph();
	void loadImplicitGraph();
	void loadMatrixGraph(GInputFile& infile);

	bool isEdge(int i, int j);
	vector<int> getNeighbors(int i);

	int nbSommets; // number of vertices
	int nbArretes; // number of edges
	string filename;
	bool implicitRep;

private:
	char** tConnect; // adjacency matrix
	vector<int>* tVoisins; // adjacency list

	vector<int> x;
	vector<int> y;
	vector<int> seg_start;
	vector<int> seg_end;
};


#endif
