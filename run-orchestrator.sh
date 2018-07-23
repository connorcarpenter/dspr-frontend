#!/usr/bin/env bash
echo '2simple4u' | sudo -S kill -9 $(echo '2simple4u' | sudo -S lsof -t -i:3171)
gnome-terminal -x sh -c "/home/connor/Work/dspr-orchestrator/dspr-orchestrator; bash"