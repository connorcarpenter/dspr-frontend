fips_begin_app(dspr-frontend windowed)
    fips_vs_warning_level(3)
    fips_files(
            Main.cpp
            Controller.h
            DsprShaderHandler.h DsprShaderHandler.cpp
            Tile.h Tile.cpp
            TileManager.h TileManager.cpp
            NetworkManager.h NetworkManager.cpp
            FogManager.h FogManager.cpp
            DsprColors.h DsprColors.cpp
            Block.h Block.cpp
            Global.h Global.cpp
            TestClass.h TestClass.cpp
            SpriteCatalog.h SpriteCatalog.cpp

            Unit/Unit.h Unit/Unit.cpp
            Unit/DyingUnit.h Unit/DyingUnit.cpp
            Unit/UnitManager.h Unit/UnitManager.cpp
            Unit/UnitOrder.h
            Unit/BloodParticle.h Unit/BloodParticle.cpp
            Unit/UnitTemplate.h Unit/UnitTemplate.cpp
            Unit/UnitTemplateCatalog.h Unit/UnitTemplateCatalog.cpp

            UI/UiManager.h UI/UiManager.cpp
            UI/Cursor.h UI/Cursor.cpp
            UI/ButtonCardCatalog.h UI/ButtonCardCatalog.cpp
            UI/ButtonCard.h UI/ButtonCard.cpp
            UI/ButtonActions.h UI/ButtonActions.cpp
            UI/Button.h UI/Button.cpp

            Minimap/Minimap.h Minimap/Minimap.cpp
            Minimap/MinimapUnits.h Minimap/MinimapUnits.cpp
            Minimap/MinimapFog.h Minimap/MinimapFog.cpp

            Circle/Circle.h Circle/Circle.cpp
            Circle/CircleCache.h Circle/CircleCache.cpp
    )
    oryol_shader(shaders.shd)
    fips_deps(Sova)
fips_end_app()
