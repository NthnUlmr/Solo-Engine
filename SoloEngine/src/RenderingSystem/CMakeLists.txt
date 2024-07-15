target_sources(SoloEngine PUBLIC 
"RenderingSystem.h"
"Image.h"
"Image.cpp"
"Texture.h"
"Texture.cpp"
"Texture3D.h"
"Texture3D.cpp"
"Uniform.h"
"Uniform.cpp"
"Shader.h"
"Shader.cpp"
"ShaderPipeline.h"
"ShaderPipeline.cpp"
"VertexArrayObject.h"
"VertexArrayObject.cpp"
"RenderComponents.h"
"Window.cpp"
"Window.h"
"CubeMap.h"
"CubeMap.cpp"
PRIVATE
"RenderingSystem.cpp"

)


 target_include_directories(SoloEngine PUBLIC ${CMAKE_CURRENT_LIST_DIR})