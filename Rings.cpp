#include <Arduino.h>
#include "Rings.h"

namespace Rings {

    // Split up the rings into 9 "rows".
    // The top and bottom row each contain the 
    // top and bottom 5 pixels, while the rest
    // contain the pixels on the right and left
    // side for that row. Also, rows 1-5 correspond
    // with rows 0-4 on the glasses matrix.
    const uint8_t rowIndices0[] = {22, 23, 0, 1, 2};
    const uint8_t rowIndices1[] = {21, 3};
    const uint8_t rowIndices2[] = {20, 4};
    const uint8_t rowIndices3[] = {19, 5};
    const uint8_t rowIndices4[] = {18, 6};
    const uint8_t rowIndices5[] = {17, 7};
    const uint8_t rowIndices6[] = {16, 8};
    const uint8_t rowIndices7[] = {15, 9};
    const uint8_t rowIndices8[] = {14, 13, 12, 11, 10};

    const Row ringRows[] = {
        { rowIndices0, sizeof(rowIndices0) },
        { rowIndices1, sizeof(rowIndices1) },     // row 0 on matrix
        { rowIndices2, sizeof(rowIndices2) },     // row 1 on matrix
        { rowIndices3, sizeof(rowIndices3) },     // row 2 on matrix
        { rowIndices4, sizeof(rowIndices4) },     // row 3 on matrix
        { rowIndices5, sizeof(rowIndices5) },     // row 4 on matrix
        { rowIndices6, sizeof(rowIndices6) },
        { rowIndices7, sizeof(rowIndices7) },
        { rowIndices8, sizeof(rowIndices8) },
    };

    const int numRows = (sizeof(ringRows) / sizeof(Row));

    // A "dummy" row. This way, if we accidentally try to get
    // an invalid row, we won't crash when trying to draw it.
    const uint8_t invaldRowIndices[] = {24};
    const Row invalidRow = {invaldRowIndices, sizeof(invaldRowIndices)};

    const Row* getRow(uint8_t index) {
        if (index >= numRows) {
            return &invalidRow;
        }

        return &ringRows[index];
    }
}
