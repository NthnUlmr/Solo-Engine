#shader vertex
#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Model;

out vec3 vertColor;

void main()
{
	gl_Position =  u_Proj * u_View * u_Model * position;
	vertColor = color;
}




#shader fragment
#version 430

layout(location = 0) out vec4 color;

in vec3 vertColor;

uniform vec4 u_Color;
uniform sampler2D u_Tex;

void main()
{
	color = vec4(vertColor,1.0);
};