#include <stdio.h>
#include <math.h>
#include "buffer.h"
#include "siggenfilter.h"



SigGenFilter::SigGenFilter(Buffer &b) : Filter(b) {
    omega=0.0; domega=0.0;
    freq=1000.0;
}

SigGenFilter::~SigGenFilter() {
}

void SigGenFilter::setFreq(float f) {
    domega = 2.0 * M_PI * f / output.getSampleRate();
}


void SigGenFilter::process() {
    output.shiftData(); // Move the last output out
    SAMPLE *outData = output.getData();
    omega = fmod(omega, 2*M_PI);
    for(int s=0;s<output.getLen();s++) {
        omega += domega;
        outData[2*s] = cos(omega);
        outData[2*s+1] = sin(omega);
    }
}

