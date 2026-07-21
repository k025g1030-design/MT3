#pragma once
#include "GeometryTypes.hpp"
#include "Algebra.hpp"
#include "MathUtility.hpp"

/**
 * 抽象的な行列計算に「3D空間とレンダリングパイプラインにおける物理的・幾何学的な意味」を持たせる処理。
 **/

/********************** ベクトル演算 (Vector Operations) ***************************/
/* 拡縮 (Scale / Scaling): */
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

/* 回転 (Rotation): */
Matrix4x4 MakeRotateXMatrix(float radian) {
    Matrix4x4 result = MakeIdentity4x4();
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    result.m[1][1] = cosTheta;
    result.m[1][2] = sinTheta;
    result.m[2][1] = -sinTheta;
    result.m[2][2] = cosTheta;
    return result;
}
Matrix4x4 MakeRotateYMatrix(float radian) {
    Matrix4x4 result = MakeIdentity4x4();
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    result.m[0][0] = cosTheta;
    result.m[0][2] = -sinTheta;
    result.m[2][0] = sinTheta;
    result.m[2][2] = cosTheta;
    return result;
}
Matrix4x4 MakeRotateZMatrix(float radian) {
    Matrix4x4 result = MakeIdentity4x4();
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    result.m[0][0] = cosTheta;
    result.m[0][1] = sinTheta;
    result.m[1][0] = -sinTheta;
    result.m[1][1] = cosTheta;
    return result;
}

/* 平行移動 (Translation): */
Matrix4x4 MakeTranslateMatrix(const Vector3& translation) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[3][0] = translation.x;
    result.m[3][1] = translation.y;
    result.m[3][2] = translation.z;
    return result;
}


/* 複合アフィン行列 (Composite TRS Matrix): */
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
    Matrix4x4 result = MakeIdentity4x4();
    Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
    Matrix4x4 rotateX = MakeRotateXMatrix(rotation.x);
    Matrix4x4 rotateY = MakeRotateYMatrix(rotation.y);
    Matrix4x4 rotateZ = MakeRotateZMatrix(rotation.z);
    Matrix4x4 translateMatrix = MakeTranslateMatrix(translation);

    Matrix4x4 rotateXYZ = (rotateX * (rotateY * rotateZ));
    //result = Multiply(rotateXYZ, translateMatrix);
    //result = Multiply(scaleMatrix, result);
    result = ((scaleMatrix * rotateXYZ) * translateMatrix);

    return result;
}

/********************** 投影とビューポート(Projection& Viewport) ***************************/
/* 直交投影 (Orthographic Projection): */
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = -2.0f / (farClip - nearClip);
    result.m[3][0] = -(right + left) / (right - left);
    result.m[3][1] = -(top + bottom) / (top - bottom);
    result.m[3][2] = -(farClip + nearClip) / (farClip - nearClip);
    return result;
}

/* 透視投影 (Perspective Projection): */
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearClip, float farClip) {
    Matrix4x4 result{};
    float f = 1.0f / std::tan(fovY / 2.0f);
    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    //result.m[2][2] = (farClip + nearClip) / (nearClip - farClip);
    //result.m[2][3] = -1.0f;
    //result.m[3][2] = (2.0f * farClip * nearClip) / (nearClip - farClip);
    result.m[2][2] = farClip / (nearClip - farClip); 
    result.m[2][3] = 1.0f;
    result.m[3][2] = -(farClip * nearClip) / (farClip - nearClip);
    return result;
}

/* ビューポート変換 (Viewport Transformation): */
Matrix4x4 MakeViewportMatrix(float x, float y, float width, float height, float minDepth, float maxDepth) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = width / 2.0f;
    result.m[1][1] = -height / 2.0f; // Y軸を反転
    result.m[2][2] = maxDepth - minDepth;
    result.m[3][0] = x + width / 2.0f;
    result.m[3][1] = y + height / 2.0f;
    result.m[3][2] = minDepth;
    return result;
}

/********************** 座標変換(Coordinate Transformation) ***************************/
/* 頂点変換 (Vertex Transformation): */
Vector3 Transform(const Vector3& point, const Matrix4x4& matrix) {
    Vector3 result = {};
    result.x = point.x * matrix.m[0][0] + point.y * matrix.m[1][0] + point.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = point.x * matrix.m[0][1] + point.y * matrix.m[1][1] + point.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = point.x * matrix.m[0][2] + point.y * matrix.m[1][2] + point.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = point.x * matrix.m[0][3] + point.y * matrix.m[1][3] + point.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
    // w が 0 の場合は透視除法ができないため、結果をそのまま返す
    if (w != 0.0f) {
        result = { result.x / w, result.y / w, result.z / w };
    }

    return result;
}
