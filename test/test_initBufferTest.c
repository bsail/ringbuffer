#include "unity.h"
#include "ringbuffer.h"
#include <string.h>
#include "mock_mutex.h"

#define BUFFER_SIZE 256

void setUp(void)
{
  lock_Ignore();
  unlock_Ignore();
}

void tearDown(void)
{
}

void test_Init_Buffer_Size_multiple_of_2()
{     
   RingBuffer buffer;
   uint8_t data[128];
   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy,lock,unlock), 1);
   TEST_ASSERT_EQUAL_INT(0xFF, buffer.sizeMask);
   TEST_ASSERT_EQUAL_INT(buffer.tail, 0);
   TEST_ASSERT_EQUAL_INT(buffer.head, 0);
   TEST_ASSERT_EQUAL_PTR(buffer.data, data);
}

void test_Init_Buffer_Size_Not_multiple_of_2()
{     
   RingBuffer buffer;
   uint8_t data[128];
   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), 126, memcpy,lock,unlock), 0);
}
