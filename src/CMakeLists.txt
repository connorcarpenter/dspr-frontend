fips_begin_app(dspr-frontend windowed)
    fips_vs_warning_level(3)
    fips_files(
            Main.cpp
            Controller.h
            DsprShaderHandler.h DsprShaderHandler.cpp
            TileManager.h TileManager.cpp
            UnitManager.h UnitManager.cpp
            UnitOrder.h
            NetworkManager.h NetworkManager.cpp
            UiManager.h UiManager.cpp
            FogManager.h FogManager.cpp
            Tile.h Tile.cpp
            Block.h Block.cpp
            Unit.h Unit.cpp
            Cursor.h Cursor.cpp
            Global.h Global.cpp
            TestClass.h TestClass.cpp
            Minimap/Minimap.h Minimap/Minimap.cpp
            Minimap/MinimapUnits.h Minimap/MinimapUnits.cpp
            Minimap/MinimapFog.h Minimap/MinimapFog.cpp
    )
    oryol_shader(shaders.shd)
    fips_deps(Sova)
fips_end_app()
