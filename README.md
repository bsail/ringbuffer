# Ring Buffer

[![Build Status](https://img.shields.io/travis/bsail/ringbuffer/master.svg)](https://travis-ci.org/bsail/ringbuffer)
[![Coverage Status](https://img.shields.io/coveralls/github/bsail/ringbuffer/master.svg)](https://coveralls.io/github/bsail/ringbuffer?branch=master)

Ring Buffer Implementation in C.

Fork of original library by [filipecalasans](https://github.com/filipecalasans/ringbuffer).

Copyright (C) 2017 Filipe Calasans. All rights reserved.

Forked by Nikolay Nerovny, BMSTU, 2018.

The source code in `src/` directory is ANSI C compilant.

This project targets to deliver a memory efficient implementation of
Ring BUffer in C. Performance aspects were privileged over safety, for we don't make safety checks before manipulating the buffer.  For example, we don't verify if the buffer is already full before inserting a new element, nor we verify if the buffer is empty before getting a new element. Users should be advised to do so in upper layers.

# Using RingBuffer in C

You need to add the sources located at `src/` into your project's search path. This implementation is well suited for embedded systems.

# Build Tests

Use **ceedling test:all** to run unit tests.

Dependecies:

   * [Ceedling](https://github.com/ThrowTheSwitch/Ceedling)

