#include <node.h>
#include <v8.h>
#include <stdlib.h>
#include <portaudio.h>
#include "audioprocessor.h"

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

    NODE_DEFINE_CONSTANT(target, MODE_USB_RX);
    NODE_DEFINE_CONSTANT(target, MODE_USB_TX);

    NODE_SET_METHOD(target, "getAudioDevices", ListDevices);
    NODE_SET_METHOD(target, "start", AudioProcessor::start);
    NODE_SET_METHOD(target, "stop", AudioProcessor::stop);
    NODE_SET_METHOD(target, "getFFT", AudioProcessor::getFFT);
    NODE_SET_METHOD(target, "setTuner", AudioProcessor::setTuner);
    NODE_SET_METHOD(target, "setFakeSource", AudioProcessor::setFakeSource);
    NODE_SET_METHOD(target, "setMode", AudioProcessor::setMode);
    NODE_SET_METHOD(target, "getSampleRate", AudioProcessor::getSampleRate);
    NODE_SET_METHOD(target, "testFilter", TestFilter);
    atexit(cleanup);
}

NODE_MODULE(nodesdr, init);
