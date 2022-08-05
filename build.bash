#!/bin/bash
set -eo pipefail

mkdir -p ./build
pushd ./build

cmake .. -G Ninja
ninja
