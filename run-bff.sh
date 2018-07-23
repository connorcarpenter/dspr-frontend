#!/usr/bin/env bash
#!/usr/bin/env bash
echo '2simple4u' | sudo -S kill -9 $(echo '2simple4u' | sudo -S lsof -t -i:3172)
gnome-terminal -x sh -c "/home/connor/Work/dspr-bff/dspr-bff; bash"