#version 430 core

/** Inputs */
layout(location = 0) in vec3 aPosition;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 aTex;


/** Uniforms */
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

/** Outputs */
layout(location = 0) out vec2 vsTex;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPosition, 1.0);
    vsTex = aTex;
}
