#!/usr/bin/bash

#apt-get update && apt-get install -y cmake git curl zip unzip && \
#git clone https://github.com/microsoft/vcpkg.git /vcpkg && /vcpkg/bootstrap-vcpkg.sh && \


podman run --rm --security-opt label=disable \
  -v "$(pwd)":/src -w /src emscripten/emsdk:latest \
  bash -c "mkdir -p build && cd build && \
  emcmake cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DVCPKG_TARGET_TRIPLET=wasm32-emscripten && \
  cmake --build . -j$(nproc)"
