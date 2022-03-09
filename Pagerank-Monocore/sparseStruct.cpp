#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "sparseStruct.hpp"
using namespace std;

/**
 * Body of Class SparseVector.
 */

vector <int> SparseVector::getColumns()
{
    return this->columns;
}

void SparseVector::setValue(int column)
{
    int size = this->columns.size();
    if (size == 0)
        this->columns.push_back(column);
    else for (int i = 0 ; i < size ; i++)
    {
        if (column == this->columns[i])
            break;
        else if (column < this->columns[0])
        {
            this->columns.insert(this->columns.begin(), column);
            break;
        }
        else if (i != size - 1 && this->columns[i] < column && column < this->columns[i + 1])
        {
            this->columns.insert(this->columns.begin() + i + 1, column);
            break;
        }
        else if (i == size - 1 || column > this->columns[size - 1])
        {
            this->columns.push_back(column);
            break;
        }
    }
}

/**
 * Body of Class SparseMatrix.
 */

SparseMatrix::SparseMatrix(int nbRows, float dumpingFactor)
{
    this->nbRows = nbRows;
    this->dumpingFactor = dumpingFactor;
    this->matrix = new SparseVector[nbRows];
}

vector <float> SparseMatrix::operator*(vector <float> rowMat)
{
    vector <float> result(this->nbRows);
    float inc;
    for (int i = 0 ; i < this->nbRows ; i++)
    {
        inc = rowMat[i] * this->dumpingFactor;
        if (this->matrix[i].getColumns().size() == 0)
        {
            inc /= this->nbRows;
            for (int j = 0 ; j < this->nbRows ; j++)
                result[j] += inc;
        }
        else
        {
            inc /= this->matrix[i].getColumns().size();
            for (int j = 0 ; j < this->matrix[i].getColumns().size() ; j++)
                result[this->matrix[i].getColumns()[j]] += inc;
        }
    }
    return result;
}

void SparseMatrix::setValue(int row, int column)
{
    this->matrix[row].setValue(column);
}

void SparseMatrix::print(ofstream &adjacencyMatFile)
{
    adjacencyMatFile << "G =\n[" << endl;
    for (int i = 0 ; i < this->nbRows ; i++)
    {
        adjacencyMatFile << "   " << i << " :: ";
        if (this->matrix[i].getColumns().size() == 0)
            adjacencyMatFile << "NULL";
        else for (int j = 0 ; j < this->matrix[i].getColumns().size() ; j++)
        {
            adjacencyMatFile << "(" << (float) 1 / this->matrix[i].getColumns().size() << " : ";
            adjacencyMatFile << this->matrix[i].getColumns()[j] << ") ";
        }
        adjacencyMatFile << endl;
    }
    adjacencyMatFile << "]";
}