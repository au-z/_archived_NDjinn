#version 130

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragmentUV;

out vec4 color;

uniform float time;
uniform sampler2D texSampler;

void main(){
	vec4 textureColor = texture(texSampler, fragmentUV);
	color = textureColor * fragmentColor; // basic color blending (Multiply)
	color = vec4(
		fragmentColor.r * (cos(fragmentPosition.x * 4 + time) + 1) * 0.5,
		fragmentColor.g * (cos(fragmentPosition.y * 8 + time) + 1) * 0.5,
		fragmentColor.b * (sin(fragmentPosition.y * 2 + time) + 1) * 0.5,
		fragmentColor.a
		) + textureColor;
}