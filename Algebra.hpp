#pragma once
#include "GeometryTypes.hpp"
#include <cmath>
/**
 * ベクトルおよび行列に対する純粋な数学的演算（加減乗除、内積、外積、逆行列、転置など）の実行。
 **/

/********************** ベクトル演算 (Vector Operations) ***************************/
/* 加減算・スカラー倍 (Addition, Subtraction, Scalar Multiplication): */
Vector3 Add(const Vector3& a, const Vector3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}
Vector3 Subtract(const Vector3& a, const Vector3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}
Vector3 Multiply(float scalar, const Vector3& v) {
    return { scalar * v.x, scalar * v.y, scalar * v.z };
}

/* 内積 (Dot Product): */
float Dot(const Vector3& a, const Vector3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/* 外積 (Cross Product): */
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x)
    );
}

/* 射影 (Vector Projection): */
Vector3 Project(const Vector3& v1, const Vector3& v2) {
    float lengthSq = Dot(v2, v2);
    if (lengthSq == 0.0f) {
        return { 0.0f, 0.0f, 0.0f };
    }
    float t = Dot(v1, v2) / lengthSq;
    return Multiply(t, v2);
}

/* ベクトルの長さの2乗を計算する（最適化のため平方根を避ける） */
float LengthSq(const Vector3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/* 実際に長さが必要な時だけ平方根を計算する */
float Length(const Vector3& v) {
    return std::sqrtf(LengthSq(v));
}

/********************** ベクトル演算 (Vector Operations) ***************************/
/* 単位行列 (Identity Matrix): */
Matrix4x4 MakeIdentity4x4() {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        result.m[i][i] = 1.0f;
    }
    return result;
}

/********************** 行列演算(Matrix Operations) ***************************/
/* 加減算・乗算 (Matrix Arithmetic): */
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m1.m[i][j] + m2.m[i][j];
        }
    }
    return result;
}
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m1.m[i][j] - m2.m[i][j];
        }
    }
    return result;
}
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return result;
}

/* 逆行列 (Inverse Matrix): */
Matrix4x4 Inverse(const Matrix4x4& mat) {
    Matrix4x4 res = {0};

    res.m[0][0] = mat.m[1][1] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[1][2] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) +
        mat.m[1][3] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]);
    res.m[0][1] = -(mat.m[0][1] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) +
        mat.m[0][3] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]));
    res.m[0][2] = mat.m[0][1] * (mat.m[1][2] * mat.m[3][3] - mat.m[1][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[1][1] * mat.m[3][3] - mat.m[1][3] * mat.m[3][1]) +
        mat.m[0][3] * (mat.m[1][1] * mat.m[3][2] - mat.m[1][2] * mat.m[3][1]);
    res.m[0][3] = -(mat.m[0][1] * (mat.m[1][2] * mat.m[2][3] - mat.m[1][3] * mat.m[2][2]) -
        mat.m[0][2] * (mat.m[1][1] * mat.m[2][3] - mat.m[1][3] * mat.m[2][1]) +
        mat.m[0][3] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]));

    res.m[1][0] = -(mat.m[1][0] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[1][2] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[1][3] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]));
    res.m[1][1] = mat.m[0][0] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]);
    res.m[1][2] = -(mat.m[0][0] * (mat.m[1][2] * mat.m[3][3] - mat.m[1][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[1][0] * mat.m[3][3] - mat.m[1][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[3][2] - mat.m[1][2] * mat.m[3][0]));
    res.m[1][3] = mat.m[0][0] * (mat.m[1][2] * mat.m[2][3] - mat.m[1][3] * mat.m[2][2]) -
        mat.m[0][2] * (mat.m[1][0] * mat.m[2][3] - mat.m[1][3] * mat.m[2][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]);

    res.m[2][0] = mat.m[1][0] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) -
        mat.m[1][1] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[1][3] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]);
    res.m[2][1] = -(mat.m[0][0] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]));
    res.m[2][2] = mat.m[0][0] * (mat.m[1][1] * mat.m[3][3] - mat.m[1][3] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[3][3] - mat.m[1][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[3][1] - mat.m[1][1] * mat.m[3][0]);
    res.m[2][3] = -(mat.m[0][0] * (mat.m[1][1] * mat.m[2][3] - mat.m[1][3] * mat.m[2][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[2][3] - mat.m[1][3] * mat.m[2][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]));

    res.m[3][0] = -(mat.m[1][0] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]) -
        mat.m[1][1] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]) +
        mat.m[1][2] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]));
    res.m[3][1] = mat.m[0][0] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]) +
        mat.m[0][2] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]);
    res.m[3][2] = -(mat.m[0][0] * (mat.m[1][1] * mat.m[3][2] - mat.m[1][2] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[3][2] - mat.m[1][2] * mat.m[3][0]) +
        mat.m[0][2] * (mat.m[1][0] * mat.m[3][1] - mat.m[1][1] * mat.m[3][0]));
    res.m[3][3] = mat.m[0][0] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) +
        mat.m[0][2] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]);

    float det = mat.m[0][0] * res.m[0][0] + mat.m[0][1] * res.m[1][0] + mat.m[0][2] * res.m[2][0] + mat.m[0][3] * res.m[3][0];

    float invDet = 1.0f / det;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[i][j] *= invDet;
        }
    }

    return res;
}

/* 転置行列 (Transpose Matrix): */
Matrix4x4 Transpose(const Matrix4x4& m) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m.m[j][i];
        }
    }
    return result;
}