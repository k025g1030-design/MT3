#pragma once
#include <vector>
/**
 * 純粋な「データコンテナ」の定義。状態を保持するだけで、計算ロジック（振る舞い）を一切持たない最下層のデータ構造です。
 **/

struct Vector3 {
    float x, y, z;
};

struct Matrix4x4 {
    float m[4][4];
};

struct CameraObj {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
};

struct Sphere {
    Vector3 center;
    float radius;
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

struct Triangle {
    Vector3 vertices[3]; //頂点
};

struct PolygonV2 {
    std::vector<Vector3> vertices;  //N個の頂点を格納（時計回り、または反時計回りに整列していること）
};
