#!/usr/bin/env bash
./fips build linux-make-debug
rm -rf ../fips-deploy/dspr-frontend/content
cp -R content ../fips-deploy/dspr-frontend/content