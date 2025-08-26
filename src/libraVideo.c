//
// Created by Fraz Mahmud on 5/14/2025.
//

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
    TemporaryArenaMemory localArena = LC_BeginTemporaryArenaMemory(arena);

    char *vertexShaderSource = nullptr;
    char *fragmentShaderSource = nullptr;
    LC_GetFileContentString(localArena.arena, vertexShaderPath, &vertexShaderSource);
    LC_GetFileContentString(localArena.arena, fragmentShaderPath, &fragmentShaderSource);

    if (!vertexShaderSource) {
        snprintf(buffer, 1024, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: vertex-shader");
        return false;
    }
    if (!fragmentShaderSource) {
        free(vertexShaderSource);
        snprintf(buffer, 1024, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: fragment-shader");
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

    LC_EndTemporaryArena(localArena);

    return true;
}

bool CheckCompileErrors(const GLuint programId, char *type, char *buffer) {
    int success;
    char infoLog[512];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(programId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(programId, 1024, nullptr, infoLog);
            snprintf(buffer, 1024, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    else {
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(programId, 1024, nullptr, infoLog);
            snprintf(buffer, 1024, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    return true;
}

// ==================================================================================================================

// =============================================Text Rendering=======================================================

bool LC_GL_InitializeTextRenderer(LC_Arena *arena, LC_GL_Renderer *renderer, const char *fontName, const float fontSize,
                                  const char *vertexShaderPath, const char *fragmentShaderPath, char *errorLog) {
    constexpr size_t STARTING_ARENA_SIZE = 300 * 1024;
    void *localArenaBuffer = malloc(STARTING_ARENA_SIZE);
    LC_Arena localArena;
    LC_InitializeArena(&localArena, localArenaBuffer, STARTING_ARENA_SIZE);

    LC_GL_Shader shader;
    if (!LC_GL_InitializeShader(&localArena, &shader, vertexShaderPath, fragmentShaderPath, errorLog)) {
        SDL_Log("%s", errorLog);
        return false;
    }
    LC_GL_GameText *gameText = renderer->gameText;
    gameText->fontShaderProgramId = shader.programId;

    size_t fontFileSize;
    uchar *fontBuffer;
    LC_GetFileContentBinary(&localArena, fontName, &fontBuffer, &fontFileSize);

    const int32 fontCount = stbtt_GetNumberOfFonts(fontBuffer);
    if (fontCount == -1) {
        snprintf(errorLog, 1024, "The font file doesn't correspond to valid font data");
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

    LC_GL_IsDSAAvailable(renderer) ? LC_GL_CreateTextureTextDSA(gameText, fontAtlasWidth, fontAtlasHeight, fontAtlasBitmap) :
        LC_GL_CreateTextureTextNonDSA(gameText, fontAtlasWidth, fontAtlasHeight, fontAtlasBitmap);

    LC_FreeAllArena(&localArena);
    free(localArenaBuffer);
    return true;
}

void LC_GL_CreateTextureTextDSA(LC_GL_GameText *gameText, const int32 fontAtlasWidth, const int32 fontAtlasHeight,
                               const uchar *fontAtlasBitmap) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glCreateTextures(GL_TEXTURE_2D, 1, &gameText->fontAtlasTextureId);

    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(gameText->fontAtlasTextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // The given texture data is a single channel 1 byte per pixel data
    glTextureStorage2D(gameText->fontAtlasTextureId, 1, GL_R8, fontAtlasWidth, fontAtlasHeight);
    glTextureSubImage2D(gameText->fontAtlasTextureId, 0, 0, 0, fontAtlasWidth, fontAtlasHeight, GL_RED, GL_UNSIGNED_BYTE,
                        fontAtlasBitmap);
}

void LC_GL_CreateTextureTextNonDSA(LC_GL_GameText *gameText, int32 fontAtlasWidth, int32 fontAtlasHeight,
                               const uchar *fontAtlasBitmap) {
    glGenTextures(1, &gameText->fontAtlasTextureId);
    glBindTexture(GL_TEXTURE_2D, gameText->fontAtlasTextureId);

    // The given texture data is a single channel 1 byte per pixel data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, fontAtlasWidth, fontAtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, fontAtlasBitmap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Unbind Texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void LC_GL_SetupVaoAndVboTextDSA(LC_GL_GameText *gameText) {
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

void LC_GL_SetupVaoAndVboTextNonDSA(LC_GL_GameText *gameText) {
    // Setting up the VAO and VBO
    glGenBuffers(1, &gameText->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gameText->vbo);
    glBufferData(GL_ARRAY_BUFFER, TEXT_STARTING_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glGenVertexArrays(1, &gameText->vao);
    glBindVertexArray(gameText->vao);

    constexpr uint8 positionIndex = 0;
    constexpr uint8 colorIndex = 1;
    constexpr uint8 texCoordIndex = 2;

    // position attribute
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), nullptr);
    glEnableVertexAttribArray(positionIndex);

    // color attribute
    glVertexAttribPointer(colorIndex, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(colorIndex);

    // texCoord attribute
    glVertexAttribPointer(texCoordIndex, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(7 * sizeof(float)));
    glEnableVertexAttribArray(texCoordIndex);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LC_GL_RenderText(LC_GL_Renderer *renderer, const LC_GL_Text *text) {
    uint64 totalCharacters = LC_GetStringLengthSkipSpaces(&text->string);

    // Each quad has 4 vertices
    const uint32 MAX_QUADS = totalCharacters;
    const GLint totalVertices = (int32)MAX_QUADS * 4;
    constexpr uint32 NUMBER_OF_FLOATS_PER_VERTEX = 9; // 3 for position, 4 for color, 2 for texture coordinates
    const GLuint sizeOfBuffer = (int64)sizeof(float) * totalVertices * NUMBER_OF_FLOATS_PER_VERTEX;

    float buffer[sizeOfBuffer];

    LC_GL_InsertTextBytesIntoBuffer(buffer, renderer->gameText, text);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    LC_GL_UseProgram(renderer->gameText->fontShaderProgramId);

    LC_GL_IsDSAAvailable(renderer) ? LC_GL_RenderTextDSA(renderer, totalVertices, sizeOfBuffer, buffer) :
        LC_GL_RenderTextNonDSA(renderer, totalVertices, sizeOfBuffer, buffer);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    LC_GL_UseProgram(0);
}

void LC_GL_RenderTextDSA(LC_GL_Renderer *renderer, GLint totalVertices, GLuint sizeOfBuffer, const float *buffer) {
    LC_GL_SetUniformInt(renderer->gameText->fontShaderProgramId, "fontAtlasTexture", 0);
    LC_GL_SetUniformMat4(renderer->gameText->fontShaderProgramId, "viewProjectionMatrix",
                         &renderer->viewProjectionMatrix);

    // Bind the Texture Unit
    glBindTextureUnit(0, renderer->gameText->fontAtlasTextureId);

    // Render here
    glBindVertexArray(renderer->gameText->vao);
    glNamedBufferSubData(renderer->gameText->vbo, 0, sizeOfBuffer, buffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, totalVertices);

    // Unbind the Texture Unit
    glBindTextureUnit(0, 0);

    // Unbind Vertex Array
    glBindVertexArray(0);
}

void LC_GL_RenderTextNonDSA(LC_GL_Renderer *renderer, GLint totalVertices, GLuint sizeOfBuffer, const float *buffer) {
    // Bind the Texture
    glBindTexture(GL_TEXTURE_2D, renderer->gameText->fontAtlasTextureId);
    glActiveTexture(GL_TEXTURE0);

    LC_GL_SetUniformInt(renderer->gameText->fontShaderProgramId, "fontAtlasTexture", 0);
    LC_GL_SetUniformMat4(renderer->gameText->fontShaderProgramId, "viewProjectionMatrix",
                         &renderer->viewProjectionMatrix);
    // Render here
    glBindVertexArray(renderer->gameText->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->gameText->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfBuffer, buffer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, totalVertices);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
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
            SDL_Log("\nCharacter '%c' with code point %d is not included in the font atlas", ch, (int)ch);
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

int32 LC_GL_InitializeVideo(LC_Arena *arena, LC_GL_Renderer *renderer, const char *title, const char *fontName,
                            char *errorLog) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        snprintf(errorLog, 1024, "Could not initialize SDLSubSystem Video: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_GL_LoadLibrary(nullptr);

    /* Set OpenGL settings. */
    // We try to get the latest OpenGL version if available
    GLint majorVersion = 4;
    GLint minorVersion = 6;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    /* Create the window */
    renderer->window = SDL_CreateWindow(title, renderer->screenWidth, renderer->screenHeight,
                                         SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!renderer->window) {
        snprintf(errorLog, 1024, "Couldn't create window: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Get the OpenGL context
    SDL_GLContext glContext = SDL_GL_CreateContext(renderer->window);
    if (!glContext) {
        snprintf(errorLog, 1024, "GL Context Error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_GL_MakeCurrent(renderer->window, glContext);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        snprintf(errorLog, 1024, "Couldn't initialize GLAD: %s", SDL_GetError());
        return EXIT_FAILURE;
    }

#if defined(DEBUG) || defined(_DEBUG)
    LC_GL_GetOpenGLVersionInfo();
#endif

    // Set the actual OpenGL version
    glGetIntegerv(GL_MAJOR_VERSION, &renderer->glMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &renderer->glMinorVersion);

    glViewport(0, 0, renderer->screenWidth, renderer->screenHeight);

    LC_GL_SetupViewProjectionMatrix2D(renderer->screenWidth, renderer->screenHeight, renderer->viewProjectionMatrix);

    const char *defaultVertexShader = LC_GL_IsDSAAvailable(renderer) ? "shaders/default.vert" : "shaders/default330.vert";
    const char *defaultFragShader = LC_GL_IsDSAAvailable(renderer) ? "shaders/default.frag" : "shaders/default330.frag";
    LC_GL_SetupDefaultRectRenderer(arena, renderer, defaultVertexShader, defaultFragShader, errorLog);

    const char *textVertexShader = LC_GL_IsDSAAvailable(renderer) ? "shaders/text.vert" : "shaders/text330.vert";
    const char *textFragShader = LC_GL_IsDSAAvailable(renderer) ? "shaders/text.frag" : "shaders/text330.frag";
    LC_GL_InitializeTextRenderer(arena, renderer, fontName, 48.0f, textVertexShader, textFragShader, errorLog);

    LC_GL_IsDSAAvailable(renderer) ? LC_GL_SetupVaoAndVboTextDSA(renderer->gameText) :
        LC_GL_SetupVaoAndVboTextNonDSA(renderer->gameText);

    return EXIT_SUCCESS;
}

void LC_GL_FramebufferSizeCallback(const int32 width, const int32 height) {
    glViewport(0, 0, width, height);
}

void LC_GL_GetOpenGLVersionInfo() {
    SDL_Log("%s", "");
    SDL_Log("Vendor: %s", (char*)glGetString(GL_VENDOR));
    SDL_Log("Renderer: %s", (char*)glGetString(GL_RENDERER));
    SDL_Log("Version: %s", (char*)glGetString(GL_VERSION));
    SDL_Log("Shading Language: %s", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

bool LC_GL_IsDSAAvailable(LC_GL_Renderer *renderer) {
    if (renderer->glMajorVersion < 4) return false;
    if (renderer->glMajorVersion >= 4 && renderer->glMinorVersion < 5) return false;

    return true;
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

void LC_GL_SetupDefaultRectRenderer(LC_Arena *arena, LC_GL_Renderer *renderer, const char *vertexShaderPath, 
                              const char *fragmentShaderPath, char *errorLog) {
    LC_GL_Shader shader;
    if (!LC_GL_InitializeShader(arena, &shader, vertexShaderPath, fragmentShaderPath, errorLog)) {
        SDL_Log("%s", errorLog);
    }
    renderer->defaultShaderProgramId = shader.programId;

    // Setup VAO, VBO, EBO
    constexpr GLuint sizeOfBuffer = sizeof(float) * 4 * 7; // Size of float * number of vertices * stride(3 pos, 4 color)
    
    const uint32 indices[] = {
        0 , 1, 3,
        1, 2, 3
    };

    LC_GL_IsDSAAvailable(renderer) ? LC_GL_DefaultRectVAODSA(renderer, indices, sizeOfBuffer) :
        LC_GL_DefaultRectVAONonDSA(renderer, indices, sizeOfBuffer);
}

void LC_GL_DefaultRectVAODSA(LC_GL_Renderer *renderer, const uint32 *indices, const GLuint sizeOfBuffer) {
    glCreateBuffers(1, &renderer->defaultVertexBufferObject);
    glNamedBufferStorage(renderer->defaultVertexBufferObject, sizeOfBuffer, nullptr, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &renderer->defaultElementBufferObject);
    glNamedBufferStorage(renderer->defaultElementBufferObject, 6 * sizeof(uint32), indices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &renderer->defaultVertexArrayObject);
    const GLuint vaoBindingPoint = 0;
    glVertexArrayVertexBuffer(renderer->defaultVertexArrayObject, vaoBindingPoint, 
                              renderer->defaultVertexBufferObject, 0, 7 * sizeof(float));
    glVertexArrayElementBuffer(renderer->defaultVertexArrayObject, renderer->defaultElementBufferObject);

    constexpr uint8 positionIndex = 0;
    constexpr uint8 colorIndex = 1;

    glEnableVertexArrayAttrib(renderer->defaultVertexArrayObject, positionIndex);
    glEnableVertexArrayAttrib(renderer->defaultVertexArrayObject, colorIndex);

    glVertexArrayAttribFormat(renderer->defaultVertexArrayObject, positionIndex, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribFormat(renderer->defaultVertexArrayObject, colorIndex, 4, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

    glVertexArrayAttribBinding(renderer->defaultVertexArrayObject, positionIndex, vaoBindingPoint);
    glVertexArrayAttribBinding(renderer->defaultVertexArrayObject, colorIndex, vaoBindingPoint);
}

void LC_GL_DefaultRectVAONonDSA(LC_GL_Renderer *renderer, const uint32 *indices, GLuint sizeOfBuffer) {
    // Create the Vertex Buffer Object to store the vertex data
    glGenVertexArrays(1, &renderer->defaultVertexArrayObject);
    glGenBuffers(1, &renderer->defaultVertexBufferObject);
    glGenBuffers(1, &renderer->defaultElementBufferObject);
    // 1. Bind Vertex Array Object first before binding and configuring Vertex Buffer Object
    glBindVertexArray(renderer->defaultVertexArrayObject);

    // We bind the buffer object using the buffer type for the Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, renderer->defaultVertexBufferObject);

    // Copy the vertex data into the buffer's memory
    // With GL_STREAM_DRAW the data is set only once and used by the GPU at most a few times.
    // With GL_STATIC_DRAW the data is set only once and used many times
    // With GL_DYNAMIC_DRAW the data is changed a lot and used many times.
    glBufferData(GL_ARRAY_BUFFER, sizeOfBuffer, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->defaultElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void LC_GL_ClearBackground(const LC_Color color) {
    glClearColor(color.r/255.0f, color.b/255.0f, color.g/255.0f, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void LC_GL_RenderRectangle(LC_GL_Renderer *renderer, const LC_Rect *rect, const LC_Color *color) {
    // Identify and setup vertices for position and color
    const float vertices[] = {
        (float)rect->x + (float)rect->w, (float)rect->y, 0.0f, color->r, color->g, color->b, color->a, // Top Right
        (float)rect->x + (float)rect->w, (float)rect->y + (float)rect->h, 0.0f, color->r, color->g, color->b, color->a, // Bottom Right
        (float)rect->x, (float)rect->y + (float)rect->h, 0.0f, color->r, color->g, color->b, color->a, // Bottom Left
        (float)rect->x, (float)rect->y, 0.0f, (float)color->r, color->g, color->b, color->a, // Top Left
    };

    LC_GL_IsDSAAvailable(renderer) ? LC_GL_RenderRectDSA(renderer, vertices, sizeof(vertices)) :
        LC_GL_RenderRectNonDSA(renderer, vertices, sizeof(vertices));
}

void LC_GL_RenderRectDSA(LC_GL_Renderer *renderer, const float *buffer, uint32 sizeOfBuffer) {
    // Setup Before Render
    LC_GL_UseProgram(renderer->defaultShaderProgramId);
    LC_GL_SetUniformMat4(renderer->defaultShaderProgramId, "viewProjectionMatrix",
                         &renderer->viewProjectionMatrix);
    glBindVertexArray(renderer->defaultVertexArrayObject);
    glNamedBufferSubData(renderer->defaultVertexBufferObject, 0, sizeOfBuffer, buffer);

    // Render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Unbind VAO
    glBindVertexArray(0);
}

void LC_GL_RenderRectNonDSA(LC_GL_Renderer *renderer, const float *buffer, uint32 sizeOfBuffer) {
    // Setup Before Render
    glUseProgram(renderer->defaultShaderProgramId);
    LC_GL_SetUniformMat4(renderer->defaultShaderProgramId, "viewProjectionMatrix",
                         &renderer->viewProjectionMatrix);
    glBindVertexArray(renderer->defaultVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->defaultVertexBufferObject);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfBuffer, buffer);

    // Render
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

bool LC_GL_SwapBuffer(SDL_Window *window, char *errorLog) {
    if (!SDL_GL_SwapWindow(window)) {
        snprintf(errorLog, 1024, "Could not swap window!");
        return false;
    }
    return true;
}

void LC_GL_FreeResources(const LC_GL_Renderer *renderer) {
    LC_GL_DeleteTextRenderer(renderer->gameText);
    SDL_GLContext glContext = SDL_GL_GetCurrentContext();

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(renderer->window);

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

// ==================================================================================================================
