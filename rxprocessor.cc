#include <node.h>
#include <portaudio.h>
#include <v8.h>
#include "util.h"
#include "buffer.h"
#include "rxprocessor.h"
#include "sampledef.h"
#include <unistd.h>

using namespace v8;
int GetDeviceNumber(const char *str);

RXProcessor *RXProcessor::instance = NULL;

RXProcessor::RXProcessor() : srcBuf(2, FRAMES_PER_BUFFER, SAMPLE_RATE), sig(srcBuf) {
    frequency = 10000.0; // Default freq
    sig.setFreq(440);
}

void RXProcessor::start(char *input, char *output) {
    PaError err;
    PaStreamParameters inputParameters, outputParameters;

    int inpDev = GetDeviceNumber(input);
    int outDev = GetDeviceNumber(output);

    fprintf(stderr, "Devices: %d, %d\n", inpDev, outDev);
    if(inpDev<0 || outDev<0) {
        fprintf(stderr,"Could not find device.\n");
        fprintf(stderr, "Input: \"%s\"\n", input);
        fprintf(stderr, "Output: \"%s\"\n", output);

        return;
    }
    inputParameters.device = inpDev;
    inputParameters.channelCount = 2;       /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = outDev;
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              &RXProcessor::callback,
              NULL );
    if( err != paNoError ) {
        printf("Could not open stream");
        return;
    }
    err = Pa_StartStream( stream );
    if( err != paNoError ) {
        printf("Could not start stream");
        return;
    }

}

void RXProcessor::stop() {
    PaError err;
    err = Pa_CloseStream( stream );
}

Buffer &RXProcessor::getCurrentOutput() {
    // Needs to be about 5 times faster than real time
    // e.g. if buffer is 100ms, this function must complete in 20ms
    sig.process();
    return srcBuf;//.getOutputBuffer();

}



int RXProcessor::callback( const void *inputBuffer, void *outputBuffer,
                     unsigned long framesPerBuffer,
                     const PaStreamCallbackTimeInfo* timeInfo,
                     PaStreamCallbackFlags statusFlags,
                     void *userData ) {
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) userData;

    if( inputBuffer == NULL )
    {
        fprintf(stderr, "Error: Empty input\n");
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = 0;  /* left - silent */
            *out++ = 0;  /* right - silent */
        }
    }
    else
    {
        fprintf(stderr, "%d frames\n", framesPerBuffer);
        getInstance()->srcBuf.addData(in);
        Buffer &outBuf = getInstance()->getCurrentOutput();
        memcpy(out, outBuf.getData(), framesPerBuffer*sizeof(SAMPLE)*2);
    }

    return paContinue;

}


