#include <stdio.h>
#include <math.h>
#include <node.h>
#include <v8.h>
#include "sampledef.h"
#include "buffer.h"
#include "filter.h"
#include "firfilter.h"

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

    int sampleNum = 0;
    int bufLen = 1024, channels=2;
    int steps = 20;
    float startFreq = args[1]->NumberValue(), endFreq = args[2]->NumberValue();
    float freqDelta = (endFreq - startFreq) / steps;

    SAMPLE *samples = new SAMPLE[bufLen * channels];
    Buffer b(channels, bufLen, SAMPLE_RATE);
    FIRFilter f(b);
    f.setSincFIR(4000, 257);
    f.shiftFilter(2100);
    f.print();
    f.windowBlackman();
    f.print();

    for(int step=0; step<steps; step++) {
        float freq = startFreq + freqDelta*(float)step;
        float inPow, outPow;

        for(int i=0; i<bufLen; i++) {
            samples[2*i]   = cos(2*3.14159*freq * sampleNum/SAMPLE_RATE);
            samples[2*i+1] = sin(2*3.14159*freq * sampleNum/SAMPLE_RATE);
            sampleNum++;
        }
        fprintf(stderr, "Adding data\n");
        b.addData(samples);
        for(int i=0; i<bufLen; i++) {
            samples[2*i]   = cos(2*3.14159*freq * sampleNum/SAMPLE_RATE);
            samples[2*i+1] = sin(2*3.14159*freq * sampleNum/SAMPLE_RATE);
            sampleNum++;
        }
        fprintf(stderr, "Adding data\n");
        b.addData(samples);
        inPow = b.getAvgSqr();
        fprintf(stderr, "processing\n");
        f.process();
        fprintf(stderr, "done processing\n");
        outPow = f.getOutputBuffer().getAvgSqr();
        fprintf(stderr, "%f\t%f dB\n", freq, 10.0*log10(outPow/inPow));
    }

    fprintf(stderr,"Testfilter\n");

    delete samples;
    return scope.Close(Undefined());
}

