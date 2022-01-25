#ifndef SSTRUCT_HPP
#define SSTRUCT_HPP
#include <vector>
using namespace std;

class SparseVector
{
    private:
        vector <int> columns;

    public:
        vector <int> getColumns();

        /**
         * Do : vector(column) <- 1.
         */
        void setValue(int column);
};

class SparseMatrix
{
    private:
        /** Maximum size of the matrix. */
        int nbRows;
        /** Useful for defining multiplication. */
        float dumpingFactor;
        /** A sparse matrix is list of sparse vectors. */
        SparseVector* matrix;

    public:
        /**
         * Create a sparse matrix of dimensions nbRows^2.
         */
        SparseMatrix(int nbRows, float dumpingFactor);

        /**
         * Return : rowMat * matrix.
         */
        vector <float> operator*(vector <float> rowMat);

        /**
         * Do : matrix(row, column) <- 1.
         */
        void setValue(int row, int column);

        /**
         * Print the adjacency matrix into a .txt file.
         */
        void print(ofstream &adjacencyMatFile);
};

#endif