#include <string.h>
#include <stdio.h>
#include "sampledef.h"
#include "buffer.h"

Buffer::Buffer(int channels, int len, float sampleRate) : channels(channels), len(len), sampleRate(sampleRate) {
    buf = new SAMPLE[2 * len * channels];
    memset(buf, 0, sizeof(SAMPLE)*2*len*channels);
    fprintf(stderr, "Buffer(1)\n");
};

Buffer::Buffer(const Buffer &b, int sampleRatio) : channels(b.channels), len(b.len/sampleRatio), sampleRate(b.sampleRate/sampleRatio){ // Create a buffer the same size as an exiting one
    buf = new SAMPLE[2 * len * channels];
    memset(buf, 0, sizeof(SAMPLE)*2*len*channels);
    fprintf(stderr, "Buffer(2)\n");
}

Buffer::~Buffer() {
}

void Buffer::addData(SAMPLE *data) {
        shiftData();
        memcpy(&buf[channels*len], data, sizeof(SAMPLE)*channels*len);
}
void Buffer::shiftData() {
    memcpy(buf, &buf[channels*len], sizeof(SAMPLE)*channels*len);
}

