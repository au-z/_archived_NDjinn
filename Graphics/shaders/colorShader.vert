#version 130

in vec2 vertexPos;
in vec2 vertexUV;
in vec4 vertexColor;

out vec4 fragmentColor; 
out vec2 fragmentPosition;
out vec2 fragmentUV;

uniform mat4 P;

void main(){
	// xy is 'swizzling'. sets both x AND y
	gl_Position.xy = (P * vec4(vertexPos, 0, 1)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragmentPosition = vertexPos;
	fragmentColor = vertexColor;
	fragmentUV = vec2(vertexUV.x, -vertexUV.y);
}

