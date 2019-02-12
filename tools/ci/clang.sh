#!/bin/sh

export CC=clang
export CXX=clang++

BUILDIR=../build-demo-s3de

meson "${BUILDIR}"
cd "${BUILDIR}"
ninja
