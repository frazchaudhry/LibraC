//
// Created by Fraz Mahmud on 5/14/2025.
//

#ifndef LIBRAVIDEO_H
#define LIBRAVIDEO_H

#include <libraC.h>

#include "libraCore.h"

// =============================================SHADER===============================================================

typedef struct shader {
    uint32 programId;
} LC_GL_Shader;

bool LC_GL_InitializeShader(Arena *arena, LC_GL_Shader *shader, const char *vertexShaderPath,
                            const char *fragmentShaderPath,
                            char *buffer);

void LC_GL_UseProgram(uint32 programId);

void LC_GL_SetUniformBool(const LC_GL_Shader *shader, const char *name, bool value);
void LC_GL_SetUniformInt(const LC_GL_Shader *shader, const char *name, int32 value);
void LC_GL_SetUniformFloat(const LC_GL_Shader *shader, const char *name, float value);
void LC_GL_SetUniformVec2(const LC_GL_Shader *shader, const char *name, const vec2 value);
void LC_GL_SetUniformVec2f(const LC_GL_Shader *shader, const char *name, float x, float y);
void LC_GL_SetUniformVec3(const LC_GL_Shader *shader, const char *name, const vec3 value);
void LC_GL_SetUniformVec3f(const LC_GL_Shader *shader, const char *name, float x, float y, float z);
void LC_GL_SetUniformVec4(const LC_GL_Shader *shader, const char *name, const vec4 value);
void LC_GL_SetUniformVec4f(const LC_GL_Shader *shader, const char *name, float x, float y, float z,
                           float w);
void LC_GL_SetUniformMat2(const LC_GL_Shader *shader, const char *name, const mat2 *mat);
void LC_GL_SetUniformMat3(const LC_GL_Shader *shader, const char *name, const mat3 *mat);
void LC_GL_SetUniformMat4(const LC_GL_Shader *shader, const char *name, const mat4 *mat);

bool CheckCompileErrors(uint32 shader, char *type, char *buffer);

// ==================================================================================================================

#endif //LIBRAVIDEO_H
