#include <stdio.h>
#include <math.h>
#include "buffer.h"
#include "firfilter.h"



FIRFilter::FIRFilter(Buffer &b) : Filter(b) {
    FIR=NULL;
}
FIRFilter::~FIRFilter() {
    if(FIR) { delete FIR; }
}
void FIRFilter::setSincFIR(float B, int l) {
    B/=2*input.getSampleRate();
    len = l;
    FIR = new float[2*l];
    center = (len - 1) / 2;
    for(int i = 0; i<len; i++) {
        float x = 2 * 3.141593 * B * (i - center);
        if(x==0) FIR[2*i]=1.0;
        else FIR[2*i] = sin(x) / x;
        FIR[2*i + 1] = 0; // imaginary part is zero
    }
}
void FIRFilter::print() {
    for(int i=0;i<len;i++) {
        printf("i=%d  fir=%f + j %f\n", (i - center), FIR[2*i], FIR[2*i+1]);
    }
}

void FIRFilter::windowHamming() {
    float M = len - 1, w;
    for(int i =0;i<len;i ++) {
        w=0.54 - 0.46 * cos(2*3.141593*i/M);
        FIR[2*i]*=w;
        FIR[2*i+1]*=w;
    }
}
void FIRFilter::windowBlackman() {
    float M = len - 1, w;
    for(int i =0;i<len;i ++) {
        w=0.42 - 0.5 * cos(2*3.141593*i/M) + 0.08*cos(4*3.141593*i/M);
        FIR[2*i]*=w;
        FIR[2*i+1]*=w;
    }

}
void FIRFilter::shiftFilter(float F) {
    for(int i = 0; i<len; i++) {
        float R = sin(2*3.141593 * i * F/SAMPLE_RATE);
        float I = cos(2*3.141593 * i * F/SAMPLE_RATE);
        float outR, outI;
        outR = FIR[2*i]*R - FIR[2*i+1]*I;
        outI = FIR[2*i + 1]*R + FIR[2*i]*I;
        FIR[2*i] = outR;
        FIR[2*i+1] = outI;
    }

}

void FIRFilter::process() {
    output.shiftData(); // Move the last output out
    SAMPLE *outData = output.getData();
    SAMPLE *inpData = input.getData();
    for(int s=0;s<output.getLen();s++) {
        float valR = 0.0, valI = 0.0;

        for(int i=0; i<len; i++) {
            valR += inpData[2*(i - len)] * FIR[2*i] - inpData[2*(i - len) + 1] * FIR[2*i + 1];
            valI += inpData[2*(i - len)] * FIR[2*i + 1] + inpData[2*(i - len) + 1] * FIR[2*i];
        }
        outData[2*s] = valR;
        outData[2*s + 1] = valI;
    }
}

