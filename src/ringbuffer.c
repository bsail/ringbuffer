#include "ringbuffer.h"

int8_t ringBufferInit(RingBuffer *buffer, uint8_t *data, uint32_t len, void *(*user_memcpy)(void *str1, const void *str2, size_t n)
#ifndef RINGBUFFER_EXCLUDE_LOCKING
   , void (*buffer_lock)(void), void (*buffer_unlock)(void)
#endif
   ) {
   if(!(len && !(len & (len - 1)))) {
      return 0;
   }

   buffer->tail = 0;
   buffer->head = 0;
   buffer->sizeMask = len-1;
   buffer->data = data;
   buffer->user_memcpy = user_memcpy;
#ifndef RINGBUFFER_EXCLUDE_LOCKING   
   buffer->buffer_lock = buffer_lock;
   buffer->buffer_unlock = buffer_unlock;
#endif
   return 1;
}

uint32_t ringBufferLen(RingBuffer *buffer) {
   if(buffer->tail >= buffer->head) {
      return buffer->tail-buffer->head;
   }

   return buffer->sizeMask-(buffer->head-buffer->tail)+1;
}

uint8_t ringBufferEmpty(RingBuffer *buffer) {
   return (buffer->tail == buffer->head);
}

uint32_t ringBufferLenAvailable(RingBuffer *buffer){
   return buffer->sizeMask - ringBufferLen(buffer);
}

uint32_t ringBufferMaxSize(RingBuffer *buffer) {
   return buffer->sizeMask;
}

void ringBufferAppendOne(RingBuffer *buffer, uint8_t data){
   buffer->data[buffer->tail] = data;
   buffer->tail = (buffer->tail + 1) & buffer->sizeMask;
}

void ringBufferAppendMultiple(RingBuffer *buffer, uint8_t *data, uint32_t len){
   if(buffer->tail + len > buffer->sizeMask) {
      uint32_t lenToTheEnd = buffer->sizeMask - buffer->tail + 1;
      uint32_t lenFromBegin = len - lenToTheEnd;
      buffer->user_memcpy(buffer->data + buffer->tail, data, lenToTheEnd);
      buffer->user_memcpy(buffer->data, data + lenToTheEnd, lenFromBegin);
   }
   else {
      buffer->user_memcpy(buffer->data, data, len);
   }
   buffer->tail = (buffer->tail + len) & buffer->sizeMask;
}

uint8_t ringBufferPeakOne(RingBuffer *buffer){
   return buffer->data[buffer->head];
}

uint8_t ringBufferGetOne(RingBuffer *buffer){
   uint8_t data =  buffer->data[buffer->head];
   buffer->head = (buffer->head + 1) & buffer->sizeMask;
   return data;
}

void ringBufferGetMultiple(RingBuffer *buffer, uint8_t *dst, uint32_t len) {
   ringBufferPeakMultiple(buffer, dst, len);
   buffer->head = (buffer->head + len) & buffer->sizeMask;
}

void ringBufferPeakMultiple(RingBuffer *buffer, uint8_t *dst, uint32_t len){
   if(buffer->head + len > buffer->sizeMask) {
      uint32_t lenToTheEnd = buffer->sizeMask - buffer->head + 1;
      uint32_t lenFromBegin = len - lenToTheEnd;
      buffer->user_memcpy(dst, buffer->data + buffer->head, lenToTheEnd);
      buffer->user_memcpy(dst + lenToTheEnd, buffer->data, lenFromBegin);
   }
   else {
      buffer->user_memcpy(dst, buffer->data, len);
   }
}

void ringBufferDiscardMultiple(RingBuffer *buffer, uint32_t len){
   buffer->head = (buffer->head + len) + buffer->sizeMask;
}

void ringBufferClear(RingBuffer *buffer){
   buffer->head = buffer->tail = 0;
}