fips_begin_app(dspr-frontend windowed)
    fips_vs_warning_level(3)
    fips_files(
            Main.cpp
            Controller.h
            DsprShaderHandler.h DsprShaderHandler.cpp
            TileManager.h TileManager.cpp
            UnitManager.h UnitManager.cpp
            NetworkManager.h NetworkManager.cpp
            Tile.h Tile.cpp
            Unit.h Unit.cpp
            Cursor.h Cursor.cpp
            Global.h Global.cpp
            TestClass.h TestClass.cpp
    )
    oryol_shader(shaders.shd)
    fips_deps(Sova)
fips_end_app()
