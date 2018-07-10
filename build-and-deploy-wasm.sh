#!/usr/bin/env bash
./fips build wasm-make-release
rm -rf ../fips-deploy/dspr-frontend/wasm-make-release/content
cp -R content ../fips-deploy/dspr-frontend/wasm-make-release/content
sudo kill -9 $(sudo lsof -t -i:8000)
./fips run dspr-frontend wasm-make-release