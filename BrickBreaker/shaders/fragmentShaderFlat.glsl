#version 330
//uniform vec4 tint; //from C++

uniform sampler2D uniformTex;

in vec3 fragPos;
in vec3 fragNorm;
in vec2 fragTexUV;

void main()
{
	vec4 color = texture(uniformTex, fragTexUV);

	vec3 lightDir = vec3(1, 1, 1);
	float lamb = max(dot(lightDir, fragNorm), 0);
	vec3 viewDir = normalize(-fragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float specAngle = max(dot(halfDir, fragNorm), 0);
	float specular = pow(specAngle, 16.0);
	float light = 1;

	gl_FragColor = vec4(color.rgb * light, 1);

}
