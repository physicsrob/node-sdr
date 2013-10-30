#ifndef FILTER_H
#define FILTER_H

#include "buffer.h"


class Filter {
protected:
    Buffer &input, output;

public:
    Filter(Buffer &inp);
    Filter(Buffer &inp, int sampleRatio);
    ~Filter();

    Buffer &getOutputBuffer() { return output; }

    void process();
};


#endif

