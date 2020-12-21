#include <stdlib.h>
#include <assert.h>
#include <node_api.h>
#include <stdio.h>

#define DECLARE_NAPI_METHOD(name, func, moduleContext)                                        \
  { name, 0, func, 0, 0, 0, napi_default, moduleContext }

void freeBuffer(napi_env env, void* finalize_data, void* finalize_hint) {
  (void) finalize_data;
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! freeBuffer \n");
  free(finalize_hint);

  int64_t totalExternal;
  size_t bufferSize = 1920 * 1080 + 960 * 540 * 2;
  napi_adjust_external_memory(env, -(bufferSize), &totalExternal);
}

napi_value NextBuffer(napi_env env, napi_callback_info info) {
  (void) info;

  size_t bufferSize = 1920 * 1080 + 960 * 540 * 2;

  napi_status status;

  // make a napi_value for the return object
  napi_value jsBuf;


  char* buffer = malloc(bufferSize);
  size_t a;
  // fill the buffer with something
  for(a = 0; a < bufferSize; a = a + 1) {
    buffer[a] = 'a';
  }
  status = napi_create_external_buffer(env, bufferSize, buffer, freeBuffer, buffer, &jsBuf);
  assert(status == napi_ok);

  // tell JS that there's a whole load of memory associated with this buffer
  int64_t totalExternal;
  status = napi_adjust_external_memory(env, bufferSize, &totalExternal);
  assert(status == napi_ok);
  printf("??? nextBuffer %ld\n", totalExternal);

  return jsBuf;
}

static napi_value ModuleInit(napi_env env, napi_value exports) {
  napi_status status;

  // export functions
  napi_property_descriptor bindings[] = {
    DECLARE_NAPI_METHOD("nextBuffer", NextBuffer, NULL),
  };
  status = napi_define_properties(env, exports, 1, bindings);
  assert(status == napi_ok);

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, ModuleInit)
