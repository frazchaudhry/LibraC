//
// Created by Fraz Mahmud on 6/19/2025.
//

#include <libraMath.h>
#include <math.h>

void LC_Vector2DAddVector2D(LC_Vector2D target, const LC_Vector2D vecToAdd) {
    target[0] += vecToAdd[0];
    target[1] += vecToAdd[1];
}

void LC_Vector3DAddVector3D(LC_Vector3D target, const LC_Vector3D vecToAdd) {
    target[0] += vecToAdd[0];
    target[1] += vecToAdd[1];
    target[2] += vecToAdd[2];
}

void LC_Vector4DAddVector4D(LC_Vector4D target, const LC_Vector4D vecToAdd) {
    target[0] += vecToAdd[0];
    target[1] += vecToAdd[1];
    target[2] += vecToAdd[2];
    target[3] += vecToAdd[3];
}

void LC_Vector2DSubVector2D(LC_Vector2D target, const LC_Vector2D vecToSub) {
    target[0] -= vecToSub[0];
    target[1] -= vecToSub[1];
}

void LC_Vector3DSubVector3D(LC_Vector3D target, const LC_Vector3D vecToSub) {
    target[0] -= vecToSub[0];
    target[1] -= vecToSub[1];
    target[2] -= vecToSub[2];
}

void LC_Vector4DSubVector3D(LC_Vector4D target, const LC_Vector4D vecToSub) {
    target[0] -= vecToSub[0];
    target[1] -= vecToSub[1];
    target[2] -= vecToSub[2];
    target[3] -= vecToSub[3];
}

void LC_Vector2DMulScaler(LC_Vector2D vec2, const float s) {
    vec2[0] *= s;
    vec2[1] *= s;
}

void LC_Vector3DMulScaler(LC_Vector3D vec3, const float s) {
    vec3[0] *= s;
    vec3[1] *= s;
    vec3[2] *= s;
}

void LC_Vector4DMulScaler(LC_Vector4D vec4, const float s) {
    vec4[0] *= s;
    vec4[1] *= s;
    vec4[2] *= s;
    vec4[3] *= s;
}

void LC_Vector2DDivScaler(LC_Vector2D vec2, const float s) {
    const float divisor = 1.0f / s;
    vec2[0] *= divisor;
    vec2[1] *= divisor;
}

void LC_Vector3DDivScaler(LC_Vector3D vec3, const float s) {
    const float divisor = 1.0f / s;
    vec3[0] *= divisor;
    vec3[1] *= divisor;
    vec3[2] *= divisor;
}

void LC_Vector4DDivScaler(LC_Vector4D vec4, const float s) {
    const float divisor = 1.0f / s;
    vec4[0] *= divisor;
    vec4[1] *= divisor;
    vec4[2] *= divisor;
    vec4[3] *= divisor;
}

float LC_Vector2DMagnitude(const LC_Vector2D vec2) {
    return sqrtf(vec2[0] * vec2[0] + vec2[1] * vec2[1]);
}

float LC_Vector3DMagnitude(const LC_Vector3D vec3) {
    return sqrtf(vec3[0] * vec3[0] + vec3[1] * vec3[1] + vec3[2] * vec3[2]);
}

float LC_Vector4DMagnitude(const LC_Vector4D vec4) {
    return sqrtf(vec4[0] * vec4[0] + vec4[1] * vec4[1] + vec4[2] * vec4[2] + vec4[3] * vec4[3]);
}

void LC_Vector2DNormalize(LC_Vector2D vec2) {
    const float magnitude = LC_Vector2DMagnitude(vec2);
    const float reciprocalOfSqrt = 1 / magnitude;
    LC_Vector2DMulScaler(vec2, reciprocalOfSqrt);
}

void LC_Vector3DNormalize(LC_Vector3D vec3) {
    const float magnitude = LC_Vector3DMagnitude(vec3);
    const float reciprocalOfSqrt = 1 / magnitude;
    LC_Vector3DMulScaler(vec3, reciprocalOfSqrt);
}

void LC_Vector4DNormalize(LC_Vector4D vec4) {
    const float magnitude = LC_Vector4DMagnitude(vec4);
    const float reciprocalOfSqrt = 1 / magnitude;
    LC_Vector4DMulScaler(vec4, reciprocalOfSqrt);
}

void LC_Matrix2DInitialize(LC_Matrix2D mat2, const LC_Vector2D a, const LC_Vector2D b) {
    mat2[0][0] = a[0]; mat2[0][1] = a[1];
    mat2[1][0] = b[0]; mat2[1][1] = b[1];
}

void LC_Matrix3DInitialize(LC_Matrix3D mat3, const LC_Vector3D a, const LC_Vector3D b, const LC_Vector3D c) {
    mat3[0][0] = a[0]; mat3[0][1] = a[1]; mat3[0][2] = a[2];
    mat3[1][0] = b[0]; mat3[1][1] = b[1]; mat3[1][2] = b[2];
    mat3[2][0] = c[0]; mat3[2][1] = c[1]; mat3[2][2] = c[2];
}

void LC_Matrix4DInitialize(LC_Matrix4D mat4, const LC_Vector4D a, const LC_Vector4D b, const LC_Vector4D c,
    const LC_Vector4D d) {
    mat4[0][0] = a[0]; mat4[0][1] = a[1]; mat4[0][2] = a[2]; mat4[0][3] = a[3];
    mat4[1][0] = b[0]; mat4[1][1] = b[1]; mat4[1][2] = b[2]; mat4[1][3] = b[3];
    mat4[2][0] = c[0]; mat4[2][1] = c[1]; mat4[2][2] = c[2]; mat4[2][3] = c[3];
    mat4[3][0] = d[0]; mat4[3][1] = d[1]; mat4[3][2] = d[2]; mat4[3][3] = d[3];
}
