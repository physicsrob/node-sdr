#include <stdio.h>
#include <math.h>
#include "buffer.h"
#include "siggenfilter.h"



SigGenFilter::SigGenFilter(Buffer &b) : Filter(b) {
    sampleNum=0;
    freq=1000.0;
}

SigGenFilter::~SigGenFilter() {
}

void SigGenFilter::process() {
    output.shiftData(); // Move the last output out
    SAMPLE *outData = output.getData();
    float r = 2*3.14159*freq / output.getSampleRate();

    for(int s=0;s<output.getLen();s++) {
        sampleNum++;
        outData[2*s] = cos(r * sampleNum);
        outData[2*s+1] = sin(r * sampleNum);
    }
}

