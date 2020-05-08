#include "block.h"
#include <vector>

using std::cout;

void Block::Build(PNG& im, int upper, int left, int dimension) {
    for (int i = left; i < left + dimension; i++) {
        vector<HSLAPixel> temp;
        for (int j = upper; j < upper + dimension; j++) {
            HSLAPixel* p = im.getPixel(i,j);
            temp.push_back(*p);
        }
        data.push_back(temp);
    }
}

void Block::Render(PNG& im, int upper, int left) const {
     for (int i = 0; i < Dimension(); i++) {
        for (int j = 0; j < Dimension(); j++) {
            // HSLAPixel* pixel = im.getPixel(left+i, upper+j);
            // *pixel = data[i][j];
            *(im.getPixel(left + i,upper + j)) = data[i][j];
        }
     }
}

int Block::Dimension() const {
    return data[0].size();
}

void Block::Negative() {
    for (int i = 0; i < Dimension(); i++) {
        for (int j = 0; j < Dimension(); j++) {
            data[i][j].h = (int)(data[i][j].h + 180) % 360;
            data[i][j].l = 1 - data[i][j].l;
        }
    }
}
