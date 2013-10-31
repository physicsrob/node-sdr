#include <node.h>
#include <portaudio.h>
#include <v8.h>
#include <math.h>
#include "util.h"
#include "buffer.h"
#include "rxprocessor.h"
#include "sampledef.h"
#include <unistd.h>
#include <fftw3.h>

using namespace v8;
int GetDeviceNumber(const char *str);

RXProcessor *RXProcessor::instance = NULL;

RXProcessor::RXProcessor() :
    srcBuf(2, FRAMES_PER_BUFFER, SAMPLE_RATE),
    sig(srcBuf),
    vfo(srcBuf),
    mixer(srcBuf, vfo.getOutputBuffer()),
    bandpass(mixer.getOutputBuffer())
{
    vfo.setFreq(-5000.0);
    //sig.setFreq(10440.0);
//    sig.setFreq(9560.0);
//    sig.setFreq(9660.0);

    // Create the RX filter chain
    //bandpass.setUnity();
    bandpass.setSincFIR(2500, 201);
    bandpass.shiftFilter(1750.0);
    bandpass.windowBlackman();


    // Create FFT Plan
    fft_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * srcBuf.getLen());
    fft_out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * srcBuf.getLen());
    fft_plan = fftw_plan_dft_1d(srcBuf.getLen(), fft_in, fft_out, FFTW_FORWARD, FFTW_ESTIMATE);
}

RXProcessor::~RXProcessor() {
    fftw_destroy_plan(fft_plan);
    fftw_free(fft_in);
    fftw_free(fft_out);
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
    //

    //
//    sig.process();
    vfo.process();
    mixer.process();
    bandpass.process();
    float inPow = srcBuf.getAvgSqr();
    float outPow = bandpass.getOutputBuffer().getAvgSqr();
    fprintf(stderr, "%f dB\n", 10.0*log10(outPow/inPow));

    return bandpass.getOutputBuffer();
//    return srcBuf;//.getOutputBuffer();

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


fftw_complex * RXProcessor::runFFT() {
    for(int i=0;i<srcBuf.getLen();i++) {
        fft_in[i][0] = srcBuf.getData()[2*i];
        fft_in[i][1] = srcBuf.getData()[2*i + 1];
    }

    fftw_execute(fft_plan);
    return fft_out;
}

Handle<Value> RXProcessor::getFFT(const Arguments&args)
{
    HandleScope scope;
    int len = getInstance()->srcBuf.getLen();
    fftw_complex *fft_out = getInstance()->runFFT();
    //double *fft_out = (double *)getInstance()->runFFT();
    Local<Array> ret = Array::New(len);
    //fprintf(stderr, "fft1 %f\n", fft_out[100]);
    float out[len], max=-1000.0, min=1000.0;

    for(int i=0;i<len;i++) {
        float val = fft_out[i][0]*fft_out[i][0]+ fft_out[i][1]*fft_out[i][1];
        out[i]=val;
        if(val>max) max=val;
        if(val<min) min=val;
    }
    for(int i=0;i<len;i++) {
        ret->Set(i, Number::New((out[i] - min)/(max-min)));
    }
    return scope.Close(ret);
}



