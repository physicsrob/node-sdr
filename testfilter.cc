#include <stdio.h>
#include <math.h>
#include <node.h>
#include <v8.h>
#include "sampledef.h"
#include "buffer.h"
#include "filter.h"
#include "firfilter.h"
#include "siggenfilter.h"

using namespace v8;

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

    int bufLen = 1024, channels=2;
    int steps = 20;
    float startFreq = args[1]->NumberValue(), endFreq = args[2]->NumberValue();
    float freqDelta = (endFreq - startFreq) / steps;

    Buffer b(channels, bufLen, SAMPLE_RATE);
    SigGenFilter sig(b);
    FIRFilter f(sig.getOutputBuffer());
    f.setSincFIR(4000, 257);
    f.shiftFilter(2100);
    f.windowBlackman();

    for(int step=0; step<steps; step++) {
        float freq = startFreq + freqDelta*(float)step;
        float inPow, outPow;

        sig.setFreq(freq);
        sig.process();
        f.process();
        sig.process();
        f.process();

        inPow = sig.getOutputBuffer().getAvgSqr();
        outPow = f.getOutputBuffer().getAvgSqr();
        fprintf(stderr, "%f\t%f dB\n", freq, 10.0*log10(outPow/inPow));
    }


    return scope.Close(Undefined());
}

