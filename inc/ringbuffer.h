#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_

#include <stdint.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif

	typedef struct {
		uint32_t tail;
		uint32_t head;
		uint32_t sizeMask;
		uint8_t *data;
		uint32_t dataSize;
		void *(*user_memcpy) (void *str1, const void *str2, size_t n);
#ifndef RINGBUFFER_EXCLUDE_LOCKING
		void (*buffer_lock) (void);
		void (*buffer_unlock) (void);
#endif
	} ringbuffer;

	int8_t ringbuffer_init(ringbuffer * buffer,
			       uint8_t * data,
			       uint32_t dataSize,
			       uint32_t len,
			       void *(*user_memcpy) (void *str1,
						     const void *str2, size_t n)
#ifndef RINGBUFFER_EXCLUDE_LOCKING
			       , void (*buffer_lock) (void),
			       void (*buffer_unlock) (void)
#endif
	    );

	uint32_t ringbuffer_length(ringbuffer * buffer);
	uint8_t ringbuffer_is_empty(ringbuffer * buffer);
	uint32_t ringbuffer_length_available(ringbuffer * buffer);
	uint32_t ringbufferMaxSize(ringbuffer * buffer);

	void ringbuffer_append(ringbuffer * buffer, uint8_t * data);
	void ringbuffer_append_multiple(ringbuffer * buffer, uint8_t * data,
					uint32_t len);

	void ringbuffer_peak(ringbuffer * buffer, uint8_t * data);
	void ringbuffer_get(ringbuffer * buffer, uint8_t * data);

	void ringbuffer_get_multiple(ringbuffer * buffer, uint8_t * dst,
				     uint32_t len);
	void ringbuffer_peak_multiple(ringbuffer * buffer, uint8_t * dst,
				      uint32_t len);

	void ringbuffer_discard_multiple(ringbuffer * buffer, uint32_t len);
	void ringbuffer_clear(ringbuffer * buffer);

#ifdef  __cplusplus
}
#endif
#endif
