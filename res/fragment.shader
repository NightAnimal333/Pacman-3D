#version 430 core

/** Inputs */
layout(location = 0) in vec2 vsTex;

/** Outputs */
layout(location = 0) out vec4 outColor;

//Uniforms
layout(location = 0) uniform sampler2D uTextureA;


vec3 lightning() {
    vec3 ambient = vec3(1.f);

    return ambient;
}

void main()
{
    vec4 tex = texture(uTextureA, vsTex);
    vec3 light = lightning();
    outColor = vec4(tex.rgb * light, tex.a);
    if (outColor.a < 0.1) {
        discard;
    }
}
