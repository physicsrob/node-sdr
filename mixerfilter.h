#ifndef MIXERFILTER_H
#define MIXERFILTER_H

#include "buffer.h"
#include "filter.h"

class MixerFilter : public Filter {
    Buffer &vfo;

public:
    MixerFilter(Buffer &b, Buffer &vfo);
    ~MixerFilter();
    virtual void process();
};


#endif

