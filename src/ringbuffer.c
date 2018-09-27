#include "ringbuffer.h"

int8_t ringbuffer_init(ringbuffer * buffer,
		       uint8_t * data,
		       uint32_t dataSize,
		       uint32_t len,
		       void *(*user_memcpy) (void *str1, const void *str2,
					     size_t n)
#ifndef RINGBUFFER_EXCLUDE_LOCKING
		       , void (*buffer_lock) (void),
		       void (*buffer_unlock) (void)
#endif
    )
{
	int8_t ret;
	if (!(len && !(len & (len - 1)))) {
		ret = 0;
	} else {
		buffer->tail = 0;
		buffer->head = 0;
		buffer->sizeMask = len - 1;
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

static uint32_t ringbuffer_lengthInternal(ringbuffer * buffer)
{
	uint32_t ret;
	if (buffer->tail >= buffer->head) {
		ret = buffer->tail - buffer->head;
	} else {
		ret = buffer->sizeMask - (buffer->head - buffer->tail) + 1;
	}
	return ret;
}

uint32_t ringbuffer_length(ringbuffer * buffer)
{
	uint32_t ret;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ret = ringbuffer_lengthInternal(buffer);
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
	return ret;
}

uint8_t ringbuffer_is_empty(ringbuffer * buffer)
{
	uint8_t ret;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ret = (buffer->tail == buffer->head);
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
	return ret;
}

uint32_t ringbuffer_length_available(ringbuffer * buffer)
{
	uint32_t ret;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ret = buffer->sizeMask - ringbuffer_lengthInternal(buffer);
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
	return ret;
}

uint32_t ringbufferMaxSize(ringbuffer * buffer)
{
	uint32_t ret;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ret = buffer->sizeMask;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
	return ret;
}

void ringbuffer_append(ringbuffer * buffer, uint8_t * data)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	buffer->user_memcpy(&(buffer->data[buffer->tail * buffer->dataSize]),
			    data, buffer->dataSize);
	buffer->tail = (buffer->tail + 1) & buffer->sizeMask;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

void ringbuffer_append_multiple(ringbuffer * buffer, uint8_t * data,
				uint32_t len)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	if (buffer->tail + len > buffer->sizeMask) {
		uint32_t lenToTheEnd = buffer->sizeMask - buffer->tail + 1;
		uint32_t lenFromBegin = len - lenToTheEnd;
		buffer->user_memcpy(&
				    (buffer->data
				     [buffer->tail * buffer->dataSize]), data,
				    lenToTheEnd * buffer->dataSize);
		buffer->user_memcpy(buffer->data,
				    &(data[lenToTheEnd * buffer->dataSize]),
				    lenFromBegin * buffer->dataSize);
	} else {
		buffer->user_memcpy(buffer->data, data, len * buffer->dataSize);
	}
	buffer->tail = (buffer->tail + len) & buffer->sizeMask;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

static void ringbuffer_peakInternal(ringbuffer * buffer, uint8_t * data)
{
	buffer->user_memcpy(data,
			    &(buffer->data[buffer->head * buffer->dataSize]),
			    buffer->dataSize);
}

void ringbuffer_peak(ringbuffer * buffer, uint8_t * data)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ringbuffer_peakInternal(buffer, data);
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

void ringbuffer_get(ringbuffer * buffer, uint8_t * data)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ringbuffer_peakInternal(buffer, data);
	buffer->head = (buffer->head + 1) & buffer->sizeMask;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

static void ringbuffer_peak_multipleInternal(ringbuffer * buffer, uint8_t * dst,
					     uint32_t len)
{
	if (buffer->head + len > buffer->sizeMask) {
		uint32_t lenToTheEnd = buffer->sizeMask - buffer->head + 1;
		uint32_t lenFromBegin = len - lenToTheEnd;
		buffer->user_memcpy(dst,
				    &(buffer->data
				      [buffer->head * buffer->dataSize]),
				    lenToTheEnd * buffer->dataSize);
		buffer->user_memcpy(&(dst[lenToTheEnd * buffer->dataSize]),
				    buffer->data,
				    lenFromBegin * buffer->dataSize);
	} else {
		buffer->user_memcpy(dst, buffer->data, len * buffer->dataSize);
	}
}

void ringbuffer_get_multiple(ringbuffer * buffer, uint8_t * dst, uint32_t len)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ringbuffer_peak_multipleInternal(buffer, dst, len);
	buffer->head = (buffer->head + len) & buffer->sizeMask;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

void ringbuffer_peak_multiple(ringbuffer * buffer, uint8_t * dst, uint32_t len)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	ringbuffer_peak_multipleInternal(buffer, dst, len);
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

void ringbuffer_discard_multiple(ringbuffer * buffer, uint32_t len)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	buffer->head = (buffer->head + len) + buffer->sizeMask;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}

void ringbuffer_clear(ringbuffer * buffer)
{
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_lock();
#endif
	buffer->head = buffer->tail = 0;
#ifndef RINGBUFFER_EXCLUDE_LOCKING
	buffer->buffer_unlock();
#endif
}
