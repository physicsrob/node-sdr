#include <node.h>
#include <v8.h>
#include <stdlib.h>
#include <portaudio.h>
#include "rxprocessor.h"

using namespace v8;

Handle<Value> ListDevices(const Arguments& args);
Handle<Value> TestFilter(const Arguments& args);

void cleanup() {
    PaError err = Pa_Terminate();
    if( err != paNoError ) {
        printf("Could not close port audio");
    }
}



void init(Handle<Object> target) {
    PaError err = Pa_Initialize();
    if( err != paNoError ) {
        printf("Could not initialize port audio");
    }
    NODE_SET_METHOD(target, "getAudioDevices", ListDevices);
    NODE_SET_METHOD(target, "startRX", RXProcessor::start);
    NODE_SET_METHOD(target, "stopRX", RXProcessor::stop);
    NODE_SET_METHOD(target, "setRXFrequency", RXProcessor::setFrequency);
    NODE_SET_METHOD(target, "getFFT", RXProcessor::getFFT);
    NODE_SET_METHOD(target, "testFilter", TestFilter);
    atexit(cleanup);
}

NODE_MODULE(nodesdr, init);
