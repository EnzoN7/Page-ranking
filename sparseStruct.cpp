#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <set>
#include "sparseStruct.hpp"


const std::set<int>& SparseVector::getColumns() const
{
    return this->columns;
}

void SparseVector::setValue(int column)
{
    this->columns.insert(column);
}

SparseMatrix::SparseMatrix(int nbRows, float dumpingFactor)
    : nbRows(nbRows), dumpingFactor(dumpingFactor), matrix(new SparseVector[nbRows]) {}

std::vector<float> SparseMatrix::operator*(const std::vector<float> &rowMat)
{
    std::vector<float> result(this->nbRows, 0.0f);
    int i, j;

    #pragma omp parallel for private(j)
    for (i = 0; i < this->nbRows; i++)
    {
        float inc = rowMat[i] * this->dumpingFactor;
        const std::set<int> &columns = this->matrix[i].getColumns();

        if (columns.empty())
        {
            inc /= this->nbRows;
            #pragma omp critical
            for (j = 0; j < this->nbRows; j++)
                result[j] += inc;
        }
        else
        {
            inc /= columns.size();
            for (const int& col : columns)
            {
                #pragma omp atomic
                result[col] += inc;
            }
        }
    }

    return result;
}

void SparseMatrix::setValue(int row, int column)
{
    this->matrix[row].setValue(column);
}

void SparseMatrix::print(std::ofstream &adjacencyMatFile)
{
    adjacencyMatFile << "G =\n[" << std::endl;
    for (int i = 0 ; i < this->nbRows ; i++)
    {
        const std::set<int>& columns = this->matrix[i].getColumns();
        adjacencyMatFile << "   " << i << " :: ";
        
        if (columns.empty())
            adjacencyMatFile << "NULL";
        else
            for (const int& col : columns) 
                adjacencyMatFile << "(" << (float) 1 / columns.size() << " : " << col << ") ";
        adjacencyMatFile << std::endl;
    }
    adjacencyMatFile << "]";
}
