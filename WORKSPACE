
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# Required for C++ unit testing
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz"],
  strip_prefix = "googletest-release-1.11.0",
  sha256 = "b4870bf121ff7795ba20d20bcdd8627b8e088f2d1dab299a031c1034eddc93d5"
)

# Starlark implementation of C++ rules in Bazel, recommended over native rules
http_archive(
  name = "rules_cc",
  urls = ["https://github.com/bazelbuild/rules_cc/archive/daf6ace7cfeacd6a83e9ff2ed659f416537b6c74.zip"],
  strip_prefix = "rules_cc-daf6ace7cfeacd6a83e9ff2ed659f416537b6c74",
  sha256 = "b295cad8c5899e371dde175079c0a2cdc0151f5127acc92366a8c986beb95c76"
)

# Required for shell smoke-testing
http_archive(
  name = "assert_sh",
  urls = ["https://github.com/torokmark/assert.sh/archive/a84514825439b1412e5718a786324ad3f354e0b1.zip"],
  strip_prefix = "assert.sh-a84514825439b1412e5718a786324ad3f354e0b1",
  sha256 = "75128e8c0eef017dacba06eca84bc572039da6fdd269c6d00a6de3a370a08906",
  build_file_content = """
sh_library(
  name = "assert",
  srcs = ["assert.sh"],
  visibility = ["//visibility:public"],
)"""
)
