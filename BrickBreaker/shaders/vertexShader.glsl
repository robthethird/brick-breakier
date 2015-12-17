#version 330
//position of a vertex
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 vertexNorm;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

out vec3 fragPos;
out vec3 fragNorm;
out vec2 fragTexUV;

void main()
{
	gl_Position = viewMatrix * worldMatrix * vec4(position, 1);

	fragPos = gl_Position.xyz;
	fragNorm = normalize(vertexNorm); //TODO rotation
	fragTexUV = vertexUV;
}
