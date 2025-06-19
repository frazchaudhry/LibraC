//
// Created by Fraz Mahmud on 5/14/2025.
//

#include <libraVideo.h>

#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

void LC_GL_UseProgram(const uint32 programId) {
    glUseProgram(programId);
}

void LC_GL_SetUniformBool(const LC_GL_Shader *shader, const char *name, const bool value) {
    glUniform1i(glGetUniformLocation(shader->programId, name), (int32)value);
}

void LC_GL_SetUniformInt(const LC_GL_Shader *shader, const char *name, const int32 value) {
    glUniform1i(glGetUniformLocation(shader->programId, name), value);
}

void LC_GL_SetUniformFloat(const LC_GL_Shader *shader, const char *name, const float value) {
    glUniform1f(glGetUniformLocation(shader->programId, name), value);
}

void LC_GL_SetUniformVec2(const LC_GL_Shader *shader, const char *name, const vec2 value) {
    glUniform2fv(glGetUniformLocation(shader->programId, name), 1, &value[0]);
}

void LC_GL_SetUniformVec2f(const LC_GL_Shader *shader, const char *name, const float x, const float y) {
    glUniform2f(glGetUniformLocation(shader->programId, name), x, y);
}

void LC_GL_SetUniformVec3(const LC_GL_Shader *shader, const char *name, const vec3 value) {
    glUniform3fv(glGetUniformLocation(shader->programId, name), 1, &value[0]);
}

void LC_GL_SetUniformVec3f(const LC_GL_Shader *shader, const char *name, const float x, const float y, const float z) {
    glUniform3f(glGetUniformLocation(shader->programId, name), x, y, z);
}

void LC_GL_SetUniformVec4(const LC_GL_Shader *shader, const char *name, const vec4 value) {
    glUniform4fv(glGetUniformLocation(shader->programId, name), 1, &value[0]);
}

void LC_GL_SetUniformVec4f(const LC_GL_Shader *shader, const char *name, const float x, const float y, const float z,
                           const float w) {
    glUniform4f(glGetUniformLocation(shader->programId, name), x, y, z, w);
}

void LC_GL_SetUniformMat2(const LC_GL_Shader *shader, const char *name, const mat2 *mat) {
    glUniformMatrix2fv(glGetUniformLocation(shader->programId, name), 1, GL_FALSE, mat[0][0]);
}

void LC_GL_SetUniformMat3(const LC_GL_Shader *shader, const char *name, const mat3 *mat) {
    glUniformMatrix3fv(glGetUniformLocation(shader->programId, name), 1, GL_FALSE, mat[0][0]);
}

void LC_GL_SetUniformMat4(const LC_GL_Shader *shader, const char *name, const mat4 *mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader->programId, name), 1, GL_FALSE, mat[0][0]);
}

bool LC_GL_InitializeShader(Arena *arena, LC_GL_Shader *shader, const char *vertexShaderPath,
                            const char *fragmentShaderPath, char *buffer) {
    char *vertexShaderSource = NULL;
    char *fragmentShaderSource = NULL;
    LC_GetFileContent(arena, vertexShaderPath, &vertexShaderSource);
    LC_GetFileContent(arena, fragmentShaderPath, &fragmentShaderSource);

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
    glShaderSource(vertexShader, 1, (char const* const *)&vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    if (!CheckCompileErrors(vertexShader, "VERTEX", buffer)) return false;

    // Fragment Shader
    const uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (char const* const *)&fragmentShaderSource, NULL);
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

bool CheckCompileErrors(const uint32 shader, char *type, char *buffer) {
    int success;
    char infoLog[1024];
    if (strcmp(type, "PROGRAM") != 0) {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            sprintf(buffer, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            sprintf(buffer, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\n", type, infoLog);
            return false;
        }
    }
    return true;
}
