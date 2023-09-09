#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <memory>
#include "sparseStruct.hpp"
using namespace std;

void cmdLineRobustness(int nbIterations, float dumpingFactor, ifstream &file, int &nbPages, const string &fName)
{
    if (nbIterations <= 0) throw 1;
    if (dumpingFactor >= 1 || dumpingFactor <= 0) throw 2;
    if (!file) throw 3;
    file >> nbPages;
    if (nbPages < 0) throw 4;

    cout << "# Number of iterations : " << nbIterations << "." << endl;
    cout << "# Dumping factor : " << dumpingFactor << "." << endl;
    cout << "# File : " << "./obj/" + fName << "." << endl;
    cout << "# Number of pages : " << nbPages << "." << endl << endl;
}

void createAdjacencyMatrix(SparseMatrix &adjacencyMat, float dumpingFactor, int nbPages, ifstream &file, bool printMat)
{
    int row, column;
    string line;
    while (file >> row >> column)
    {
        if (row >= nbPages || column >= nbPages) throw 6;
        adjacencyMat.setValue(row, column);
    }
    cout << "Adjacency matrix : constructed.\n";

    if (printMat)
    {
        ofstream adjacencyMatFile(".//obj//[GEN]adjacencyMat.txt");
        if (!adjacencyMatFile) throw 3;
        adjacencyMat.print(adjacencyMatFile);
        cout << "Adjacency matrix file : created.\n";
    }
}

void createWeightVector(vector<float> &weight, vector<float> &tempW, SparseMatrix &adjacencyMat, float dumpingFactor, int nbPages, int nbIterations)
{
    float sum;
    cout << "Weight calculation : ";
    for (int i = 0 ; i < nbIterations ; i++)
    {
        sum = 0;
        tempW = weight;
        weight = adjacencyMat * weight;

        #pragma omp parallel for reduction(+ : sum)
        for (int j = 0 ; j < nbPages ; j++)
            sum += (float) (1 - dumpingFactor) / nbPages * tempW[j];

        for (int k = 0 ; k < nbPages ; k++)
            weight[k] += sum;
    }
    cout << "completed.\n";

    tempW = move(weight);
    sort(weight.begin(), weight.end());
    cout << "Weight : sorted.\n";
}

void createFiles(const vector<float> &weight, const vector<float> &tempW, int nbPages)
{
    ofstream weightFile(".//obj//[GEN]weight.txt");
    ofstream pagerankFile(".//obj//[GEN]pagerank.txt");
    
    vector<pair<float, int>> indexedWeight;
    indexedWeight.reserve(nbPages);

    for (int i = 0; i < nbPages; ++i) {
        indexedWeight.emplace_back(tempW[i], i);
    }
    
    sort(indexedWeight.begin(), indexedWeight.end(), 
         [](const pair<float, int>& a, const pair<float, int>& b) -> bool {
             return a.first > b.first; 
         });

    for (int i = 0; i < nbPages; ++i) {
        pagerankFile << indexedWeight[i].second << endl;
        weightFile << indexedWeight[i].first << endl;
    }

    weightFile.close();
    pagerankFile.close();
    
    cout << "Weight file & pagerank file  : created.\n";
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2) throw 5;

        int nbIterations = 50; float dumpingFactor = 0.5; bool printMat = false;
        for (int i = 1 ; i < argc - 1 ; i++)
        {
            if (strcmp(argv[i], "-I") == 0)
                nbIterations = atoi(argv[i + 1]);
            else if (strcmp(argv[i], "-D") == 0)
                dumpingFactor = strtof(argv[i + 1], NULL);
            else if (strcmp(argv[i], "-P") == 0)
                printMat = true;
        }

        string fName(argv[argc - 1]); ifstream file(".//obj//" + fName); int nbPages = -1;
        cmdLineRobustness(nbIterations, dumpingFactor, file, nbPages, fName);

        SparseMatrix adjacencyMat(nbPages, dumpingFactor);
        vector <float> weight(nbPages, (float) 1 / nbPages);
        vector <float> tempW(nbPages);
        tempW.reserve(nbPages);

        createAdjacencyMatrix(adjacencyMat, dumpingFactor, nbPages, file, printMat);
        createWeightVector(weight, tempW, adjacencyMat, dumpingFactor, nbPages, nbIterations);
        createFiles(weight, move(tempW), nbPages);

        file.close();
    }
    catch (int x)
    {
        cout << "Error" << " :: ";
        switch (x)
        {
            case 1: cout << "Wrong number of iterations (> 0)." << endl; break;
            case 2: cout << "Wrong dumping factor (> 0 && < 1)." << endl; break;
            case 3: cout << "Unopened file." << endl; break;
            case 4: cout << "Wrong number of pages, check file format." << endl; break;
            case 5: cout << "Wrong number of arguments." << endl; break;
            case 6: cout << "Inconsistent data, check file." << endl;
        }
    };
    return 0;
}
