fips_begin_app(dspr-frontend windowed)
    fips_vs_warning_level(3)
    fips_files(
            Main.cpp
            Controller.h
            DsprShaderHandler.h DsprShaderHandler.cpp
            Game/Tile.h Game/Tile.cpp
            Game/TileManager.h Game/TileManager.cpp
            Game/FogManager.h Game/FogManager.cpp
            DsprColors.h DsprColors.cpp
            Game/Block.h Game/Block.cpp
            Game/Global.h Game/Global.cpp
            TestClass.h TestClass.cpp
            SpriteCatalog.h SpriteCatalog.cpp
            RefIsoGrid.h
            ResourceList.h
            Game/EconomyManager.h Game/EconomyManager.cpp
            GraphicsManager.h GraphicsManager.cpp

            Network/NetworkManager.h Network/NetworkManager.cpp
            Network/MessageSender.h Network/MessageSender.cpp
            Network/MessageReceiver.h Network/MessageReceiver.cpp
            Network/StringScanner.h Network/StringScanner.cpp

            DsprMessage/Main.h
            DsprMessage/ToServerMsg.h DsprMessage/ToServerMsg.cpp
            DsprMessage/ToClientMsg.h DsprMessage/ToClientMsg.cpp
            DsprMessage/Number.h DsprMessage/Number.cpp
            DsprMessage/Array.h DsprMessage/Array.cpp
            DsprMessage/CStr.h DsprMessage/CStr.cpp
            DsprMessage/CharVector.h DsprMessage/CharVector.cpp

            Game/Unit/Unit.h Game/Unit/Unit.cpp
            Game/Unit/DyingUnit.h Game/Unit/DyingUnit.cpp
            Game/Unit/UnitManager.h Game/Unit/UnitManager.cpp
            Game/Unit/UnitOrder.h
            Game/Unit/UnitTemplate.h Game/Unit/UnitTemplate.cpp
            Game/Unit/UnitTemplateCatalog.h Game/Unit/UnitTemplateCatalog.cpp
            Game/Unit/ConstructionQueue.h Game/Unit/ConstructionQueue.cpp
            Game/Item/Inventory.h Game/Item/Inventory.cpp
            Game/Unit/ShadowManager.h Game/Unit/ShadowManager.cpp

            Game/Unit/SpecificUnit/SpecificUnit.h Game/Unit/SpecificUnit/SpecificUnit.cpp
            Game/Unit/SpecificUnit/Manafount.h Game/Unit/SpecificUnit/Manafount.cpp

            Game/Item/Item.h Game/Item/Item.cpp
            Game/Item/ItemManager.h Game/Item/ItemManager.cpp
            Game/Item/ItemTemplate.h Game/Item/ItemTemplate.cpp
            Game/Item/ItemTemplateCatalog.h Game/Item/ItemTemplateCatalog.cpp

            Game/Effects/Manaball.h Game/Effects/Manaball.cpp
            Game/Effects/BloodParticle.h Game/Effects/BloodParticle.cpp
            Game/Effects/FloatingNumber.h Game/Effects/FloatingNumber.cpp
            Game/Effects/Sparkle.h Game/Effects/Sparkle.cpp
            Game/Effects/Projectile.h Game/Effects/Projectile.cpp

            Game/UI/UiManager.h Game/UI/UiManager.cpp
            Game/UI/Cursor.h Game/UI/Cursor.cpp
            Game/UI/ButtonCardCatalog.h Game/UI/ButtonCardCatalog.cpp
            Game/UI/ButtonCard.h Game/UI/ButtonCard.cpp
            Game/UI/ButtonActions.h Game/UI/ButtonActions.cpp
            Game/UI/Button.h Game/UI/Button.cpp
            Game/UI/ChatManager.h Game/UI/ChatManager.cpp

            Game/Minimap/Minimap.h Game/Minimap/Minimap.cpp
            Game/Minimap/MinimapUnits.h Game/Minimap/MinimapUnits.cpp
            Game/Minimap/MinimapFog.h Game/Minimap/MinimapFog.cpp

            Circle/Circle.h Circle/Circle.cpp
            Circle/CircleCache.h Circle/CircleCache.cpp

            IsoBox/IsoBox.h IsoBox/IsoBox.cpp
            IsoBox/IsoBoxCache.h IsoBox/IsoBoxCache.cpp
    )
    oryol_shader(shaders.shd)
    fips_deps(Sova)
fips_end_app()
