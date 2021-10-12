#ifndef _RINGS_H_
#define _RINGS_H_

namespace Rings {
    
    struct Row {
        const uint8_t* indices;
        uint8_t numIndices;
    };

    extern const int numRows;
    extern const Row* getRow(uint8_t index);
}

#endif
