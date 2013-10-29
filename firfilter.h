#ifndef FIRFILTER_H
#define FIRFILTER_H

#include "buffer.h"
#include "filter.h"

class FIRFilter : public Filter {
    float *FIR;
    int len; // Length of FIR filter
    int center;
public:
    FIRFilter(Buffer &b);
    ~FIRFilter();
    void setSincFIR(float B, int l);
    void print();
    void windowHamming();
    void windowBlackman();
    void shiftFilter(float F);
    virtual void process();
};


#endif

