//
// Created by Fraz Mahmud on 5/14/2025.
//

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

#include <libraVideo.h>

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

bool LC_GL_InitializeShader(Arena *arena, LC_GL_Shader *shader, const char *vertexShaderPath,
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

bool LC_GL_InitializeTextRenderer(Arena *arena, LC_GL_GameText *gameText, const char *fontName, const float fontSize,
                                  const char *vertexShaderPath, const char *fragmentShaderPath, char *errorLog) {
    constexpr size_t STARTING_ARENA_SIZE = 300 * 1024;
    void *localArenaBuffer = malloc(STARTING_ARENA_SIZE);
    Arena localArena = { nullptr };
    LC_InitializeArena(&localArena, localArenaBuffer, STARTING_ARENA_SIZE);

    LC_GL_Shader shader;
    if (!LC_GL_InitializeShader(&localArena, &shader, vertexShaderPath, fragmentShaderPath, errorLog)) {
        printf(errorLog);
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
    return true;
}

void LC_GL_DeleteTextRenderer(const LC_GL_GameText *gameText) {
    glDeleteBuffers(1, &gameText->vao);
    glDeleteBuffers(1, &gameText->vbo);
    glDeleteTextures(1, &gameText->fontAtlasTextureId);
    glDeleteProgram(gameText->fontShaderProgramId);
}

// ==================================================================================================================

// =============================================Game Core============================================================

int32 LC_GL_InitializeVideo(GLFWwindow **window, const int32 screenWidth, const int32 screenHeight, const char *title,
    char *errorLog) {
    if (!glfwInit()) {
        const char *errorDesc;
        glfwGetError(&errorDesc);
        sprintf(errorLog, "Couldn't initialize GLFW: %s", errorDesc);
        return EXIT_FAILURE;
    }

    /* Set OpenGL settings. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create the window */
    *window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);
    if (!*window) {
        const char *errorDesc;
        glfwGetError(&errorDesc);
        sprintf(errorLog, "Couldn't create window and renderer: %s", errorLog);
        return EXIT_FAILURE;
    }

    // Get the OpenGL context
    glfwMakeContextCurrent(*window);
    glfwSetFramebufferSizeCallback(*window, LC_GL_FramebufferSizeCallback);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        const char *errorDesc;
        glfwGetError(&errorDesc);
        sprintf(errorLog, "Couldn't initialize GLAD: %s", errorDesc);
        return EXIT_FAILURE;
    }

#if defined(DEBUG) || defined(_DEBUG)
    LC_GL_GetOpenGLVersionInfo();
#endif

    glViewport(0, 0, screenWidth, screenHeight);

    return EXIT_SUCCESS;
}

void LC_GL_FramebufferSizeCallback(GLFWwindow *window, const int32 width, const int32 height) {
    glViewport(0, 0, width, height);
}

void LC_GL_GetOpenGLVersionInfo() {
    printf("\n\nVendor: %s", (char*)glGetString(GL_VENDOR));
    printf("\nRenderer: %s", (char*)glGetString(GL_RENDERER));
    printf("\nVersion: %s", (char*)glGetString(GL_VERSION));
    printf("\nShading Language: %s\n\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

void LC_GL_SetupViewProjectionMatrix2D(int32 screenWidth, int32 screenHeight, mat4 viewProjectionMatrix) {
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

// ==================================================================================================================
