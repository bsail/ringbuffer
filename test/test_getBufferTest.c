#include "unity.h"
#define RINGBUFFER_EXCLUDE_LOCKING   1
#include "ringbuffer.h"
#include <string.h>

#define BUFFER_SIZE 128

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Peak_One_After_Multiple_Insertions()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, elements[i]);
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
        TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 0);
        TEST_ASSERT_EQUAL_INT(ringBufferPeakOne(&buffer), elements[0]);
   }
}

void test_Get_One_After_Multiple_Insertions()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, elements[i]);
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
        TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 0);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), BUFFER_SIZE-i-1);
        TEST_ASSERT_EQUAL_INT(ringBufferGetOne(&buffer), elements[i]);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);
}

void test_Get_Multiple_After_Multiple_Insertions_No_Round()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferEmpty(&buffer), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, elements[i]);
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

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy), 1);

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
