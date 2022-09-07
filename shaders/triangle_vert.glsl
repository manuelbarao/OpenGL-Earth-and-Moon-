#version 330 core
layout (location = 0) in vec3 InPosition;

out float Height;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 ModelViewProjection;

uniform mat4 ModelViewMatrix;

void main()
{
    Height = InPosition.y;
    vec4 ViewPosition = ModelViewMatrix * vec4(InPosition, 1.0);

	Position = ViewPosition.xyz / ViewPosition.w;
    gl_Position = ModelViewProjection * vec4(InPosition, 1.0);
}