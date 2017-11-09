//VERTEX SHADER

#version 330 core
layout (location = 0) in vec3 position;		  // vertex attribute 0 is a 3-dim vector
layout (location = 2) in float density;

uniform float pointSize;

out float dens;

// uniforms are global, unique per shader program object, accessable by any shader
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * transform * vec4(position, 1.0f);
	gl_PointSize = pointSize;
	dens = density;
}