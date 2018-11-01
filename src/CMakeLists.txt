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
            RefIsoGrid.h
            ResourceList.h
            EconomyManager.h EconomyManager.cpp
            GraphicsManager.h GraphicsManager.cpp

            Unit/Unit.h Unit/Unit.cpp
            Unit/DyingUnit.h Unit/DyingUnit.cpp
            Unit/UnitManager.h Unit/UnitManager.cpp
            Unit/UnitOrder.h
            Unit/UnitTemplate.h Unit/UnitTemplate.cpp
            Unit/UnitTemplateCatalog.h Unit/UnitTemplateCatalog.cpp
            Unit/ConstructionQueue.h Unit/ConstructionQueue.cpp

            Unit/SpecificUnit/SpecificUnit.h Unit/SpecificUnit/SpecificUnit.cpp
            Unit/SpecificUnit/Manafount.h Unit/SpecificUnit/Manafount.cpp

            Effects/Manaball.h Effects/Manaball.cpp
            Effects/BloodParticle.h Effects/BloodParticle.cpp
            Effects/FloatingNumber.h Effects/FloatingNumber.cpp

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

            IsoBox/IsoBox.h IsoBox/IsoBox.cpp
            IsoBox/IsoBoxCache.h IsoBox/IsoBoxCache.cpp
    )
    oryol_shader(shaders.shd)
    fips_deps(Sova)
fips_end_app()
