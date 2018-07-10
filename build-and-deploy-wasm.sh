#!/usr/bin/env bash
./fips build wasm-make-release
rm -rf ../fips-deploy/dspr-frontend/wasm-make-release/content
cp -R content ../fips-deploy/dspr-frontend/wasm-make-release/content
./fips run dspr-frontend wasm-make-release