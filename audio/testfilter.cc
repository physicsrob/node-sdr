#include <stdio.h>
#include <math.h>
#include <node.h>
#include <v8.h>
#include "sampledef.h"
#include "buffer.h"
#include "filter.h"
#include "firfilter.h"
#include "siggenfilter.h"
#include "decfilter.h"

using namespace v8;

float filter1(float freq) {
    float inPow, outPow;

    Buffer b(2, 1024*64, SAMPLE_RATE);
    SigGenFilter sig(b);
    FIRFilter f(sig.getOutputBuffer());
    f.setSincFIR(1000, 101);
    //f.shiftFilter(1500);
    f.windowBlackman();

    sig.setFreq(freq);
    sig.process();
    f.process();
    sig.process();
    f.process();
    inPow = sig.getOutputBuffer().getAvgSqr();
    outPow = f.getOutputBuffer().getAvgSqr();

    return outPow/inPow;
}


Handle<Value> TestFilter(const Arguments& args) {
    HandleScope scope;
    if (args.Length() < 3) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }

    int steps = 20;
    int fnum = args[0]->NumberValue();
    float startFreq = args[1]->NumberValue(), endFreq = args[2]->NumberValue();
    float freqDelta = (endFreq - startFreq) / steps;

    for(int step=0; step<steps; step++) {
        float freq = startFreq + freqDelta*(float)step;
        float val=1.0;
        switch(fnum) {
            case 1:
                val=filter1(freq);
                break;
        }
        fprintf(stderr, "%f\t%f dB\n", freq, 10.0*log10(val));
    }


    return scope.Close(Undefined());
}

