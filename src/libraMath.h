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
void LC_Matrix3DInitialize(LC_Matrix3D mat3, const LC_Vector3D a, const LC_Vector3D b, const LC_Vector3D c);
void LC_Matrix4DInitialize(LC_Matrix4D mat4, const LC_Vector4D a, const LC_Vector4D b, const LC_Vector4D c,
    const LC_Vector4D d);

#endif //LIBRAMATH_H
