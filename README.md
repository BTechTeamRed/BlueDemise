# BlueDemise Game Engine

<a name="build"></a>
# How to build
1. You'll need to download the latest version of CMake https://cmake.org/download/
    (Make sure you add cmake to your PATH)
2. Open console and navigate to the root directory of the project.
3. Run `cmake .` This will configure the CMake files. For more options https://cmake.org/cmake/help/latest/manual/cmake.1.html#generate-a-project-buildsystem
4. Run `cmake -Dtest=ON` to build the test project. It won't build otherwise
5. Run `cmake --build .` to build the project

The project should now be built according to the configure used in step 3.

<a name="advanced-shader-template"></a>
# How to construct an advanced shader
1. You must use the .as file extension to extract the source for an advanced shader.
2. The advanced shader uses a custom syntax that gets merged with the default shaders
	in the background. The following is a template for the advanced shader syntax:
	
Example translated from TextureFill.vs and TextureFill.fs
Illegal syntax warning: Do not use [] characters in comments

Template:

[vertex]
[layout]
(location=0) in vec3 pos;
(location=1) in vec2 vertexUV;

[uniform]
mat4 mvp;

[out]
vec2 uv;

[main]
gl_Position = mvp * vec4(pos, 1);
uv = vertexUV;

[fragment]
[uniform]
vec4 col;
sampler2D myTextureSampler;

[in]
vec2 uv;

[out]
vec4 color;

[main]
color = texture(myTextureSampler, uv).rgba * col;