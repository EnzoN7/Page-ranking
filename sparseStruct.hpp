#ifndef SSTRUCT_HPP
#define SSTRUCT_HPP

#include <vector>
#include <set>
#include <memory>
#include <fstream>


class SparseVector
{
    private:
        std::set<int> columns;

    public:
        const std::set<int>& getColumns() const;
        void setValue(int column);
};

class SparseMatrix
{
    private:
        int nbRows;
        float dumpingFactor;
        std::unique_ptr<SparseVector[]> matrix;

    public:
        SparseMatrix(int nbRows, float dumpingFactor);
        std::vector<float> operator*(const std::vector<float>& rowMat);
        void setValue(int row, int column);
        void print(std::ofstream &adjacencyMatFile);
};

#endif
