#pragma once
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

struct Line {
    Vector3 origin;  // 始点
    Vector3 diff;    // 終点
};

struct Ray {
    Vector3 origin;
    Vector3 diff;
};

struct Segment {
    Vector3 origin;
    Vector3 diff;
};

struct Plane {
    Vector3 normal; // 法線
    float distance; // 距離
};