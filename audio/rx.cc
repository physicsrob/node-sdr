#include <stdio.h>
#include <math.h>
#include <node.h>
#include <portaudio.h>
#include <v8.h>
#include "util.h"

using namespace v8;
int GetDeviceNumber(const char *str);
PaStream *stream = NULL;
float frequency = 10000.0;

#define SAMPLE_RATE         (88200)
#define PA_SAMPLE_TYPE      paFloat32
#define FRAMES_PER_BUFFER   (1024*8)

typedef float SAMPLE;

float getNextSin() {
    static int ind=0;
    return sin(2*3.14159*frequency*(ind++)/SAMPLE_RATE);
}

float getNextCos() {
    static int ind=0;
    return cos(2*3.14159*frequency*(ind++)/SAMPLE_RATE);
}


float getNextSin2() {
    static int ind=0;
    return sin(2*3.14159*12000.0*(ind++)/SAMPLE_RATE);
}

float getNextCos2() {
    static int ind=0;
    return cos(2*3.14159*12000.0*(ind++)/SAMPLE_RATE);
}



float lowPassFilter(float inp) {
    static float res = 0.0;
    float A=0.99;
    res=A*res + (1.0-A)*inp;
    return (1.0/(1.0-A))*res;
}



static int RXCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData );
/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int RXCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData )
{
    SAMPLE *out = (SAMPLE*)outputBuffer;
    const SAMPLE *in = (const SAMPLE*)inputBuffer;
    unsigned int i;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) userData;

    float avg=0;

    if( inputBuffer == NULL )
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            *out++ = 0;  /* left - silent */
            *out++ = 0;  /* right - silent */
        }
    }
    else
    {
        for( i=0; i<framesPerBuffer; i++ )
        {
            //float I = *in++, Q=*in++;
            float Q=getNextSin2(), I=getNextCos2();
            float outVal = I*getNextSin() - Q*getNextCos();
            outVal = lowPassFilter(outVal);
            *out++ = outVal; *out++ = outVal;
            avg+=outVal*outVal;
//            printf("I=%f, Q=%f, out=%f\n", I,Q, outVal);
        }
        fprintf(stderr, "avg=%f\n", avg);
    }

    return paContinue;
}

Handle<Value> StartRX(const Arguments& args) {
    HandleScope scope;
    PaError err;
    PaStreamParameters inputParameters, outputParameters;

    if (args.Length() < 2) {
        ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
        return scope.Close(Undefined());
    }

    if (!args[0]->IsString() || !args[1]->IsString()) {
        ThrowException(Exception::TypeError(String::New("Wrong arguments")));
        return scope.Close(Undefined());
    }
    char * inpDevStr = TO_CHAR(args[0]);
    char * outDevStr = TO_CHAR(args[1]);

    int inpDev = GetDeviceNumber(inpDevStr);
    int outDev = GetDeviceNumber(outDevStr);

    free(inpDevStr); free(outDevStr);

    fprintf(stderr, "Devices: %d, %d", inpDev, outDev);
    if(inpDev<0 || outDev<0) {
        printf("Could not find device.");
        return scope.Close(Undefined());
    }

    err = Pa_Initialize();
    if( err != paNoError ) {
        printf("Could not initialize port audio");
        return scope.Close(Undefined());
    }

    inputParameters.device = inpDev;
    inputParameters.channelCount = 2;       /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = outDev;
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              0, /* paClipOff, */  /* we won't output out of range samples so don't bother clipping them */
              RXCallback,
              NULL );
    if( err != paNoError ) {
        printf("Could not open stream");
        return scope.Close(Undefined());
    }
    err = Pa_StartStream( stream );
    if( err != paNoError ) {
        printf("Could not start stream");
        return scope.Close(Undefined());
    }

    return scope.Close(Undefined());
}

Handle<Value> StopRX(const Arguments& args) {
    HandleScope scope;
    PaError err;
    err = Pa_CloseStream( stream );
    err=Pa_Terminate();
    return scope.Close(Undefined());
}

