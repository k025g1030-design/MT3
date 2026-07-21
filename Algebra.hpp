#pragma once
#include "GeometryTypes.hpp"
#include <cmath>
/**
 * ベクトルおよび行列に対する純粋な数学的演算（加減乗除、内積、外積、逆行列、転置など）の実行。
 **/

/********************** ベクトル演算 (Vector Operations) ***************************/

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
    return (t * v2);
}

/* ベクトルの長さの2乗を計算する（最適化のため平方根を避ける） */
float LengthSq(const Vector3& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

/* 実際に長さが必要な時だけ平方根を計算する */
float Length(const Vector3& v) {
    return std::sqrtf(LengthSq(v));
}

/* 指定されたベクトルに直交するベクトルを生成する */
Vector3 Perpendicular(const Vector3& vector) {
    // XY平面上に成分を持つ場合は、その成分を入れ替えて直交ベクトルを算出
    if (vector.x != 0.0f || vector.y != 0.0f) {
        return { -vector.y, vector.x, 0.0f };
    }
    // Z軸と平行な場合はYZ平面の成分を使用して直交ベクトルを算出
    return { 0.0f, -vector.z, vector.y };
}

/* ベクトルの正規化 */
Vector3 Normalize(const Vector3& v) {
    float length = Length(v);

    // 長さが0（または極めて0に近い）場合の安全対策（NaN汚染の防止）
    if (length == 0.0f) {
        return { 0.0f, 0.0f, 0.0f };
    }

    // 割り算を1回で済ませ、以降は掛け算を使用する（パフォーマンス最適化）
    float invLength = 1.0f / length;
    return { v.x * invLength, v.y * invLength, v.z * invLength };
}

/* 線形補間 (Linear Interpolation): */
Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
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

/* 逆行列 (Inverse Matrix): */
Matrix4x4 Inverse(const Matrix4x4& mat) {
    Matrix4x4 res = { 0 };

    // 1) 2x2 部分行列式の事前計算 (ラプラス展開のブロック化)
    // 第1行と第2行 (Rows 0, 1) の組み合わせ
    float f0 = mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0];
    float f1 = mat.m[0][0] * mat.m[1][2] - mat.m[0][2] * mat.m[1][0];
    float f2 = mat.m[0][0] * mat.m[1][3] - mat.m[0][3] * mat.m[1][0];
    float f3 = mat.m[0][1] * mat.m[1][2] - mat.m[0][2] * mat.m[1][1];
    float f4 = mat.m[0][1] * mat.m[1][3] - mat.m[0][3] * mat.m[1][1];
    float f5 = mat.m[0][2] * mat.m[1][3] - mat.m[0][3] * mat.m[1][2];

    // 第3行と第4行 (Rows 2, 3) の組み合わせ
    float b0 = mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0];
    float b1 = mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0];
    float b2 = mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0];
    float b3 = mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1];
    float b4 = mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1];
    float b5 = mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2];

    // 2) 4x4 行列式 (Determinant) の計算
    float det = f0 * b5 - f1 * b4 + f2 * b3 + f3 * b2 - f4 * b1 + f5 * b0;

    // 3) 安全性チェック: 行列式が0に近い場合、逆行列は存在しない (NaN 汚染の防止)
    if (std::abs(det) < 1e-6f) {
        return MakeIdentity4x4(); // エンジンのアーキテクチャに応じて、ゼロ行列を返すか例外をスローする
    }

    float invDet = 1.0f / det;

    // 4) 余因子行列 (Adjugate Matrix) の計算と invDet の乗算
    // (大量の重複演算を、キャッシュされた f0~f5 および b0~b5 の抽出に置き換える)
    res.m[0][0] = (mat.m[1][1] * b5 - mat.m[1][2] * b4 + mat.m[1][3] * b3) * invDet;
    res.m[0][1] = (-mat.m[0][1] * b5 + mat.m[0][2] * b4 - mat.m[0][3] * b3) * invDet;
    res.m[0][2] = (mat.m[3][1] * f5 - mat.m[3][2] * f4 + mat.m[3][3] * f3) * invDet;
    res.m[0][3] = (-mat.m[2][1] * f5 + mat.m[2][2] * f4 - mat.m[2][3] * f3) * invDet;

    res.m[1][0] = (-mat.m[1][0] * b5 + mat.m[1][2] * b2 - mat.m[1][3] * b1) * invDet;
    res.m[1][1] = (mat.m[0][0] * b5 - mat.m[0][2] * b2 + mat.m[0][3] * b1) * invDet;
    res.m[1][2] = (-mat.m[3][0] * f5 + mat.m[3][2] * f2 - mat.m[3][3] * f1) * invDet;
    res.m[1][3] = (mat.m[2][0] * f5 - mat.m[2][2] * f2 + mat.m[2][3] * f1) * invDet;

    res.m[2][0] = (mat.m[1][0] * b4 - mat.m[1][1] * b2 + mat.m[1][3] * b0) * invDet;
    res.m[2][1] = (-mat.m[0][0] * b4 + mat.m[0][1] * b2 - mat.m[0][3] * b0) * invDet;
    res.m[2][2] = (mat.m[3][0] * f4 - mat.m[3][1] * f2 + mat.m[3][3] * f0) * invDet;
    res.m[2][3] = (-mat.m[2][0] * f4 + mat.m[2][1] * f2 - mat.m[2][3] * f0) * invDet;

    res.m[3][0] = (-mat.m[1][0] * b3 + mat.m[1][1] * b1 - mat.m[1][2] * b0) * invDet;
    res.m[3][1] = (mat.m[0][0] * b3 - mat.m[0][1] * b1 + mat.m[0][2] * b0) * invDet;
    res.m[3][2] = (-mat.m[3][0] * f3 + mat.m[3][1] * f1 - mat.m[3][2] * f0) * invDet;
    res.m[3][3] = (mat.m[2][0] * f3 - mat.m[2][1] * f1 + mat.m[2][2] * f0) * invDet;

    return res;
}

/**
 * 剛体変換（回転と平行移動のみ）の逆行列を高速に計算する。
 * 汎用的な Inverse() は 4x4 の行列式計算を行うため、衝突判定ループ内で使用するには重すぎる。
 * 直交行列の性質 (R^-1 = R^T) を利用した最適化パス (Fast Path)。
 */
Matrix4x4 MakeRigidInverse(const Vector3& center, const Vector3& axisX, const Vector3& axisY, const Vector3& axisZ) {
    Matrix4x4 res; // 不依賴 MakeIdentity4x4()，避免額外的迴圈開銷

    // メモリアクセス最適化（Cache-Friendly）のため、行(Row)ごとに連続してメモリに書き込む
    // 回転行列の転置: 基底ベクトル(axis)は列として配置されるため、各要素を横方向に代入する
    res.m[0][0] = axisX.x; res.m[0][1] = axisY.x; res.m[0][2] = axisZ.x; res.m[0][3] = 0.0f;
    res.m[1][0] = axisX.y; res.m[1][1] = axisY.y; res.m[1][2] = axisZ.y; res.m[1][3] = 0.0f;
    res.m[2][0] = axisX.z; res.m[2][1] = axisY.z; res.m[2][2] = axisZ.z; res.m[2][3] = 0.0f;

    // 第4行: 平行移動の逆変換 (-Dot) と同次座標の w=1
    // Row-Major の規約に従い、移動成分を底部に配置する
    res.m[3][0] = -Dot(center, axisX);
    res.m[3][1] = -Dot(center, axisY);
    res.m[3][2] = -Dot(center, axisZ);
    res.m[3][3] = 1.0f;

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



