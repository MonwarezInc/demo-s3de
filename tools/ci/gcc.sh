#!/bin/sh
export CC=gcc
export CXX=g++

BUILDIR=../build-demo-s3de

meson "${BUILDIR}"
cd "${BUILDIR}"

ninja
