# Ring Buffer

Ring Buffer Implementation in C.

Fork of original library by [filipecalasans](https://github.com/filipecalasans/ringbuffer).

Copyright (C) 2017 Filipe Calasans. All rights reserved.

Forked by Nikolay Nerovny, BMSTU, 2018.

This project targets to deliver a memory efficient implementation of
Ring BUffer in C. Performance aspects were privileged over safety, for we don't make safety checks before manipulating the buffer.  For example, we don't verify if the buffer is already full before inserting a new element, nor we verify if the buffer is empty before getting a new element. Users should be advised to do so in upper layers.

# Using RingBuffer in C

You need to add the sources locate at **src/** in your project. This implementation is well suited for embedded systems.

# Build Tests

The Makefile in the directory **tests/** builds the plain-c ring-buffer Unit Tests. 

Dependecies:

   * [libcheck](https://libcheck.github.io/check/)

