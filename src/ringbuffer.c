#include "ringbuffer.h"

int8_t ringBufferInit(RingBuffer *buffer,
   uint8_t *data,
   uint32_t dataSize,
   uint32_t len,
   void *(*user_memcpy)(void *str1, const void *str2, size_t n)
#ifndef RINGBUFFER_EXCLUDE_LOCKING
   , void (*buffer_lock)(void),
   void (*buffer_unlock)(void)
#endif
   ) {
   int8_t ret;
   if(!(len && !(len & (len - 1)))) {
      ret = 0;
   } else {
      buffer->tail = 0;
      buffer->head = 0;
      buffer->sizeMask = len-1;
      buffer->data = data;
      buffer->dataSize = dataSize;
      buffer->user_memcpy = user_memcpy;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
      buffer->buffer_lock = buffer_lock;
      buffer->buffer_unlock = buffer_unlock;
#endif
      ret = 1;
   }
   return ret;
}

static uint32_t ringBufferLenInternal(RingBuffer *buffer) {
   uint32_t ret;
   if(buffer->tail >= buffer->head) {
      ret = buffer->tail-buffer->head;
   } else {
      ret = buffer->sizeMask-(buffer->head-buffer->tail)+1;
   }
   return ret;
}

uint32_t ringBufferLen(RingBuffer *buffer) {
   uint32_t ret;
   ret = ringBufferLenInternal(buffer);
   return ret;
}


uint8_t ringBufferEmpty(RingBuffer *buffer) {
   uint8_t ret;
   ret = (buffer->tail == buffer->head);
   return ret;
}

uint32_t ringBufferLenAvailable(RingBuffer *buffer){
   uint32_t ret;
   ret = buffer->sizeMask - ringBufferLenInternal(buffer);
   return ret;
}

uint32_t ringBufferMaxSize(RingBuffer *buffer) {
   uint32_t ret;
   ret = buffer->sizeMask;
   return ret;
}

void ringBufferAppendOne(RingBuffer *buffer, uint8_t* data){
   buffer->user_memcpy(&(buffer->data[buffer->tail*buffer->dataSize]),data,buffer->dataSize);
   buffer->tail = (buffer->tail + 1) & buffer->sizeMask;
}

void ringBufferAppendMultiple(RingBuffer *buffer, uint8_t *data, uint32_t len){
   if(buffer->tail + len > buffer->sizeMask) {
      uint32_t lenToTheEnd = buffer->sizeMask - buffer->tail + 1;
      uint32_t lenFromBegin = len - lenToTheEnd;
      buffer->user_memcpy(&(buffer->data[buffer->tail*buffer->dataSize]), data, lenToTheEnd*buffer->dataSize);
      buffer->user_memcpy(buffer->data, &(data[lenToTheEnd*buffer->dataSize]), lenFromBegin*buffer->dataSize);
   }
   else {
      buffer->user_memcpy(buffer->data, data, len*buffer->dataSize);
   }
   buffer->tail = (buffer->tail + len) & buffer->sizeMask;
}

static void ringBufferPeakOneInternal(RingBuffer *buffer, uint8_t* data){
   buffer->user_memcpy(data,&(buffer->data[buffer->head*buffer->dataSize]),buffer->dataSize);
}

void ringBufferPeakOne(RingBuffer *buffer, uint8_t* data){
   ringBufferPeakOneInternal(buffer,data);
}

void ringBufferGetOne(RingBuffer *buffer, uint8_t* data){
   ringBufferPeakOneInternal(buffer,data);
   buffer->head = (buffer->head + 1) & buffer->sizeMask;
}

static void ringBufferPeakMultipleInternal(RingBuffer *buffer, uint8_t *dst, uint32_t len){
   if(buffer->head + len > buffer->sizeMask) {
      uint32_t lenToTheEnd = buffer->sizeMask - buffer->head + 1;
      uint32_t lenFromBegin = len - lenToTheEnd;
      buffer->user_memcpy(dst, &(buffer->data[buffer->head*buffer->dataSize]), lenToTheEnd*buffer->dataSize);
      buffer->user_memcpy(&(dst[lenToTheEnd*buffer->dataSize]), buffer->data, lenFromBegin*buffer->dataSize);
   }
   else {
      buffer->user_memcpy(dst, buffer->data, len*buffer->dataSize);
   }
}

void ringBufferGetMultiple(RingBuffer *buffer, uint8_t *dst, uint32_t len) {
   ringBufferPeakMultipleInternal(buffer, dst, len);
   buffer->head = (buffer->head + len) & buffer->sizeMask;
}

void ringBufferPeakMultiple(RingBuffer *buffer, uint8_t *dst, uint32_t len){
   ringBufferPeakMultipleInternal(buffer, dst, len);
}

void ringBufferDiscardMultiple(RingBuffer *buffer, uint32_t len){
   buffer->head = (buffer->head + len) + buffer->sizeMask;
}

void ringBufferClear(RingBuffer *buffer){
   buffer->head = buffer->tail = 0;
}