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

void test_Append_Element_No_Round()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, &(elements[i]));
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_INT(data[i], elements[i]);
   }

}

void test_Append_Element_No_Round_Large_Array()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE*2] = {0xFF};
   uint16_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0])*2, BUFFER_SIZE, memcpy), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = 0x100+i;
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, (uint8_t*)&(elements[i]));
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_MEMORY(&(data[2*i]), (uint8_t*)&(elements[i]),2);
   }

}

void test_Append_Element_No_Round_Very_Large_Array()
{     
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE*4] = {0xFF};
   uint32_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0])*4, BUFFER_SIZE, memcpy), 1);

   for(uint8_t i=0; i<BUFFER_SIZE; i++) {
        elements[i] = 0x12345678+i;
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);
        ringBufferAppendOne(&buffer, (uint8_t*)&(elements[i]));
        TEST_ASSERT_EQUAL_INT(ringBufferLen(&buffer), i+1);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
       TEST_ASSERT_EQUAL_MEMORY(&(data[4*i]), (uint8_t*)&(elements[i]),4);
   }

}

void test_Append_Element_Head_In_The_middle()
{     
    
   RingBuffer buffer;
   uint8_t data[BUFFER_SIZE] = {0xFF};
   uint8_t elements[BUFFER_SIZE] = {0};

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy), 1);

   /* Simulate Insertions and removals */
   buffer.tail = buffer.head = BUFFER_SIZE/2;

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
        elements[i] = i;
        TEST_ASSERT_EQUAL_INT(elements[i], i);
   }

   for(uint8_t i=0; i<BUFFER_SIZE-1; i++) {
      TEST_ASSERT_EQUAL_INT(ringBufferLenAvailable(&buffer), BUFFER_SIZE-i-1);        
      ringBufferAppendOne(&buffer, &(elements[i]));
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

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy), 1);

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

   TEST_ASSERT_EQUAL_INT(ringBufferInit(&buffer, data, sizeof(data[0]), BUFFER_SIZE, memcpy), 1);

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
