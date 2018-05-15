#include <node.h>
#include <ruby.h>

namespace ripperjs {
  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void Sexp(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
      return;
    }

    if (!args[0]->IsString()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Filename must be a string")));
      return;
    }

    args.GetReturnValue().Set(args[0]->ToString());
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "sexp", Sexp);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, init)
}
