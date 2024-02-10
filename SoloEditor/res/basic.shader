#shader vertex
#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 tex;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Model;

out vec2 texCoord;

void main()
{
	gl_Position =  u_Proj * u_View * u_Model * position;
	texCoord = tex;
}




#shader fragment
#version 430

layout(location = 0) out vec4 color;

in vec2 texCoord;

uniform vec4 u_Color;
uniform sampler2D u_Tex;

void main()
{
	color = texture(u_Tex, texCoord);
};