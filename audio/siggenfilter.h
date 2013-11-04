#ifndef SIGGENFILTER_H
#define SIGGENFILTER_H

#include "buffer.h"
#include "filter.h"

class SigGenFilter: public Filter {
    float freq;
    double omega, domega;
public:
    SigGenFilter(Buffer &b);
    ~SigGenFilter();
    void setFreq(float f);
    virtual void process();
};


#endif

