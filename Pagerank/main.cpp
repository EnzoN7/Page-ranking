#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "sparseStruct.hpp"
using namespace std;

void cmdLineRobustness(int nbIterations, float dumpingFactor, ifstream &file, int *nbPages, string fName)
{
    if (nbIterations <= 0) throw 1;
    if (dumpingFactor >= 1 || dumpingFactor <= 0) throw 2;
    if (!file) throw 3;
    file >> *nbPages;
    if (*nbPages < 0) throw 4;

    cout << "# Number of iterations : " << nbIterations << "." << endl;
    cout << "# Dumping factor : " << dumpingFactor << "." << endl;
    cout << "# File : " << ".\\obj\\" + fName << "." << endl;
    cout << "# Number of pages : " << *nbPages << "." << endl << endl;
}

void createAdjacencyMatrix(SparseMatrix *adjMatrix, float dumpingFactor, int nbPages, ifstream &file, bool printMat)
{
    int row, column;
    string line;
    while (getline(file, line))
    {
        file >> row; file >> column;
        if (row >= nbPages || column >= nbPages) throw 6;
        adjMatrix->setValue(row, column);
    }
    cout << "Adjacency matrix : constructed.\n";

    if (printMat)
    {
        ofstream adjacencyMatFile(".\\obj\\[GEN]adjacencyMat.txt");
        if (!adjacencyMatFile) throw 3;
        adjMatrix->print(adjacencyMatFile);
        cout << "Adjacency matrix file : created.\n";
    }
}

void createWeightVector(vector <float> *weight, vector <float> *tempW, SparseMatrix adjMatrix, float dumpingFactor, int nbPages, int nbIterations)
{
    float sum;
    int progress = 0;
    cout << "Weight calculation : ";
    for (int i = 0 ; i < nbIterations ; i++)
    {
        if (i >= progress)
        {
            cout << ". ";
            progress += nbIterations / 10;
        }

        sum = 0;
        *tempW = *weight;
        *weight = adjMatrix * (*weight);

        for (int j = 0 ; j < nbPages ; j++)
            sum += (float) (1 - dumpingFactor) / nbPages * (*tempW)[j];

        for (int k = 0 ; k < nbPages ; k++)
            (*weight)[k] += sum;
    }
    cout << "completed !\n";

    *tempW = *weight;
    sort(weight->begin(), weight->end());
    cout << "Weight : sorted.\n";
}

void createFiles(vector <float> weight, vector <float> tempW, int nbPages)
{
    ofstream weightFile(".\\obj\\[GEN]weight.txt");
    ofstream pagerankFile(".\\obj\\[GEN]pagerank.txt");
    if (!pagerankFile || !weightFile) throw 3;
    for (int i = 0 ; i < nbPages ; i++)
    {
        for (int j = 0 ; j < nbPages ; j++)
            if (weight[nbPages - 1 - i] == tempW[j])
                pagerankFile << j << endl;
        weightFile << weight[nbPages - 1 - i] << endl;
    }
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

        string fName(argv[argc - 1]); ifstream file(".\\obj\\" + fName); int nbPages = -1;
        cmdLineRobustness(nbIterations, dumpingFactor, file, &nbPages, fName);

        SparseMatrix adjMatrix = SparseMatrix(nbPages, dumpingFactor);
        vector <float> weight(nbPages, (float) 1 / nbPages);
        vector <float> tempW(nbPages);

        createAdjacencyMatrix(&adjMatrix, dumpingFactor, nbPages, file, printMat);
        createWeightVector(&weight, &tempW, adjMatrix, dumpingFactor, nbPages, nbIterations);
        createFiles(weight, tempW, nbPages);
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