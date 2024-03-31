#shader vertex
#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Model;

out vec4 vertColor;

void main()
{
	gl_Position =  u_Proj * u_View * u_Model * vec4(position,1.0);
	vertColor = vec4(color, 1.0);// color;
}




#shader fragment
#version 430

out vec4 color;

in vec4 vertColor;

uniform vec4 u_Color;
uniform sampler2D u_Tex;

void main()
{
	color = vec4(vertColor);
};