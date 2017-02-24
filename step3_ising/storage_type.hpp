#pragma once

#include <vector>

// Storage class for 2D spin array.
// Implemented as vector of vectors for simplicity.
class storage_type {
  private:
    std::vector< std::vector<int> > data_;
  public:
    // Constructor
    storage_type(int nrows, int ncols): data_(nrows, std::vector<int>(ncols, 0)) {}

    // Read access
    int operator()(int i, int j) const {
        return data_[i][j];
    }
    // Read/Write access
    int& operator()(int i, int j) {
        return data_[i][j];
    }
};
