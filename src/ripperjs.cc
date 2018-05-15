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

  const char * rb_cRipperSexp(const char *code) {
    VALUE rb_cRipper = rb_const_get(rb_cObject, rb_intern("Ripper"));
    VALUE result = rb_funcall(rb_cRipper, rb_intern("sexp"), 1, rb_str_new2(code));

    VALUE inspected = rb_sprintf("%" PRIsVALUE, result);
    return StringValueCStr(inspected);
  }

  void Sexp(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1) {
      Local<String> error = String::NewFromUtf8(isolate, "Wrong number of arguments");
      isolate->ThrowException(Exception::TypeError(error));
      return;
    }

    if (!args[0]->IsString()) {
      Local<String> error = String::NewFromUtf8(isolate, "Filename must be a string");
      isolate->ThrowException(Exception::TypeError(error));
      return;
    }

    String::Utf8Value code(isolate, args[0]->ToString());
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, rb_cRipperSexp(*code)));
  }

  void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "sexp", Sexp);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, init)
}
