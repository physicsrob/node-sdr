#ifndef BUFFER_H
#define BUFFER_H

#include <string.h>
#include <stdio.h>
#include "sampledef.h"

class Buffer {
    SAMPLE *buf;
    int channels;
    int len;
    float sampleRate;

public:
    Buffer(int channels, int len, float sampleRate);
    Buffer(const Buffer &b, int sampleRatio = 1);
    ~Buffer();
    void addData(SAMPLE *data);
    void shiftData();


    SAMPLE *getData() { return &buf[len*channels]; }
    int getLen() { return len; }
    int getChannels() { return channels; }

    // Convenience methods
    float getAvgSqr() {
        float ret = 0.0;
        for(int i=0; i<(len*channels); i++){
            ret+=buf[len*channels + i]*buf[len*channels + i];
        }
        return ret/(len*channels);
    }


};


#endif
