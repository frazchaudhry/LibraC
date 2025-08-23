//
// Created by Fraz Mahmud on 5/14/2025.
//

#include "libraCore.h"
#include <libraVideo.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


static constexpr GLuint TEXT_STARTING_BUFFER_SIZE = 3600; // 4(sizeof(float)) * 100(Vertices) * 9 (Number of float per vertex)

// =============================================SHADER===============================================================

void LC_GL_UseProgram(const uint32 programId) {
    glUseProgram(programId);
}

void LC_GL_SetUniformBool(const GLuint programId, const char *name, const bool value) {
    glUniform1i(glGetUniformLocation(programId, name), (int32)value);
}

void LC_GL_SetUniformInt(const GLuint programId, const char *name, const int32 value) {
    glUniform1i(glGetUniformLocation(programId, name), value);
}

void LC_GL_SetUniformFloat(const GLuint programId, const char *name, const float value) {
    glUniform1f(glGetUniformLocation(programId, name), value);
}

void LC_GL_SetUniformVec2(const GLuint programId, const char *name, const vec2 value) {
    glUniform2fv(glGetUniformLocation(programId, name), 1, &value[0]);
}

void LC_GL_SetUniformVec2f(const GLuint programId, const char *name, const float x, const float y) {
    glUniform2f(glGetUniformLocation(programId, name), x, y);
}

void LC_GL_SetUniformVec3(const GLuint programId, const char *name, const vec3 value) {
    glUniform3fv(glGetUniformLocation(programId, name), 1, &value[0]);
}

void LC_GL_SetUniformVec3f(const GLuint programId, const char *name, const float x, const float y, const float z) {
    glUniform3f(glGetUniformLocation(programId, name), x, y, z);
}

void LC_GL_SetUniformVec4(const GLuint programId, const char *name, const vec4 value) {
    glUniform4fv(glGetUniformLocation(programId, name), 1, &value[0]);
}

void LC_GL_SetUniformVec4f(const GLuint programId, const char *name, const float x, const float y, const float z,
                           const float w) {
    glUniform4f(glGetUniformLocation(programId, name), x, y, z, w);
}

void LC_GL_SetUniformMat2(const GLuint programId, const char *name, const mat2 *mat) {
    glUniformMatrix2fv(glGetUniformLocation(programId, name), 1, GL_FALSE, mat[0][0]);
}

void LC_GL_SetUniformMat3(const GLuint programId, const char *name, const mat3 *mat) {
    glUniformMatrix3fv(glGetUniformLocation(programId, name), 1, GL_FALSE, mat[0][0]);
}

void LC_GL_SetUniformMat4(const GLuint programId, const char *name, const mat4 *mat) {
    glUniformMatrix4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, mat[0][0]);
}

bool LC_GL_InitializeShader(LC_Arena *arena, LC_GL_Shader *shader, const char *vertexShaderPath,
                            const char *fragmentShaderPath, char *buffer) {
    char *vertexShaderSource = nullptr;
    char *fragmentShaderSource = nullptr;
    LC_GetFileContentString(arena, vertexShaderPath, &vertexShaderSource);
    LC_GetFileContentString(arena, fragmentShaderPath, &fragmentShaderSource);

    if (!vertexShaderSource) {
        sprintf(buffer, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: vertex-shader");
        return false;
    }
    if (!fragmentShaderSource) {
        free(vertexShaderSource);
        sprintf(buffer, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: fragment-shader");
        return false;
    }

    // Vertex Shader
    const uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (char const* const *)&vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    if (!CheckCompileErrors(vertexShader, "VERTEX", buffer)) return false;

    // Fragment Shader
    const uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (char const* const *)&fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    if (!CheckCompileErrors(fragmentShader, "FRAGMENT", buffer)) return false;

    // Shader Program
    shader->programId = glCreateProgram();
    glAttachShader(shader->programId, vertexShader);
    glAttachShader(shader->programId, fragmentShader);
    glLinkProgram(shader->programId);
    if (!CheckCompileErrors(shader->programId, "PROGRAM", buffer)) return false;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

bool CheckCompileErrors(const GLuint programId, char *type, char *buffer) {
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(programId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(programId, 1024, nullptr, infoLog);
            sprintf(buffer, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    else {
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programId, 1024, nullptr, infoLog);
            sprintf(buffer, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    return true;
}

// ==================================================================================================================

// =============================================Text Rendering=======================================================

bool LC_GL_InitializeTextRenderer(LC_Arena *arena, LC_GL_GameText *gameText, const char *fontName, const float fontSize,
                                  const char *vertexShaderPath, const char *fragmentShaderPath, char *errorLog) {
    constexpr size_t STARTING_ARENA_SIZE = 300 * 1024;
    void *localArenaBuffer = malloc(STARTING_ARENA_SIZE);
    LC_Arena localArena;
    LC_InitializeArena(&localArena, localArenaBuffer, STARTING_ARENA_SIZE);

    LC_GL_Shader shader;
    if (!LC_GL_InitializeShader(&localArena, &shader, vertexShaderPath, fragmentShaderPath, errorLog)) {
        printf("%s", errorLog);
        return false;
    }
    gameText->fontShaderProgramId = shader.programId;

    size_t fontFileSize;
    uchar *fontBuffer;
    LC_GetFileContentBinary(&localArena, fontName, &fontBuffer, &fontFileSize);

    const int32 fontCount = stbtt_GetNumberOfFonts(fontBuffer);
    if (fontCount == -1) {
        sprintf(errorLog, "\nThe font file doesn't correspond to valid font data");
        return false;
    }

    constexpr uint32 fontAtlasWidth = 512;
    constexpr uint32 fontAtlasHeight = 512;

    uchar *fontAtlasBitmap = LC_AllocateArena(&localArena, fontAtlasWidth * fontAtlasHeight * sizeof(uchar));
    constexpr uint32 codePointOfFirstCharacter = 32;
    constexpr uint32 charsToIncludeInFontAtlas = 95;

    gameText->fontSize = fontSize;
    gameText->codePointOfFirstCharacter = codePointOfFirstCharacter;
    gameText->charsToIncludeInFontAtlas = charsToIncludeInFontAtlas;

    gameText->packedChars = LC_AllocateArena(arena, sizeof(stbtt_packedchar) * charsToIncludeInFontAtlas);
    gameText->alignedQuads = LC_AllocateArena(arena, sizeof(stbtt_aligned_quad) * charsToIncludeInFontAtlas);

    stbtt_pack_context context;

    stbtt_PackBegin(&context, fontAtlasBitmap, fontAtlasWidth, fontAtlasHeight, 0, 1, nullptr);
    stbtt_PackFontRange(&context, fontBuffer, 0, gameText->fontSize, codePointOfFirstCharacter, charsToIncludeInFontAtlas,
                        gameText->packedChars);

    for (size_t i = 0; i < charsToIncludeInFontAtlas; i++) {
        float unusedX, unusedY;

        stbtt_GetPackedQuad(gameText->packedChars, fontAtlasWidth, fontAtlasHeight, (int32)i, &unusedX, &unusedY,
                            &gameText->alignedQuads[i], 0);
    }


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glCreateTextures(GL_TEXTURE_2D, 1, &gameText->fontAtlasTextureId);

    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(gameText->fontAtlasTextureId, 1, GL_R8, fontAtlasWidth, fontAtlasHeight);
    glTextureSubImage2D(gameText->fontAtlasTextureId, 0, 0, 0, fontAtlasWidth, fontAtlasHeight, GL_RED, GL_UNSIGNED_BYTE,
                        fontAtlasBitmap);

    LC_FreeAllArena(&localArena);
    free(localArenaBuffer);
    return true;
}

void LC_GL_InsertTextBytesIntoBuffer(float *buffer, const LC_GL_GameText *gameText, const LC_GL_Text *text) {
    const char codePointOfFirstCharacter = gameText->codePointOfFirstCharacter;
    const char charsToIncludeInFontAtlas = gameText->charsToIncludeInFontAtlas;
    const float fontSize = gameText->fontSize;
    vec3 localPosition = { text->position[0], text->position[1], text->position[2] };
    uint32 bufferPosition = 0;

    for (size_t i = 0; i < text->string.length; i++) {
        const char ch = text->string.data[i];

        if (ch < codePointOfFirstCharacter || ch > codePointOfFirstCharacter + charsToIncludeInFontAtlas) {
            printf("\nCharacter '%c' with code point %d is not included in the font atlas", ch, (int)ch);
            continue;
        }
        // Handle newlines separately.
        if (ch == '\n') {
            // advance y by fontSize, reset x-coordinate
            localPosition[1] -= fontSize * text->scale;
            localPosition[0] = text->position[0];
            continue;
        }
        // Retrieve the data used to render a glyph of character 'ch'
        const stbtt_packedchar *packedChar = &gameText->packedChars[ch - codePointOfFirstCharacter];
        const stbtt_aligned_quad *alignedQuad = &gameText->alignedQuads[ch - codePointOfFirstCharacter];

        const uint8 characterWidth = packedChar->x1 - packedChar->x0;
        const uint8 characterHeight = packedChar->y1 - packedChar->y0;

        // Handle spaces by skipping them.
        if (ch == ' ') {
            // advance x by fontSize, no need to reset y-coordinate
            localPosition[0] += packedChar->xadvance * text->scale;
            continue;
        }

        // The units of the above struct fields are in pixels,
        // convert them to a unit of what we won't be multiplying to pixelScale
        const vec2 glyphSize =
        {
            (float)characterWidth * text->scale,
            (float)characterHeight * text->scale
        };

        const vec2 glyphBoundingBoxBottomLeft =
        {
            localPosition[0] + (packedChar->xoff * text->scale),
            localPosition[1] + (packedChar->yoff + (float)characterHeight) * text->scale
        };

        // The vertex order of a quad goes bottom left, bottom right, top left, top right.
        const vec2 glyphVertices[4] =
        {
            { glyphBoundingBoxBottomLeft[0], glyphBoundingBoxBottomLeft[1] },
            { glyphBoundingBoxBottomLeft[0] + glyphSize[0], glyphBoundingBoxBottomLeft[1] },
            { glyphBoundingBoxBottomLeft[0], glyphBoundingBoxBottomLeft[1] - glyphSize[1] },
            { glyphBoundingBoxBottomLeft[0] + glyphSize[0], glyphBoundingBoxBottomLeft[1] - glyphSize[1] },
        };

        const vec2 glyphTextureCoords[4] =
        {
            { alignedQuad->s0, alignedQuad->t1 },
            { alignedQuad->s1, alignedQuad->t1 },
            { alignedQuad->s0, alignedQuad->t0 },
            { alignedQuad->s1, alignedQuad->t0 },
        };

        // We need to fill the vertex buffer by 4 vertices to render a quad as we are rendering a quad as 2 triangles
        for (size_t j = 0; j < 4; j++) {
            // position
            buffer[bufferPosition++] = glyphVertices[j][0];
            buffer[bufferPosition++] = glyphVertices[j][1];
            buffer[bufferPosition++] = text->position[2];

            // color
            buffer[bufferPosition++] = text->color[0];
            buffer[bufferPosition++] = text->color[1];
            buffer[bufferPosition++] = text->color[2];
            buffer[bufferPosition++] = text->color[3];

            // texture coordinates
            buffer[bufferPosition++] = glyphTextureCoords[j][0];
            buffer[bufferPosition++] = glyphTextureCoords[j][1];
        }
        // Update the position to render the next glyph specified by packedChar->xadvance.
        localPosition[0] += packedChar->xadvance * text->scale;
    }
}

void LC_GL_SetupVaoAndVboText(LC_GL_GameText *gameText) {
    glCreateBuffers(1, &gameText->vbo);
    glNamedBufferStorage(gameText->vbo, TEXT_STARTING_BUFFER_SIZE, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &gameText->vao);
    constexpr GLuint vaoBindingPoint = 0;
    glVertexArrayVertexBuffer(gameText->vao, vaoBindingPoint, gameText->vbo, 0, 9 * sizeof(float));

    constexpr uint8 positionIndex = 0;
    constexpr uint8 colorIndex = 1;
    constexpr uint8 texCoordIndex = 2;

    glEnableVertexArrayAttrib(gameText->vao, positionIndex);
    glEnableVertexArrayAttrib(gameText->vao, colorIndex);
    glEnableVertexArrayAttrib(gameText->vao, texCoordIndex);

    glVertexArrayAttribFormat(gameText->vao, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(gameText->vao, colorIndex, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribFormat(gameText->vao, texCoordIndex, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float));

    glVertexArrayAttribBinding(gameText->vao, positionIndex, vaoBindingPoint);
    glVertexArrayAttribBinding(gameText->vao, colorIndex, vaoBindingPoint);
    glVertexArrayAttribBinding(gameText->vao, texCoordIndex, vaoBindingPoint);
}

void LC_GL_DrawText(LC_Arena *arena, LC_GL_GameState *gameState, const LC_GL_Text *text) {
    const TemporaryArenaMemory temporaryArenaMemory = LC_BeginTemporaryArenaMemory(arena);

    uint64 totalCharacters = LC_GetStringLengthSkipSpaces(&text->string);

    // Each quad has 4 vertices
    const uint32 MAX_QUADS = totalCharacters;
    const GLint totalVertices = (int32)MAX_QUADS * 4;
    constexpr uint32 NUMBER_OF_FLOATS_PER_VERTEX = 9; // 3 for position, 4 for color, 2 for texture coordinates
    const GLuint sizeOfBuffer = (int64)sizeof(float) * totalVertices * NUMBER_OF_FLOATS_PER_VERTEX;

    float *buffer = LC_AllocateArena(arena, sizeOfBuffer);

    LC_GL_InsertTextBytesIntoBuffer(buffer, gameState->gameText, text);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LC_GL_UseProgram(gameState->gameText->fontShaderProgramId);
    LC_GL_SetUniformInt(gameState->gameText->fontShaderProgramId, "fontAtlasTexture", 0);
    LC_GL_SetUniformMat4(gameState->gameText->fontShaderProgramId, "viewProjectionMatrix",
                         &gameState->viewProjectionMatrix);

    // Bind the Texture Unit
    glBindTextureUnit(0, gameState->gameText->fontAtlasTextureId);

    // Render here
    glBindVertexArray(gameState->gameText->vao);
    glNamedBufferSubData(gameState->gameText->vbo, 0, sizeOfBuffer, buffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, totalVertices);

    // Unbind the Texture Unit
    glBindTextureUnit(0, gameState->gameText->fontAtlasTextureId);

    // Unbind Vertex Array
    glBindVertexArray(gameState->gameText->vao);

    LC_EndTemporaryArena(temporaryArenaMemory);
}

void LC_GL_DeleteTextRenderer(const LC_GL_GameText *gameText) {
    glDeleteBuffers(1, &gameText->vao);
    glDeleteBuffers(1, &gameText->vbo);
    glDeleteTextures(1, &gameText->fontAtlasTextureId);
    glDeleteProgram(gameText->fontShaderProgramId);
}

// ==================================================================================================================

// =============================================Game Core============================================================

void LC_InitializeColor(const float red, const float green, const float blue, const float alpha, LC_Color *color) {
    color->r = red;
    color->g = green;
    color->b = blue;
    color->a = alpha;
}

LC_Color LC_CreateColor(const float red, const float green, const float blue, const float alpha) {
    const LC_Color color = {
        .r = red,
        .g = green,
        .b = blue,
        .a = alpha
    };
    return color;
}

int32 LC_GL_InitializeVideo(LC_Arena *arena, LC_GL_GameState *gameState, const char *title, const char *fontName,
                            char *errorLog) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        snprintf(errorLog, 1024, "Could not initialize SDLSubSystem Video: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_GL_LoadLibrary(nullptr);

    /* Set OpenGL settings. */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* Create the window */
    gameState->window = SDL_CreateWindow(title, gameState->screenWidth, gameState->screenHeight,
                                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!gameState->window) {
        snprintf(errorLog, 1024, "Couldn't create window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Get the OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(gameState->window);
    if (!glContext) {
        snprintf(errorLog, 1024, "GL Context Error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_GL_MakeCurrent(gameState->window, glContext);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        snprintf(errorLog, 1024, "Couldn't initialize GLAD: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

#if defined(DEBUG) || defined(_DEBUG)
    LC_GL_GetOpenGLVersionInfo();
#endif

    glViewport(0, 0, gameState->screenWidth, gameState->screenHeight);

    LC_GL_SetupViewProjectionMatrix2D(gameState->screenWidth, gameState->screenHeight, gameState->viewProjectionMatrix);

    LC_GL_InitializeTextRenderer(arena, gameState->gameText, fontName, 48.0f,
                                 "shaders/text.vert", "shaders/text.frag", errorLog);
    LC_GL_SetupVaoAndVboText(gameState->gameText);


    return EXIT_SUCCESS;
}

void LC_GL_FramebufferSizeCallback(const int32 width, const int32 height) {
    glViewport(0, 0, width, height);
}

void LC_GL_GetOpenGLVersionInfo() {
    printf("\n\nVendor: %s", (char*)glGetString(GL_VENDOR));
    printf("\nRenderer: %s", (char*)glGetString(GL_RENDERER));
    printf("\nVersion: %s", (char*)glGetString(GL_VERSION));
    printf("\nShading Language: %s\n\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void LC_GL_SetupViewProjectionMatrix2D(const int32 screenWidth, const int32 screenHeight, mat4 viewProjectionMatrix) {
    mat4 projection;
    glm_ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f, projection);
    mat4 view = GLM_MAT4_IDENTITY_INIT;
    vec3 translate = { 0.0f, 0.0f, 0.0f };
    glm_translate(view, translate);
    vec3 xAxis = { 1.0f, 0.0f, 0.0f };
    vec3 yAxis = { 0.0f, -1.0f, 0.0f };
    vec3 zAxis = { 0.0f, 0.0f, 1.0f };
    glm_rotate(view, 0.0f, xAxis);
    glm_rotate(view, 0.0f, yAxis);
    glm_rotate(view, 0.0f, zAxis);
    vec3 scale = { 1.0f, 1.0f, 1.0f };
    glm_scale(view, scale);

    glm_mat4_mul(projection, view, viewProjectionMatrix);
}

void LC_GL_ClearBackground(const LC_Color color) {
    glClearColor(color.r, color.b, color.g, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void LC_GL_FreeResources(const LC_GL_GameState *gameState) {
    LC_GL_DeleteTextRenderer(gameState->gameText);
    SDL_GLContext glContext = SDL_GL_GetCurrentContext();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(gameState->window);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

// ==================================================================================================================
