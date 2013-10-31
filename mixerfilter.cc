#include <stdio.h>
#include <math.h>
#include "buffer.h"
#include "mixerfilter.h"



MixerFilter::MixerFilter(Buffer &b, Buffer &vfo) : Filter(b), vfo(vfo) {
}
MixerFilter::~MixerFilter() {
}
void MixerFilter::process() {
    output.shiftData(); // Move the last output out
    SAMPLE *outData = output.getData();
    SAMPLE *inpData = input.getData();
    SAMPLE *vfoData = vfo.getData();
    for(int s=0;s<output.getLen();s++) {
        outData[2*s] = inpData[2*s] * vfoData[2*s] - inpData[2*s + 1] * vfoData[2*s + 1];
        outData[2*s + 1] = inpData[2*s] * vfoData[2*s +1] + inpData[2*s + 1] * vfoData[2*s];
    }
}

