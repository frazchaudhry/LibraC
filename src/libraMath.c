//
// Created by Fraz Mahmud on 6/19/2025.
//

#include <libraMath.h>
#include <math.h>
#include <stdio.h>

void LC_MatrixPrintf(void *mat, const uint8 m, const uint8 n) {
    float *bytes = mat;
    for (size_t i = 0; i < m; i++) {
        for (size_t j = 0; j < n; ++j) {
            void *elementPointer = bytes + (j * n + i);
            printf("%f\t", *(float *)elementPointer);
        }
        printf("\n");
    }
}

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


float LC_Vector2DDotVector2D(const LC_Vector2D a, const LC_Vector2D b) {
    return a[0] * b[0] + a[1] * b[1];
}

float LC_Vector3DDotVector3D(const LC_Vector3D a, const LC_Vector3D b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

float LC_Vector4DDotVector4D(const LC_Vector4D a, const LC_Vector4D b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

void LC_Matrix2DInitialize(LC_Matrix2D mat2, const LC_Vector2D a, const LC_Vector2D b) {
    mat2[0][0] = a[0];
    mat2[0][1] = a[1];
    mat2[1][0] = b[0];
    mat2[1][1] = b[1];
}

void LC_Matrix2DInitializeF(const float n00, const float n01,
                            const float n10, const float n11, LC_Matrix2D mat2) {
    mat2[0][0] = n00;
    mat2[0][1] = n10;
    mat2[1][0] = n01;
    mat2[1][1] = n11;
}

void LC_Matrix3DInitialize(LC_Matrix3D mat3, const LC_Vector3D a, const LC_Vector3D b, const LC_Vector3D c) {
    mat3[0][0] = a[0];
    mat3[0][1] = a[1];
    mat3[0][2] = a[2];
    mat3[1][0] = b[0];
    mat3[1][1] = b[1];
    mat3[1][2] = b[2];
    mat3[2][0] = c[0];
    mat3[2][1] = c[1];
    mat3[2][2] = c[2];
}

void LC_Matrix3DInitializeF(const float n00, const float n01, const float n02,
                            const float n10, const float n11, const float n12,
                            const float n20, const float n21, const float n22, LC_Matrix3D mat3) {
    mat3[0][0] = n00;
    mat3[0][1] = n10;
    mat3[0][2] = n20;
    mat3[1][0] = n01;
    mat3[1][1] = n11;
    mat3[1][2] = n21;
    mat3[2][0] = n02;
    mat3[2][1] = n12;
    mat3[2][2] = n22;
}

void LC_Matrix4DInitialize(LC_Matrix4D mat4, const LC_Vector4D a, const LC_Vector4D b, const LC_Vector4D c,
                           const LC_Vector4D d) {
    mat4[0][0] = a[0];
    mat4[0][1] = a[1];
    mat4[0][2] = a[2];
    mat4[0][3] = a[3];
    mat4[1][0] = b[0];
    mat4[1][1] = b[1];
    mat4[1][2] = b[2];
    mat4[1][3] = b[3];
    mat4[2][0] = c[0];
    mat4[2][1] = c[1];
    mat4[2][2] = c[2];
    mat4[2][3] = c[3];
    mat4[3][0] = d[0];
    mat4[3][1] = d[1];
    mat4[3][2] = d[2];
    mat4[3][3] = d[3];
}

void LC_Matrix4DInitializeF(const float n00, const float n01, const float n02, const float n03,
                            const float n10, const float n11, const float n12, const float n13,
                            const float n20, const float n21, const float n22, const float n23,
                            const float n30, const float n31, const float n32, const float n33, LC_Matrix4D mat4) {
    mat4[0][0] = n00;
    mat4[0][1] = n10;
    mat4[0][2] = n20;
    mat4[0][3] = n30;
    mat4[1][0] = n01;
    mat4[1][1] = n11;
    mat4[1][2] = n21;
    mat4[1][3] = n31;
    mat4[2][0] = n02;
    mat4[2][1] = n12;
    mat4[2][2] = n22;
    mat4[2][3] = n32;
    mat4[3][0] = n03;
    mat4[3][1] = n13;
    mat4[3][2] = n23;
    mat4[3][3] = n33;
}

void LC_Matrix2DMulVector2D(LC_Matrix2D mat2, const LC_Vector2D vec2, LC_Vector2D destination) {
    destination[0] = mat2[0][0] * vec2[0] + mat2[1][0] * vec2[1];
    destination[1] = mat2[0][1] * vec2[0] + mat2[1][1] * vec2[1];
}

void LC_Matrix3DMulVector3D(LC_Matrix3D mat3, const LC_Vector3D vec3, LC_Vector3D destination) {
    destination[0] = mat3[0][0] * vec3[0] + mat3[1][0] * vec3[1] + mat3[2][0] * vec3[2];
    destination[1] = mat3[0][1] * vec3[0] + mat3[1][1] * vec3[1] + mat3[2][1] * vec3[2];
    destination[2] = mat3[0][2] * vec3[0] + mat3[1][2] * vec3[1] + mat3[2][2] * vec3[2];
}

void LC_Matrix4DMulVector4D(LC_Matrix4D mat4, const LC_Vector4D vec4, LC_Vector4D destination) {
    destination[0] = mat4[0][0] * vec4[0] + mat4[1][0] * vec4[1] + mat4[2][0] * vec4[2] + mat4[3][0] * vec4[3];
    destination[1] = mat4[0][1] * vec4[0] + mat4[1][1] * vec4[1] + mat4[2][1] * vec4[2] + mat4[3][1] * vec4[3];
    destination[2] = mat4[0][2] * vec4[0] + mat4[1][2] * vec4[1] + mat4[2][2] * vec4[2] + mat4[3][2] * vec4[3];
    destination[3] = mat4[0][3] * vec4[0] + mat4[1][3] * vec4[1] + mat4[2][3] * vec4[2] + mat4[3][3] * vec4[3];
}

void LC_Matrix2DMulMatrix2D(LC_Matrix2D a, LC_Matrix2D b, LC_Matrix2D destination) {
    LC_Matrix2DInitializeF(a[0][0] * b[0][0] + a[1][0] * b[0][1],
                           a[0][0] * b[1][0] + a[1][0] * b[1][1],
                           a[0][1] * b[0][0] + a[1][1] * b[0][1],
                           a[0][1] * b[1][0] + a[1][1] * b[1][1], destination);
}

void LC_Matrix3DMulMatrix3D(LC_Matrix3D a, LC_Matrix3D b, LC_Matrix3D destination) {
    LC_Matrix3DInitializeF(a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2],
                           a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2],
                           a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2],
                           a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2],
                           a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2],
                           a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2],
                           a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2],
                           a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2],
                           a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2], destination);
}

void LC_Matrix4DMulMatrix4D(LC_Matrix4D a, LC_Matrix4D b, LC_Matrix4D destination) {
    LC_Matrix4DInitializeF(a[0][0] * b[0][0] + a[1][0] * b[0][1] + a[2][0] * b[0][2] + a[3][0] * b[0][3],
                           a[0][0] * b[1][0] + a[1][0] * b[1][1] + a[2][0] * b[1][2] + a[3][0] * b[1][3],
                           a[0][0] * b[2][0] + a[1][0] * b[2][1] + a[2][0] * b[2][2] + a[3][0] * b[2][3],
                           a[0][0] * b[3][0] + a[1][0] * b[3][1] + a[2][0] * b[3][2] + a[3][0] * b[3][3],
                           a[0][1] * b[0][0] + a[1][1] * b[0][1] + a[2][1] * b[0][2] + a[3][1] * b[0][3],
                           a[0][1] * b[1][0] + a[1][1] * b[1][1] + a[2][1] * b[1][2] + a[3][1] * b[1][3],
                           a[0][1] * b[2][0] + a[1][1] * b[2][1] + a[2][1] * b[2][2] + a[3][1] * b[2][3],
                           a[0][1] * b[3][0] + a[1][1] * b[3][1] + a[2][1] * b[3][2] + a[3][1] * b[3][3],
                           a[0][2] * b[0][0] + a[1][2] * b[0][1] + a[2][2] * b[0][2] + a[3][2] * b[0][3],
                           a[0][2] * b[1][0] + a[1][2] * b[1][1] + a[2][2] * b[1][2] + a[3][2] * b[1][3],
                           a[0][2] * b[2][0] + a[1][2] * b[2][1] + a[2][2] * b[2][2] + a[3][2] * b[2][3],
                           a[0][2] * b[3][0] + a[1][2] * b[3][1] + a[2][2] * b[3][2] + a[3][2] * b[3][3],
                           a[0][3] * b[0][0] + a[1][3] * b[0][1] + a[2][3] * b[0][2] + a[3][3] * b[0][3],
                           a[0][3] * b[1][0] + a[1][3] * b[1][1] + a[2][3] * b[1][2] + a[3][3] * b[1][3],
                           a[0][3] * b[2][0] + a[1][3] * b[2][1] + a[2][3] * b[2][2] + a[3][3] * b[2][3],
                           a[0][3] * b[3][0] + a[1][3] * b[3][1] + a[2][3] * b[3][2] + a[3][3] * b[3][3],
                           destination);
}

void LC_Vector3DCrossVector3D(const LC_Vector3D a, const LC_Vector3D b, LC_Vector3D destination) {
    destination[0] = a[1] * b[2] - a[2] * b[1];
    destination[1] = a[2] * b[0] - a[0] * b[2];
    destination[2] = a[0] * b[1] - a[1] * b[0];
}
