fips_begin_app(dspr-frontend windowed)
    fips_vs_warning_level(3)
    fips_files(
            Main.cpp
            Controller.h
            DsprShaderHandler.h DsprShaderHandler.cpp
            MoveSprite.h MoveSprite.cpp
    )
    oryol_shader(shaders.shd)
    fips_deps(Sova)
fips_end_app()
