#include <stdio.h>
#include <math.h>
#include <node.h>
#include <portaudio.h>
#include <v8.h>

using namespace v8;

Handle<Value> ListDevices(const Arguments& args) {
    int     i, numDevices;
    const   PaDeviceInfo *deviceInfo;
    PaError err;
    HandleScope scope;


    numDevices = Pa_GetDeviceCount();
    Local<Array> ret = Array::New(numDevices);

    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
        err = numDevices;
        return scope.Close(Undefined());
    }

    for( i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
        ret->Set(i, String::New(deviceInfo->name));
    }

    return scope.Close(ret);
}

int GetDeviceNumber(const char *str) {
    int     i, numDevices;
    const   PaDeviceInfo *deviceInfo;
    PaError err;
    HandleScope scope;


    numDevices = Pa_GetDeviceCount();
    Local<Array> ret = Array::New(numDevices);

    for( i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
        if(strcmp(deviceInfo->name, str)==0) {
            return i;
        }
    }
    return -1;
}

