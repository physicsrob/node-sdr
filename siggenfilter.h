#ifndef SIGGENFILTER_H
#define SIGGENFILTER_H

#include "buffer.h"
#include "filter.h"

class SigGenFilter: public Filter {
    float freq;
    int sampleNum;
public:
    SigGenFilter(Buffer &b);
    ~SigGenFilter();
    void setFreq(float f) { freq=f; }
    virtual void process();
};


#endif

