//
// Created by Fraz Mahmud on 5/14/2025.
//

#ifndef LIBRAVIDEO_H
#define LIBRAVIDEO_H

#include <stb_truetype.h>

#include <libraC.h>
#include "libraCore.h"

// =============================================STRUCTS==============================================================
// SHADER
typedef struct shader {
    GLuint programId;
} LC_GL_Shader;

// TEXT RENDERING
typedef struct text {
    LC_String string;
    vec3 position;
    vec4 color;
    float scale;
} LC_GL_Text;

typedef struct gameText {
    GLuint vao;
    GLuint vbo;
    GLuint fontAtlasTextureId;
    GLuint fontShaderProgramId;
    char *fontName;
    float fontSize;
    char codePointOfFirstCharacter;
    char charsToIncludeInFontAtlas;
    stbtt_packedchar *packedChars;
    stbtt_aligned_quad *alignedQuads;
} LC_GL_GameText;

// GAME CORE
typedef struct color {
    float r;    // Value between 0.0f and 255.0f
    float b;    // Value between 0.0f and 255.0f
    float g;    // Value between 0.0f and 255.0f
    float a;    // Value between 0.0f and 1.0f
} LC_Color;

typedef struct {
    int32 x;
    int32 y;
    int32 w;
    int32 h;
} LC_Rect;

typedef struct renderer_gl {
    int32 screenWidth;
    int32 screenHeight;
    SDL_Window *window;
    mat4 viewProjectionMatrix;
    GLuint defaultShaderProgramId;
    GLuint defaultVertexArrayObject;
    GLuint defaultVertexBufferObject;
    GLuint defaultElementBufferObject;
    LC_GL_GameText *gameText;
    GLint glMajorVersion;
    GLint glMinorVersion;
} LC_GL_Renderer;

// =============================================SHADER===============================================================

bool LC_GL_InitializeShader(LC_Arena *arena, LC_GL_Shader *shader, const char *vertexShaderPath,
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

bool LC_GL_InitializeTextRenderer(LC_Arena *arena, LC_GL_Renderer *renderer, const char *fontName, float fontSize,
                                  const char *vertexShaderPath, const char *fragmentShaderPath, char *errorLog);
void LC_GL_CreateTextureTextDSA(LC_GL_GameText *gameText, int32 fontAtlasWidth, int32 fontAtlasHeight,
                               const uchar *fontAtlasBitmap);
void LC_GL_CreateTextureTextNonDSA(LC_GL_GameText *gameText, int32 fontAtlasWidth, int32 fontAtlasHeight,
                               const uchar *fontAtlasBitmap);
void LC_GL_SetupVaoAndVboTextDSA(LC_GL_GameText *gameText);
void LC_GL_SetupVaoAndVboTextNonDSA(LC_GL_GameText *gameText);
void LC_GL_RenderText(LC_GL_Renderer *renderer, const LC_GL_Text *text);
void LC_GL_RenderTextDSA(LC_GL_Renderer *renderer, GLint totalVertices, GLuint sizeOfBuffer, const float *buffer);
void LC_GL_RenderTextNonDSA(LC_GL_Renderer *renderer, GLint totalVertices, GLuint sizeOfBuffer, const float *buffer);
void LC_GL_InsertTextBytesIntoBuffer(float *buffer, const LC_GL_GameText *gameText,
                                     const LC_GL_Text *text);

// ==================================================================================================================

// =============================================Game Core============================================================

void LC_InitializeColor(float red, float green, float blue, float alpha, LC_Color *color);
LC_Color LC_CreateColor(float red, float green, float blue, float alpha);

int32 LC_GL_InitializeVideo(LC_Arena *arena, LC_GL_Renderer *renderer, const char *title, 
                            const char *fontName, char *errorLog);
void LC_GL_FramebufferSizeCallback(int32 width, int32 height);
void LC_GL_GetOpenGLVersionInfo();
bool LC_GL_IsDSAAvailable(LC_GL_Renderer *renderer);
void LC_GL_SetupViewProjectionMatrix2D(int32 screenWidth, int32 screenHeight, mat4 viewProjectionMatrix);
void LC_GL_SetupDefaultRectRenderer(LC_Arena *arena, LC_GL_Renderer *renderer, const char *vertexShaderPath, 
                              const char *fragmentShaderPath, char *errorLog);
void LC_GL_DefaultRectVAODSA(LC_GL_Renderer *renderer, const uint32 *indices,  GLuint sizeOfBuffer);
void LC_GL_DefaultRectVAONonDSA(LC_GL_Renderer *renderer, const uint32 *indices, GLuint sizeOfBuffer);
void LC_GL_ClearBackground(LC_Color color);
void LC_GL_RenderRectangle(LC_GL_Renderer *renderer, const LC_Rect *rect, const LC_Color *color);
void LC_GL_RenderRectDSA(LC_GL_Renderer *renderer, const float *buffer, uint32 sizeOfBuffer);
void LC_GL_RenderRectNonDSA(LC_GL_Renderer *renderer, const float *buffer, uint32 sizeOfBuffer);
bool LC_GL_SwapBuffer(SDL_Window *window, char *errorLog);
void LC_GL_FreeResources(const LC_GL_Renderer *renderer);

// ==================================================================================================================

#endif //LIBRAVIDEO_H
