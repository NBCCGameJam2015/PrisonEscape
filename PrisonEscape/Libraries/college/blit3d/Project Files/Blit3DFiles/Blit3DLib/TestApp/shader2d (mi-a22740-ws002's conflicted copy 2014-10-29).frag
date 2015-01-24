#version 330 core

uniform sampler2D mytexture;

in vec2 v_texcoord;
uniform float in_Alpha;

out vec4 out_Color;

void main(void)
{	
	vec4 myTexel = 	texture2D(mytexture, v_texcoord);

    out_Color = myTexel * in_Alpha; 
}