#ifndef AudioProcessor_H
#define AudioProcessor_H

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

#define MODE_USB_RX 1
#define MODE_USB_TX 2


class AudioProcessor {
protected:
    static AudioProcessor *instance;

    Buffer srcBuf;
    SigGenFilter fakeSig;  bool useFakeSig;
    FIRFilter preTuneFilter;
    SigGenFilter vfo;
    MixerFilter mixer;
    FIRFilter postTuneFilter;

    float iq_tx_alpha, iq_tx_epsilon, iq_rx_alpha, iq_rx_epsilon;
    int mode;

    PaStream *stream;
    fftw_complex *fft_in, *fft_out;
    fftw_plan fft_plan;

private:
    AudioProcessor();
    ~AudioProcessor();

public:
    void start(char *input, char *output);
    void stop();
    fftw_complex *runFFT(bool src);
    Buffer &getCurrentOutput();
    void setMode(int m);

    void setTuner(float f) { vfo.setFreq(f); }
    void setFakeSource(float f) { fakeSig.setFreq(f); useFakeSig=true;}
    void setIQBal(float txa, float txe, float rxa, float rxe) {
        iq_tx_alpha = txa;
        iq_tx_epsilon = txe;
        iq_rx_alpha = rxa;
        iq_rx_epsilon = rxe;
    }

public: // Static methods
    static AudioProcessor *getInstance() {
        if(!instance) { instance = new AudioProcessor(); }
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

    static Handle<Value> getFFT(const Arguments&args);


    static Handle<Value> setTuner(const Arguments& args) {
        HandleScope scope;
        getInstance()->setTuner(args[0]->NumberValue());
        return scope.Close(Undefined());
    }


    static Handle<Value> setFakeSource(const Arguments& args) {
        HandleScope scope;
        getInstance()->setFakeSource(args[0]->NumberValue());
        return scope.Close(Undefined());
    }


    static Handle<Value> setMode(const Arguments& args) {
        HandleScope scope;
        int mode = (int)args[0]->NumberValue();
        getInstance()->setMode(mode);
        return scope.Close(Undefined());
    }

    static Handle<Value> getSampleRate(const Arguments& args) {
        HandleScope scope;
        return scope.Close(Number::New(SAMPLE_RATE));
    }

    static Handle<Value> setIQBal(const Arguments& args) {
        HandleScope scope;
        getInstance()->setIQBal(args[0]->NumberValue(),args[1]->NumberValue(),args[2]->NumberValue(),args[3]->NumberValue());
        return scope.Close(Undefined());
    }



    static int callback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );

};

#endif

