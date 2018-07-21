#!/usr/bin/env bash
#echo '2simple4u' | sudo -S kill -9 $(echo '2simple4u' | sudo -S lsof -t -i:3170)
#gnome-terminal -x sh -c "/home/connor/Work/dspr-contentserver/dspr-contentserver; bash"
xdg-open http://www.deuspora.com:3170/dspr-frontend.html &
echo 'content server launched, browser launched, wasm deployed'