#include<cstring>
#include "graphe.h"

// For intersection detection
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

// For JSON parsing
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>



Graph::Graph(string fname, bool implicit) {

	filename = fname;
	implicitRep = implicit;
	if (implicit) {
		loadImplicitGraph();
	}
	else {
		loadGraph();
	}
	
}


Graph::~Graph(){
	cout << "Free graph" << endl;
	if (!implicitRep) {
		for(int i=0; i<nbSommets; i++){
			delete[] tConnect[i];
		}

		delete[] tConnect;
		delete[] tVoisins;
	}
	cout << "Freed graph (done)" << endl;
}

void Graph::loadImplicitGraph() {
	cout << "Using implicit representation" << endl;
	ifstream instanceStream(filename);
    nlohmann::json instanceJSON;
    instanceStream >> instanceJSON;
    x = instanceJSON["x"].get<vector<int> >();
    y = instanceJSON["y"].get<vector<int> >();
    seg_start = instanceJSON["edge_i"].get<vector<int> >();
    seg_end = instanceJSON["edge_j"].get<vector<int> >();

	nbSommets = instanceJSON["m"].get<int>();
	nbArretes = 1163364; //hardcoded?? hopefully this doesn't matter... TODO: remove nbArretes?
}

void Graph::loadGraph(){
	GInputFile infile(filename);
	infile.open();

	char* buf;
	char* tok;
	int nbArretesAjoutee=0;

	while ( (buf=infile.readUncommentedLine()) ) {
		tok=infile.getNextToken();
                if (tok != NULL){
		if(tok && strcmp(tok, "nbVertices")==0){ // lecture d'une arrete
			loadMatrixGraph(infile);
			infile.close();
			buildVoisins();
			return;
		}


		if(*tok=='p'){ // lecture de la taille du graphe
			tok=infile.getNextToken(); //pas interessant
			tok=infile.getNextToken();
			nbSommets=atoi(tok);
			tok=infile.getNextToken();
			nbArretes=atoi(tok);

			// creation du graphe
			tConnect=new char*[nbSommets];
			for(int i=0; i<nbSommets; i++){
				tConnect[i]=new char[nbSommets];
				for (int j=0; j<nbSommets; j++)
					tConnect[i][j]=0;
			}

			//printf("Sommets ajoutes: %d\n", nbSommets);
		}

		if(*tok=='e'){ // lecture d'une arrete
			tok=infile.getNextToken();
			int v1=atoi(tok);
			tok=infile.getNextToken();
			int v2=atoi(tok);
			tConnect[v1-1][v2-1]=1;
			tConnect[v2-1][v1-1]=1;
			nbArretesAjoutee++;
		}}
	}

	//printf("Arretes ajoutees: %d / %d\n",nbArretesAjoutee ,nbArretes);

	infile.close();

	buildVoisins();
}





void Graph::loadMatrixGraph(GInputFile& infile){
	printf("Debut de chargement d'un graphe matrice haute\n");
	char* tok;
	tok=infile.getNextToken();
    nbSommets=atoi(tok);
    nbArretes=0;
    printf("Sommets ajoutes: %d\n", nbSommets);

    // creation du graphe
    // creation du graphe

    tConnect=new char*[nbSommets];
    for(int i=0; i<nbSommets; i++){
        tConnect[i]=new char[nbSommets];
    }

    for(int i=0; i<nbSommets; i++){
        infile.readUncommentedLine();
    	for(int j=0; j<nbSommets; j++){
			tok=infile.getNextToken(' ');
			tConnect[i][j]=atoi(tok);
			if(tConnect[i][j])
                nbArretes++;
		}
	}
	printf("Arretes ajoutees: %d\n",nbArretes);
}




void Graph::buildVoisins(){
	tVoisins=new vector<int>[nbSommets];
	for (int i=0; i<nbSommets; i++) {
		for (int j=0; j<nbSommets; j++) {
			if(isEdge(i, j))
				tVoisins[i].push_back(j);
		}
	}
}

bool Graph::isEdge(int i, int j) {
	if (implicitRep) {
		Point i_start = {x[seg_start[i]], y[seg_start[i]]};
		Point i_end   = {x[seg_end[i]],   y[seg_end[i]]};
		Point j_start = {x[seg_start[j]], y[seg_start[j]]};
		Point j_end   = {x[seg_end[j]],   y[seg_end[j]]};
		bool isNeighbor = intersects(i_start, i_end, j_start, j_end);
		return isNeighbor;
	} else {
		return tConnect[i][j];
	}
}

vector<int> Graph::getNeighbors(int i) {
	if (implicitRep) {
		vector<int> neighbors;

		Point i_start = {x[seg_start[i]], y[seg_start[i]]};
		Point i_end   = {x[seg_end[i]],   y[seg_end[i]]};

		for (int j = 0; j < nbSommets; j++) {
			Point j_start = {x[seg_start[j]], y[seg_start[j]]};
			Point j_end = {x[seg_end[j]],   y[seg_end[j]]};
			bool isNeighbor = intersects(i_start, i_end, j_start, j_end);
			if (isNeighbor) {
				neighbors.push_back(j);
			}
		}
		return neighbors;
	} else {
		return tVoisins[i];
	}
}

// Code for intersection detection.
// Copy-pasted from parseInstances/parse.cpp. TODO: Refactor this.
// Inspired by https://stackoverflow.com/a/3842240, which cites Joe O'Rourke's
//     "Computational Geometry in C"

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
