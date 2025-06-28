//
// Created by Fraz Mahmud on 5/14/2025.
//

#ifndef LIBRAVIDEO_H
#define LIBRAVIDEO_H

#include <external/stb_truetype.h>

#include <libraC.h>
#include "libraCore.h"

// =============================================SHADER===============================================================

typedef struct shader {
    GLuint programId;
} LC_GL_Shader;

bool LC_GL_InitializeShader(Arena *arena, LC_GL_Shader *shader, const char *vertexShaderPath,
                            const char *fragmentShaderPath,
                            char *buffer);

void LC_GL_UseProgram(GLuint programId);

void LC_GL_SetUniformBool(GLuint programId, const char *name, bool value);
void LC_GL_SetUniformInt(GLuint programId, const char *name, int32 value);
void LC_GL_SetUniformFloat(GLuint programId, const char *name, float value);
void LC_GL_SetUniformVec2(GLuint programId, const char *name, const vec2 value);
void LC_GL_SetUniformVec2f(GLuint programId, const char *name, float x, float y);
void LC_GL_SetUniformVec3(GLuint programId, const char *name, const vec3 value);
void LC_GL_SetUniformVec3f(GLuint programId, const char *name, float x, float y, float z);
void LC_GL_SetUniformVec4(GLuint programId, const char *name, const vec4 value);
void LC_GL_SetUniformVec4f(GLuint programId, const char *name, float x, float y, float z,
                           float w);
void LC_GL_SetUniformMat2(GLuint programId, const char *name, const mat2 *mat);
void LC_GL_SetUniformMat3(GLuint programId, const char *name, const mat3 *mat);
void LC_GL_SetUniformMat4(GLuint programId, const char *name, const mat4 *mat);

bool CheckCompileErrors(GLuint programId, char *type, char *buffer);

// ==================================================================================================================

// =============================================Text Rendering=======================================================

typedef struct gameText {
    GLuint vao;
    GLuint vbo;
    GLuint fontAtlasTextureId;
    GLuint fontShaderProgramId;
    char *fontName;
    float fontSize;
    uint32 codePointOfFirstCharacter;
    uint32 charsToIncludeInFontAtlas;
    stbtt_packedchar *packedChars;
    stbtt_aligned_quad *alignedQuads;
} LC_GL_GameText;

bool LC_GL_InitializeTextRenderer(Arena *arena, LC_GL_GameText *gameText, const char *fontName, float fontSize,
                                   const char *vertexShaderPath, const char *fragmentShaderPath, char *errorLog);

void LC_GL_FramebufferSizeCallback(GLFWwindow *window, int32 width, int32 height);
void LC_GL_GetOpenGLVersionInfo();
void LC_GL_DeleteTextRenderer(const LC_GL_GameText *gameText);

// ==================================================================================================================

// =============================================Game Core============================================================

typedef struct gameState {
    GLFWwindow *window;
    mat4 viewProjectionMatrix;
    LC_GL_GameText *gameText;
} LC_GL_GameState;

int32 LC_GL_InitializeVideo(GLFWwindow **window, int32 screenWidth, int32 screenHeight, const char *title,
    char *errorLog);
void LC_GL_FramebufferSizeCallback(GLFWwindow *window, int32 width, int32 height);
void LC_GL_SetupViewProjectionMatrix2D(int32 screenWidth, int32 screenHeight, mat4 viewProjectionMatrix);

// ==================================================================================================================

#endif //LIBRAVIDEO_H
