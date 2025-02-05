#version 330 core

out vec4 FragColor;
in vec2 texCord;

uniform sampler2D textures[3];
uniform int selTex;

void main(){
	FragColor = texture(textures[selTex], texCord);
}