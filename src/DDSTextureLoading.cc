//------------------------------------------------------------------------------
//  DDSTextureLoading.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "Core/Main.h"
#include "Core/Containers/StaticArray.h"
#include "IO/IO.h"
#include "HttpFS/HTTPFileSystem.h"
#include "LocalFS/LocalFileSystem.h"
#include "Gfx/Gfx.h"
#include "Assets/Gfx/ShapeBuilder.h"
#include "Assets/Gfx/TextureLoader.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shaders.h"

using namespace Oryol;

class DDSTextureLoadingApp : public App {
public:
    AppState::Code OnRunning();
    AppState::Code OnInit();
    AppState::Code OnCleanup();

    glm::mat4 computeMVP(const glm::vec3& pos);

    DrawState drawState;
    Id texture;
    Shader::vsParams vsParams;
    glm::mat4 view;
    glm::mat4 proj;
};
OryolMain(DDSTextureLoadingApp);

//------------------------------------------------------------------------------
AppState::Code
DDSTextureLoadingApp::OnInit() {

    // setup IO system
    IOSetup ioSetup;
#if ORYOL_EMSCRIPTEN
    ioSetup.FileSystems.Add("http", HTTPFileSystem::Creator());
    ioSetup.Assigns.Add("tex:", "http://localhost:8000/data/");
#else
    ioSetup.FileSystems.Add("file", LocalFileSystem::Creator());
    ioSetup.Assigns.Add("tex:", "root:data/");
#endif

    IO::Setup(ioSetup);

    // setup rendering system
    auto gfxSetup = GfxSetup::Window(600, 400, "Oryol DDS Loading Sample");
    gfxSetup.DefaultPassAction = PassAction::Clear(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    Gfx::Setup(gfxSetup);

    // setup resources
    Id shd = Gfx::CreateResource(Shader::Setup());

    TextureSetup texBluePrint;
    texBluePrint.Sampler.MinFilter = TextureFilterMode::Linear;
    texBluePrint.Sampler.MagFilter = TextureFilterMode::Linear;
    texBluePrint.Sampler.WrapU = TextureWrapMode::ClampToEdge;
    texBluePrint.Sampler.WrapV = TextureWrapMode::ClampToEdge;
    static const char* texturePath = "tex:texture.dds";
    this->texture = Gfx::LoadResource(TextureLoader::Create(TextureSetup::FromFile(texturePath, texBluePrint)));

    const glm::mat4 rot90 = glm::rotate(glm::mat4(), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    ShapeBuilder shapeBuilder;
    shapeBuilder.Layout = {
        { VertexAttr::Position, VertexFormat::Float3 },
        { VertexAttr::TexCoord0, VertexFormat::Float2 }
    };
    shapeBuilder.Transform(rot90).Plane(1.0f, 1.0f, 1.0f);
    this->drawState.Mesh[0] = Gfx::CreateResource(shapeBuilder.Build());
    auto ps = PipelineSetup::FromLayoutAndShader(shapeBuilder.Layout, shd);
    ps.DepthStencilState.DepthWriteEnabled = true;
    ps.DepthStencilState.DepthCmpFunc = CompareFunc::LessEqual;
    this->drawState.Pipeline = Gfx::CreateResource(ps);

    const float fbWidth = (const float) Gfx::DisplayAttrs().FramebufferWidth;
    const float fbHeight = (const float) Gfx::DisplayAttrs().FramebufferHeight;
    this->proj = glm::perspectiveFov(glm::radians(45.0f), fbWidth, fbHeight, -1.0f, 1.0f);
    //this->proj = glm::ortho(-300.0f, 300.0f, 300.0f, -300.0f, -1.0f, 1.0f);
    this->view = glm::mat4();
    
    return App::OnInit();
}

//------------------------------------------------------------------------------
AppState::Code
DDSTextureLoadingApp::OnRunning() {

    Gfx::BeginPass();

    // only render when texture is loaded (until texture placeholder are implemented)
    static const glm::vec3 pos = glm::vec3(0.0f, 0.0f, -1.0f);

    const auto resState = Gfx::QueryResourceInfo(this->texture).State;
    if (resState == ResourceState::Valid) {
        this->vsParams.mvp = this->computeMVP(pos);
        this->drawState.FSTexture[Shader::tex] = this->texture;
        Gfx::ApplyDrawState(this->drawState);
        Gfx::ApplyUniformBlock(this->vsParams);
        Gfx::Draw();
    }
    
    Gfx::EndPass();
    Gfx::CommitFrame();
    
    // continue running or quit?
    return Gfx::QuitRequested() ? AppState::Cleanup : AppState::Running;
}

//------------------------------------------------------------------------------
AppState::Code
DDSTextureLoadingApp::OnCleanup() {
    Gfx::Discard();
    IO::Discard();    
    return App::OnCleanup();
}

//------------------------------------------------------------------------------
glm::mat4
DDSTextureLoadingApp::computeMVP(const glm::vec3& pos) {
    glm::mat4 modelTform = glm::translate(glm::mat4(), pos);
    return this->proj * this->view * modelTform;
}

