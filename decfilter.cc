#include <stdio.h>
#include <math.h>
#include "buffer.h"
#include "decfilter.h"



DecFilter::DecFilter(Buffer &b, int ratio) : Filter(b, ratio), ratio(ratio) {
}

DecFilter::~DecFilter() {
}

void DecFilter::process() {
    output.shiftData(); // Move the last output out
    SAMPLE *outData = output.getData();
    SAMPLE *inpData = input.getData();

    for(int s=0;s<output.getLen(); s++) {
        outData[2*s]=0.0;
        outData[2*s+1]=0.0;
    }
    for(int s=0;s<input.getLen();s++) {
        int outS = s/ratio;
        outData[2*outS] += inpData[2*s];
        outData[2*outS+1] += inpData[2*s + 1];
    }
}

