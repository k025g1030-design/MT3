#pragma once
#include <vector>
/**
 * 純粋な「データコンテナ」の定義。状態を保持するだけで、計算ロジック（振る舞い）を一切持たない最下層のデータ構造です。
 **/

struct Vector3 {
    float x, y, z;
};

Vector3 operator+(const Vector3& a, const Vector3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 operator-(const Vector3& a, const Vector3& b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 operator*(float scalar, const Vector3& v) {
    return { scalar * v.x, scalar * v.y, scalar * v.z };
}

Vector3 operator*(const Vector3& v, float scalar) {
    return scalar * v;
}

Vector3 operator/(const Vector3& v, float scalar) {
    return (1.0f / scalar) * v;
}

struct Matrix4x4 {
    float m[4][4];
}; 
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m1.m[i][j] + m2.m[i][j];
        }
    }
    return result;
}

Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m1.m[i][j] - m2.m[i][j];
        }
    }
    return result;
}
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
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


struct CameraObj {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
};

struct Sphere {
    Vector3 center;
    float radius;
};

struct LocalTransform {
    Vector3 translate;
    Vector3 rotate;
    Vector3 scale;
};

struct Node {
    LocalTransform* localTransform;
    Matrix4x4 worldMatrix;

    int parentIndex = -1;
    std::vector<int> children;
    uint32_t color = 0xFFFFFFFF; // デフォルトの色を白に設定    
};

// 注意事項：数学上、Plane（平面）は無限に広がるものとして扱われます。
//          そのため、ここでは描画範囲や実体の大きさではなく、単純に数学的な判定だけを行っています。
struct Plane {
    Vector3 normal; // 法線
    float distance; // 距離
};



// 線の種類を定義する列挙型
enum class LineType {
    Line,    // 直線（両方向に無限に伸びる線）
    Ray,     // 光線・レイ（始点があり、一方向に無限に伸びる線）
    Segment  // 線分（始点と終点がある有限の長さの線）
};

struct Line {
    Vector3 origin;    // 始点
    Vector3 diff;      // 方向ベクトル（または始点から終点へのベクトル）
    LineType type;     // 線の種類
};

// 注意事項：ゲームエンジンにとって、すべての3D平面は三角形で構成されています。
struct Triangle {
    Vector3 vertices[3]; // 頂点
};

struct PolygonV2 {
    std::vector<Vector3> vertices;  // N個の頂点を格納（時計回り、または反時計回りに整列していること）
};

struct AABB {
    Vector3 min; // 最小点
    Vector3 max; // 最大点
};

struct OBB {
    Vector3 center; // 中心点
    Vector3 orientations[3];   // 座標軸
    Vector3 size;              // 座標軸方向の長さの半分。中心から面までの距離
};

struct Spring {
    Vector3 anchor; // 固定点
    float naturalLength; // 自然長
    float stiffness; // ばね定数
    float dampingCoefficient; // 減衰係数
};

struct Ball {
    Vector3 position; // 位置
    Vector3 velocity; // 速度
    Vector3 acceleration; // 加速度
    float mass; // 質量
    float radius; // 半径
    uint32_t color; // 色
};

struct Pendulum {
    Vector3 anchor; // 振り子の固定点
    float length;   // 振り子の長さ
    float angle;    // 振り子の角度（ラジアン）
    float angularVelocity;     // 角速度
    float angularAcceleration; // 角加速度
};
