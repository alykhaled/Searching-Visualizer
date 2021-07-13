#version 410 core

layout(location = 0) out vec4 color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_texture;

void main()
{

	//vec4 texColor = texture(u_texture, v_TexCoord);
	color = u_Color;
};