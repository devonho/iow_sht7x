#include <napi.h>

#include "sht7x.h"


Napi::Number MethodReadTemp(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  double value;
  IOWSHT7x sht;
  if(sht.open() && sht.read())
  {
    value = sht.getTemperature();
    sht.close();
  }
  else
  {
    sht.close();
    throw Napi::Error::New(env, sht.get_last_error());
  }

  return Napi::Number::New(env, value);
}

Napi::Number MethodReadHumidity(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  double value;
  IOWSHT7x sht;
  if(sht.open() && sht.read())
  {
    value = sht.getHumidity();
    sht.close();
  }
  else
  {
    sht.close();
    throw Napi::Error::New(env, sht.get_last_error());
  }  

  return Napi::Number::New(env, value);
}


Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "readTemperature"),
              Napi::Function::New(env, MethodReadTemp));
  exports.Set(Napi::String::New(env, "readHumidity"),
              Napi::Function::New(env, MethodReadHumidity));
  return exports;
}

NODE_API_MODULE(iow_sht7x, Init)