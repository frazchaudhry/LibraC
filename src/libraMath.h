//
// Created by Fraz Mahmud on 6/19/2025.
//

#ifndef LIBRAMATH_H
#define LIBRAMATH_H

typedef float LC_Vector2D[2];
typedef float LC_Vector3D[3];
typedef float LC_Vector4D[4];

typedef float LC_Matrix2D[2][2];
typedef float LC_Matrix3D[3][3];
typedef float LC_Matrix4D[4][4];

void LC_Vector2DAddVector2D(LC_Vector2D target, const LC_Vector2D vecToAdd);
void LC_Vector3DAddVector3D(LC_Vector3D target, const LC_Vector3D vecToAdd);
void LC_Vector4DAddVector4D(LC_Vector4D target, const LC_Vector4D vecToAdd);
void LC_Vector2DSubVector2D(LC_Vector2D target, const LC_Vector2D vecToSub);
void LC_Vector3DSubVector3D(LC_Vector3D target, const LC_Vector3D vecToSub);
void LC_Vector4DSubVector3D(LC_Vector4D target, const LC_Vector4D vecToSub);
void LC_Vector2DMulScaler(LC_Vector2D vec2, float s);
void LC_Vector3DMulScaler(LC_Vector3D vec3, float s);
void LC_Vector4DMulScaler(LC_Vector4D vec4, float s);
void LC_Vector2DDivScaler(LC_Vector2D vec2, float s);
void LC_Vector3DDivScaler(LC_Vector3D vec3, float s);
void LC_Vector4DDivScaler(LC_Vector4D vec4, float s);
float LC_Vector2DMagnitude(const LC_Vector2D vec2);
float LC_Vector3DMagnitude(const LC_Vector3D vec3);
float LC_Vector4DMagnitude(const LC_Vector4D vec4);
void LC_Vector2DNormalize(LC_Vector2D vec2);
void LC_Vector3DNormalize(LC_Vector3D vec3);
void LC_Vector4DNormalize(LC_Vector4D vec4);

void LC_Matrix2DInitialize(LC_Matrix2D mat2, const LC_Vector2D a, const LC_Vector2D b);
void LC_Matrix2DInitializeF(float n00, float n01,
                            float n10, float n11, LC_Matrix2D mat2);
void LC_Matrix3DInitialize(LC_Matrix3D mat3, const LC_Vector3D a, const LC_Vector3D b, const LC_Vector3D c);
void LC_Matrix3DInitializeF(float n00, float n01, float n02,
                            float n10, float n11, float n12,
                            float n20, float n21, float n22, LC_Matrix3D mat3);
void LC_Matrix4DInitialize(LC_Matrix4D mat4, const LC_Vector4D a, const LC_Vector4D b, const LC_Vector4D c,
    const LC_Vector4D d);
void LC_Matrix4DInitializeF(float n00, float n01, float n02, float n03,
                            float n10, float n11, float n12, float n13,
                            float n20, float n21, float n22, float n23,
                            float n30, float n31, float n32, float n33, LC_Matrix4D mat4);
void LC_Matrix2DMulVector2D(LC_Matrix2D mat2, const LC_Vector2D vec2, LC_Vector2D destination);
void LC_Matrix3DMulVector3D(LC_Matrix3D mat3, const LC_Vector3D vec3, LC_Vector3D destination);
void LC_Matrix4DMulVector4D(LC_Matrix4D mat4, const LC_Vector4D vec4, LC_Vector4D destination);
void LC_Matrix2DMulMatrix2D(LC_Matrix2D a, LC_Matrix2D b, LC_Matrix2D destination);
void LC_Matrix3DMulMatrix3D(LC_Matrix3D a, LC_Matrix3D b, LC_Matrix3D destination);
void LC_Matrix4DMulMatrix4D(LC_Matrix4D a, LC_Matrix4D b, LC_Matrix4D destination);

#endif //LIBRAMATH_H
