
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test", "cc_library")

cc_library(
    name = "lib",
    hdrs = ["lib.h"],
)

cc_binary(
    name = "main",
    srcs = ["main.cpp"],
    deps = [":lib"],
)

cc_test(
    name = "unit_tests",
    srcs = ["unit_tests.cpp"],
    deps = [
      "@com_google_googletest//:gtest_main", 
      ":lib"
    ],
)

sh_test(
    name = "smoke_tests",
    srcs = ["smoke_tests.sh"],
    data = glob(["testdata/*.txt"]) + [":main", "@assert_sh//:assert"],
)
