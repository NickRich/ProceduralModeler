#version 330

in vec2 coord;
in vec3 position;

in mat4 modMat;
in mat4 viewMat;
in mat4 projMat;
in mat4 prevViewMat;

uniform sampler2D textureData;
uniform sampler3D depthBuff;

void main()
{
	gl_FragColor = texture(textureData, coord);
	vec4 depth = texture(depthBuff, position);

}
