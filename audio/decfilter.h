#ifndef DECFILTER_H
#define DECFILTER_H

#include "buffer.h"
#include "filter.h"

class DecFilter: public Filter {
    int ratio;
public:
    DecFilter(Buffer &b, int ratio);
    ~DecFilter();
    virtual void process();
};


#endif

