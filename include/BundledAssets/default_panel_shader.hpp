#pragma once

static const char *panel_vert_shader = R"GLSL(
    #version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 uOrthoProjection;
uniform mat4 uModel;

out vec2 TexCoords;

void main()
{
    gl_Position = uOrthoProjection * uModel * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}
)GLSL";

static const char *panel_frag_shader = R"GLSL(
    #version 330 core
    out vec4 FragColor;  
    in vec2 TexCoords;
    
    uniform sampler2D ourTexture;
    uniform vec4 color;
    
    void main()
    {
        FragColor = color;
    }
)GLSL";
