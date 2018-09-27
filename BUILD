package(default_visibility = ["//visibility:public"])

load("@avr_tools//tools/avr:hex.bzl", "hex", "eeprom", "listing")
load("@utils//:unity_runner_generator.bzl", "cmock_generate")
load("@utils//:custom_macro.bzl", "custom_cc_library")
load("@utils//:custom_macro.bzl", "custom_cc_library_mock")
load("@utils//:unity_runner_generator.bzl", "unity_runner")
load("@utils//:unity_runner_generator.bzl", "UNITY_COPTS")
load("@utils//:unity_runner_generator.bzl", "UNITY_DEPS")

config_setting(
    name = "avr",
    values = {
        "cpu": "avr",
    },
)

################################################################################
## 
## Library: ringbuffer
## 
################################################################################

LIBRARY_NAME = "ringbuffer"
LIBRARY_VISIBILITY = ["//visibility:public"]
LIBRARY_DEPS = [
    ]

LIBRARY_TEST_UNITY_SIZE = "small"
LIBRARY_TEST_UNITY_DEPS = [
    "mock_mutex",
    ]
LIBRARY_CMOCK_DEPS = []

filegroup(
    name = LIBRARY_NAME+"_header",
    srcs = ["inc/"+LIBRARY_NAME+".h"],
)

custom_cc_library(
    name = LIBRARY_NAME,
    srcs = [
        "src/"+LIBRARY_NAME+".c",
        "inc/"+LIBRARY_NAME+".h",
        ],
    hdr = LIBRARY_NAME+"_header",
    deps = LIBRARY_DEPS,
    copts = [],
    visibility = LIBRARY_VISIBILITY,
)

custom_cc_library(
    name = LIBRARY_NAME+"_for_mocking",
    srcs = [
        "src/"+LIBRARY_NAME+".c",
        "inc/"+LIBRARY_NAME+".h",
    ],
    hdr = LIBRARY_NAME+"_header",
    deps = LIBRARY_TEST_UNITY_DEPS,
    copts = ["-DTEST"],
    testonly = True,
)

cmock_generate(
    prefix = LIBRARY_NAME,
    src = ":"+LIBRARY_NAME+"_header",
)

custom_cc_library_mock(
    srcs = [
    ],
    name = LIBRARY_NAME,
    hdr = LIBRARY_NAME+"_header",
    deps = LIBRARY_CMOCK_DEPS,
    visibility = LIBRARY_VISIBILITY,
    copts = ["-DTEST"],
    testonly = True,
)

LIBRARY_TEST_UNITY_MAIN_SOURCE = "test_"+LIBRARY_NAME+".c"

unity_runner(
    name = "runner_"+LIBRARY_NAME+"_test",
    src = "test/"+LIBRARY_TEST_UNITY_MAIN_SOURCE,
)

cc_test(
    name = LIBRARY_NAME+"_unity_test",
    srcs = [
        "test/"+LIBRARY_TEST_UNITY_MAIN_SOURCE,
        "runner_"+LIBRARY_NAME+"_test.c",
    ],
    copts = UNITY_COPTS + ["-DTEST"],
    size = LIBRARY_TEST_UNITY_SIZE,
    deps = [
        ":"+LIBRARY_NAME+"_for_mocking",
    ] + UNITY_DEPS + LIBRARY_TEST_UNITY_DEPS,
)

################################################################################
## 
## Library: mutex
## 
################################################################################

LIBRARY_NAME = "mutex"
LIBRARY_VISIBILITY = ["//visibility:public"]
LIBRARY_DEPS = [
    ]

LIBRARY_TEST_UNITY_SIZE = "small"
LIBRARY_TEST_UNITY_DEPS = []
LIBRARY_CMOCK_DEPS = []

filegroup(
    name = LIBRARY_NAME+"_header",
    srcs = ["inc/"+LIBRARY_NAME+".h"],
)

cmock_generate(
    prefix = LIBRARY_NAME,
    src = ":"+LIBRARY_NAME+"_header",
)

custom_cc_library_mock(
    srcs = [],
    name = LIBRARY_NAME,
    hdr = LIBRARY_NAME+"_header",
    deps = LIBRARY_CMOCK_DEPS,
    visibility = LIBRARY_VISIBILITY,
    copts = ["-DTEST"],
    testonly = True,
)
