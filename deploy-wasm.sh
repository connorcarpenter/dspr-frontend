#!/usr/bin/env bash
echo '2simple4u' | sudo -S kill -9 $(echo '2simple4u' | sudo -S lsof -t -i:8000)
./fips run dspr-frontend wasm-make-release