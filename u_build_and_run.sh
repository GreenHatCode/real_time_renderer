#!/usr/bin/env bash

set -e

CLEAN=0
OFFLINE=0

while [[ $# -gt 0 ]]; do
    case "$1" in
        -c|--clean)
            CLEAN=1
            shift
            ;;
        -o|--offline)
            OFFLINE=1
            shift
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [-c|--clean] [-o|--offline]"
            exit 1
            ;;
    esac
done

if [[ $CLEAN -eq 1 ]]; then
    rm -rf build
fi

CMAKE_ARGS=("-DCMAKE_BUILD_TYPE=Debug")

if [[ $OFFLINE -eq 1 ]]; then
    CMAKE_ARGS+=("-DOFFLINE_BUILD=ON")
fi

cmake "${CMAKE_ARGS[@]}" -B build -S .
cmake --build build -j"$(nproc)"

echo "Run app"
exec ./build/real_time_renderer