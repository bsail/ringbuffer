#include "unity.h"
#include "ringbuffer.h"
#include <string.h>
#include "mock_mutex.h"

#define BUFFER_SIZE 128

void setUp(void)
{
  lock_Ignore();
  unlock_Ignore();
}

void tearDown(void)
{
}

void test_Peak_One_After_Multiple_Insertions()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy,lock,unlock), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, &(elements[i]));
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
        TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 0);
        uint8_t temp = 0xFF;
        ringBufferPeakOne(&buffer,&temp);
        TEST_ASSERT_EQUAL_INT(temp, elements[0]);
   }
}

void test_Get_One_After_Multiple_Insertions()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy,lock,unlock), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, &(elements[i]));
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
        TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 0);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), BUFFER_SIZE-i-1);
        uint8_t temp = 0xFF;
        ringBufferGetOne(&buffer,&temp);
        TEST_ASSERT_EQUAL_INT(temp, elements[i]);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);
}

void test_Get_Multiple_After_Multiple_Insertions_No_Round()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy,lock,unlock), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, &(elements[i]));
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
        TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 0);
   }

   uint8_t out[BUFFER_SIZE];

   ringBufferGetMultiple(&buffer, out, ringBufferLen(&buffer));
   TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), 0);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(out[i], elements[i]);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);
}

void test_Get_Multiple_After_Multiple_Insertions_Round()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy,lock,unlock), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   /* Simulate Insertions and removals */
   buffer.tail = buffer.head = BUFFER_SIZE/2;

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-1);
   ringBufferAppendMultiple(&buffer, elements, BUFFER_SIZE-1);
   TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), BUFFER_SIZE-1);

   uint8_t out[BUFFER_SIZE];
   ringBufferGetMultiple(&buffer, out, ringBufferLen(&buffer));
   TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), 0);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(out[i], elements[i]);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);
}
