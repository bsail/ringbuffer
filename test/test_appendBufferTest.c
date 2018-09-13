#include "unity.h"
#include "ringbuffer.h"
#include <string.h>
#include "mock_mutex.h"

#define BUFFER_SIZE 128

void setUp(void)
{
}

void tearDown(void)
{
}

void test_Append_Element_No_Round()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy,lock,unlock), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, elements[i]);
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_INT(data[i], elements[i]);
   }

}

void test_Append_Element_Head_In_The_middle()
{     
    
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy,lock,unlock), 1);

   /* Simulate Insertions and removals */
   buffer.tail = buffer.head = BUFFER_SIZE/2;

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
      TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);        
      ringBufferAppendOne(&buffer, elements[i]);
      TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
   }

   for(uint8_t i=0; i<(BUFFER_SIZE-1)/2; i++) {
       TEST_ASSERT_EQUAL_INT(data[BUFFER_SIZE/2 + i], elements[i]);
   }
   
   for(uint8_t i=BUFFER_SIZE/2; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_INT(data[i-BUFFER_SIZE/2], elements[i]);
   }

}


void test_Append_Multiple_Elements_No_Round()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy,lock,unlock), 1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-1);
   ringBufferAppendMultiple(&buffer, elements, BUFFER_SIZE-1);
   TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), BUFFER_SIZE-1);

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_INT(data[i], elements[i]);
   }

}

void test_Append_Multiple_Elements_Head_In_The_middle()
{     
    
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, BUFFER_SIZE, memcpy,lock,unlock), 1);

   /* Simulate Insertions and removals */
   buffer.tail = buffer.head = BUFFER_SIZE/2;

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }
   
   TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-1);
   ringBufferAppendMultiple(&buffer, elements, BUFFER_SIZE-1);
   TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), BUFFER_SIZE-1);

   for(uint8_t i=0; i<(BUFFER_SIZE-1)/2; i++) {
       TEST_ASSERT_EQUAL_INT(data[BUFFER_SIZE/2 + i], elements[i]);
   }

   for(uint8_t i=BUFFER_SIZE/2; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_INT(data[i-BUFFER_SIZE/2], elements[i]);
   }

}
