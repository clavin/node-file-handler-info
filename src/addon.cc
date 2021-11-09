#include <napi.h>
#include "impl.h"

namespace {

/**
 * A simple utility to convert an std::optional<os_string> to either a JS string
 * or null.
 */
Napi::Value StringOrNull(Napi::Env env, std::optional<os_string> str) {
  return str.has_value()
    ? Napi::String::From(env, str.value())
    : env.Null();
}

/**
 * Calls the correct method on the Napi::String to convert it to the
 * implementation's string type.
 */
os_string NapiToOsString(Napi::String str) {
#ifdef _WIN32
  return str.Utf16Value();
#else
  return str.Utf8Value();
#endif
}

} // namespace

/**
 * Gets information about the application which handles the file passed in.
 */
Napi::Value GetHandlerInfo(const Napi::CallbackInfo& cbInfo) {
  Napi::Env env = cbInfo.Env();

  // Extract the file path argument
  if (cbInfo.Length() < 1) {
    Napi::TypeError::New(
      env,
      "Expected 1 argument"
    ).ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!cbInfo[0].IsString()) {
    Napi::TypeError::New(
      env,
      "Expected 1st argument to be string"
    ).ThrowAsJavaScriptException();
    return env.Null();
  }
  os_string filePath = ::NapiToOsString(cbInfo[0].As<Napi::String>());

  // Ask the implementation to get the information
  FileHandlerInfo fhInfo;
  GetFileHandlerInfo(filePath, fhInfo);

  // Create and return the result object
  Napi::Object result = Napi::Object::New(env);

  result.Set(
    Napi::String::New(env, "handlerPath"),
    ::StringOrNull(env, fhInfo.handler_path)
  );
  result.Set(
    Napi::String::New(env, "friendlyName"),
    ::StringOrNull(env, fhInfo.friendly_name)
  );

  return result;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(
    Napi::String::New(env, "getHandlerInfo"),
    Napi::Function::New(env, GetHandlerInfo)
  );

  return exports;
}
NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
