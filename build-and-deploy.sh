#!/usr/bin/env bash
./fips build wasm-make-release
./fips run old-paclone-example wasm-make-release