#pragma once

static const char *text_vert_shader = R"GLSL(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 2) in vec2 aTexCoords;


    uniform mat4 uOrthoProjection;
    uniform mat4 uModel;

    out vec2 TexCoords;


    void main()
    {
        vec4 pos = uModel * vec4(aPos, 1.0);
        pos.z += 0.1; // Offset text slightly forward to render on top of panels
        gl_Position = uOrthoProjection * pos;
        TexCoords = aTexCoords;
    }
)GLSL";

static const char *text_frag_shader = R"GLSL(
    #version 330 core
    out vec4 FragColor;

    in vec2 TexCoords;

    uniform sampler2D diffusedTexture;
    uniform vec3 text_color;

    void main()
    {
        vec2 TexCoordsFlipped = TexCoords;
        TexCoordsFlipped.y = 1.0 - TexCoords.y;

        float textAlpha = texture(diffusedTexture, TexCoordsFlipped).r;
        FragColor =  vec4(text_color, textAlpha);
    }  
)GLSL";
