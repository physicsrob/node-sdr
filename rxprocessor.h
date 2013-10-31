#ifndef RXPROCESSOR_H
#define RXPROCESSOR_H

#include <node.h>
#include <portaudio.h>
#include <v8.h>
#include "util.h"
#include "buffer.h"
#include "siggenfilter.h"
#include "firfilter.h"
#include "mixerfilter.h"
#include <fftw3.h>

using namespace v8;

class RXProcessor {
protected:
    static RXProcessor *instance;
    Buffer srcBuf;
    PaStream *stream;
    SigGenFilter sig;

    SigGenFilter vfo;
    MixerFilter mixer;
    FIRFilter bandpass;

    float bw = 3000.0;

    fftw_complex *fft_in, *fft_out;
    fftw_plan fft_plan;

private:
    RXProcessor();
    ~RXProcessor();

public:
    void start(char *input, char *output);
    void stop();
    void setFrequency(float f) { vfo.setFreq(f); }
    fftw_complex *runFFT();

    Buffer &getCurrentOutput();

public: // Static methods
    static RXProcessor *getInstance() {
        if(!instance) { instance = new RXProcessor(); }
        return instance;
    }
    static Handle<Value> start(const Arguments& args) {
        HandleScope scope;

        char * inpDevStr = TO_CHAR(args[0]);
        char * outDevStr = TO_CHAR(args[1]);
        getInstance()->start(inpDevStr, outDevStr);
        free(inpDevStr); free(outDevStr);
        return scope.Close(Undefined());
    }

    static Handle<Value> stop(const Arguments& args) {
        HandleScope scope;
        getInstance()->stop();
        return scope.Close(Undefined());
    }

    static Handle<Value> setFrequency(const Arguments& args) {
        HandleScope scope;
        getInstance()->setFrequency(args[0]->NumberValue());
        return scope.Close(Undefined());
    }

    static Handle<Value> getFFT(const Arguments&args);



    static int callback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );

};

#endif

