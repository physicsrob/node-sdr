#include <node.h>
#include <v8.h>

using namespace v8;

Handle<Value> ListDevices(const Arguments& args);
Handle<Value> StartRX(const Arguments& args);
Handle<Value> StopRX(const Arguments& args);
Handle<Value> TestFilter(const Arguments& args);

Handle<Value> Method(const Arguments& args) {
  HandleScope scope;
  return scope.Close(String::New("world"));
}


void init(Handle<Object> target) {
  NODE_SET_METHOD(target, "hello", Method);
  NODE_SET_METHOD(target, "getAudioDevices", ListDevices);
  NODE_SET_METHOD(target, "startRX", StartRX);
  NODE_SET_METHOD(target, "stopRX", StopRX);
  NODE_SET_METHOD(target, "testFilter", TestFilter);
}

NODE_MODULE(nodesdr, init);
