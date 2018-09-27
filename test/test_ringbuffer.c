#include "unity.h"
#include "ringbuffer.h"
#include <string.h>
#include "mock_mutex.h"

#define BUFFER_SIZE 128
#define BUFFER_SIZE_LARGE 256

int GlobalVerifyOrder;
int GlobalExpectCount;

void setUp(void)
{
	GlobalVerifyOrder = 0;
	GlobalExpectCount = 0;
	lock_Ignore();
	unlock_Ignore();
}

void tearDown(void)
{
}

void test_Append_Element_No_Round()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, &(elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(data[i], elements[i]);
	}

}

void test_Append_Element_No_Round_Large_Array()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE * 2] = { 0xFF };
	uint16_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]) * 2, BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = 0x100 + i;
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, (uint8_t *) & (elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_MEMORY(&(data[2 * i]),
					 (uint8_t *) & (elements[i]), 2);
	}

}

void test_Append_Element_No_Round_Very_Large_Array()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE * 4] = { 0xFF };
	uint32_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]) * 4, BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = 0x12345678 + i;
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, (uint8_t *) & (elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_MEMORY(&(data[4 * i]),
					 (uint8_t *) & (elements[i]), 4);
	}

}

void test_Append_Element_Head_In_The_middle()
{

	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	/* Simulate Insertions and removals */
	buffer.tail = buffer.head = BUFFER_SIZE / 2;

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, &(elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
	}

	for (uint8_t i = 0; i < (BUFFER_SIZE - 1) / 2; i++) {
		TEST_ASSERT_EQUAL_INT(data[BUFFER_SIZE / 2 + i], elements[i]);
	}

	for (uint8_t i = BUFFER_SIZE / 2; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(data[i - BUFFER_SIZE / 2], elements[i]);
	}

}

void test_Append_Multiple_Elements_No_Round()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
			      BUFFER_SIZE - 1);
	ringbuffer_append_multiple(&buffer, elements, BUFFER_SIZE - 1);
	TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), BUFFER_SIZE - 1);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(data[i], elements[i]);
	}

}

void test_Append_Multiple_Elements_Head_In_The_middle()
{

	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	/* Simulate Insertions and removals */
	buffer.tail = buffer.head = BUFFER_SIZE / 2;

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
			      BUFFER_SIZE - 1);
	ringbuffer_append_multiple(&buffer, elements, BUFFER_SIZE - 1);
	TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), BUFFER_SIZE - 1);

	for (uint8_t i = 0; i < (BUFFER_SIZE - 1) / 2; i++) {
		TEST_ASSERT_EQUAL_INT(data[BUFFER_SIZE / 2 + i], elements[i]);
	}

	for (uint8_t i = BUFFER_SIZE / 2; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(data[i - BUFFER_SIZE / 2], elements[i]);
	}

}

void test_Peak_One_After_Multiple_Insertions()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, &(elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
		TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 0);
		uint8_t temp = 0xFF;
		ringbuffer_peak(&buffer, &temp);
		TEST_ASSERT_EQUAL_INT(temp, elements[0]);
	}
}

void test_Get_One_After_Multiple_Insertions()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, &(elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
		TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 0);
	}

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer),
				      BUFFER_SIZE - i - 1);
		uint8_t temp = 0xFF;
		ringbuffer_get(&buffer, &temp);
		TEST_ASSERT_EQUAL_INT(temp, elements[i]);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);
}

void test_Get_Multiple_After_Multiple_Insertions_No_Round()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
				      BUFFER_SIZE - i - 1);
		ringbuffer_append(&buffer, &(elements[i]));
		TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), i + 1);
		TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 0);
	}

	uint8_t out[BUFFER_SIZE];

	ringbuffer_get_multiple(&buffer, out, ringbuffer_length(&buffer));
	TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), 0);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(out[i], elements[i]);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);
}

void test_Get_Multiple_After_Multiple_Insertions_Round()
{
	ringbuffer buffer;
	uint8_t data[BUFFER_SIZE] = { 0xFF };
	uint8_t elements[BUFFER_SIZE] = { 0 };

	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), BUFFER_SIZE,
			       memcpy, lock, unlock), 1);

	for (uint8_t i = 0; i < BUFFER_SIZE; i++) {
		elements[i] = i;
		TEST_ASSERT_EQUAL_INT(elements[i], i);
	}

	/* Simulate Insertions and removals */
	buffer.tail = buffer.head = BUFFER_SIZE / 2;

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);

	TEST_ASSERT_EQUAL_INT(ringbuffer_length_available(&buffer),
			      BUFFER_SIZE - 1);
	ringbuffer_append_multiple(&buffer, elements, BUFFER_SIZE - 1);
	TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), BUFFER_SIZE - 1);

	uint8_t out[BUFFER_SIZE];
	ringbuffer_get_multiple(&buffer, out, ringbuffer_length(&buffer));
	TEST_ASSERT_EQUAL_INT(ringbuffer_length(&buffer), 0);

	for (uint8_t i = 0; i < BUFFER_SIZE - 1; i++) {
		TEST_ASSERT_EQUAL_INT(out[i], elements[i]);
	}

	TEST_ASSERT_EQUAL_INT(ringbuffer_is_empty(&buffer), 1);
}

void test_Init_Buffer_Size_multiple_of_2()
{
	ringbuffer buffer;
	uint8_t data[128];
	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]),
			       BUFFER_SIZE_LARGE, memcpy, lock, unlock), 1);
	TEST_ASSERT_EQUAL_INT(0xFF, buffer.sizeMask);
	TEST_ASSERT_EQUAL_INT(buffer.tail, 0);
	TEST_ASSERT_EQUAL_INT(buffer.head, 0);
	TEST_ASSERT_EQUAL_PTR(buffer.data, data);
}

void test_Init_Buffer_Size_Not_multiple_of_2()
{
	ringbuffer buffer;
	uint8_t data[128];
	TEST_ASSERT_EQUAL_INT(ringbuffer_init
			      (&buffer, data, sizeof(data[0]), 126, memcpy,
			       lock, unlock), 0);
}
