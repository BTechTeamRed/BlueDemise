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
<ol>
<li>You must use the .as file extension to extract the source for an advanced shader.</li>
<li>The advanced shader uses a custom syntax that gets merged with the default shaders
	in the background. The following is a template for the advanced shader syntax:</li>
</ol>
	
<i>Example translated from TextureFill.vs and TextureFill.fs</i><br />
<b>Illegal syntax warning: Do not use [] characters in comments</b>

Template:

<span style="color: black">[vertex]</span><br />
<span style="color: black">[layout]</span><br />
(location=0) in vec3 pos;<br />
(location=1) in vec2 vertexUV;<br />

<span style="color: black">[uniform]</span><br />
mat4 mvp;

<span style="color: black">[out]</span><br />
vec2 uv;

<span style="color: black">[main]</span><br />
gl_Position = mvp * vec4(pos, 1);
uv = vertexUV;

<span style="color: black">[fragment]</span><br />
<span style="color: black">[uniform]</span><br />
vec4 col;<br />
sampler2D myTextureSampler;

<span style="color: black">[in]</span><br />
vec2 uv;

<span style="color: black">[out]</span><br />
vec4 color;

<span style="color: black">[main]</span><br />
color = texture(myTextureSampler, uv).rgba * col;