#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include <curl/curl.h>

uv_loop_t *loop;
CURLM *curl_handle;
uv_timer_t timeout;

typedef struct curl_context_s {
  uv_poll_t poll_handle;
  curl_socket_t  sockfd;
} curl_context_t;

curl_context_t *create_curl_context(curl_socket_t sockfd) {
  curl_context_t *context = (curl_context_t*)malloc(sizeof(curl_context_t));
  context->sockfd = sockfd;
  int r = uv_poll_init_socket(loop, &context->poll_handle, sockfd);
  assert(r == 0);
  /*
    使用data域来保存指向自身context的指针
  */
  context->poll_handle.data = context;

  return context;
}

void curl_close_cb(uv_handle_t *handle) {
  curl_context_t *context = (uv_poll_t *)handle->data;
  free(context);
}



