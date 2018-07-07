# dspr-frontend

Notes:

- The following command will kill any app running on localhost:8000
        sudo kill -9 $(sudo lsof -t -i:8000)
        
        
--------

TODO:

- Assets/Gfx/TextureLoader.cc is a child of ResourceLoader, and loads dds files.
We gotta make one of those that loads PNG files