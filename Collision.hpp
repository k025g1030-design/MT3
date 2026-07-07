#pragma once
#include "GeometryTypes.hpp"
#include "Algebra.hpp"
#include "MathUtility.hpp"
/**
 * 幾何学的なプリミティブ（線分、球など）間の空間的関係、最短距離の計算、および交差（衝突）判定を行う。
 */

Vector3 ClosestPoint(const Vector3& point, const Line& segment) {
    Vector3 v = Subtract(point, segment.origin);

    float lengthSq = Dot(segment.diff, segment.diff);
    float t = 0.0f;

    if (lengthSq != 0.0f) {
        t = Dot(v, segment.diff) / lengthSq;
    }

    t = Clamp(t, 0.0f, 1.0f);

    return Add(segment.origin, Multiply(t, segment.diff));
}

bool IsCollision(const Sphere& s1, const Sphere& s2) {
    float distanceSq = LengthSq(Subtract(s1.center, s2.center));

    float radiusSum = s1.radius + s2.radius;
    float radiusSumSq = radiusSum * radiusSum;

    if (distanceSq <= radiusSumSq) {
        return true;
    }
    return false;
}

bool IsCollision(const Sphere& s, const Plane& p) {
    // 球の中心から平面までの符号付き距離を計算： (N・C) - d
    float distToPlane = Dot(p.normal, s.center) - p.distance;
    // 距離の絶対値が半径以下であれば衝突している
    return std::abs(distToPlane) <= s.radius;
}

// 線と平面の衝突判定関数
// 
bool IsCollision(const Line& line, const Plane& plane) {

    // 1. まず垂直判定を行うために、法線と線の内積を求める
    float dot = Dot(plane.normal, line.diff);

    // 線が平面と平行であるため、通常は交差しない
    if (std::abs(dot) < 1e-6f) {
        return false;
    }

    // 2. tを求める
    float t = (plane.distance - Dot(line.origin, plane.normal)) / dot;

    // 3. tの値と線の種類によって衝突しているかを判断する
    switch (line.type) {
    case LineType::Line:
        // [直線]無限に伸びるため、平面と平行でない限り必ずどこかで交差する
        return true;

    case LineType::Ray:
        // [光線/レイ]始点から前方にのみ無限に伸びる
        // t >= 0 であれば、進行方向の先で交差している（t < 0 は始点の後方なので衝突なし）
        return t >= 0.0f;

    case LineType::Segment:
        // [線分]始点と終点がある有限の長さ
        // t = 0 で始点、t = 1 で終点を表すため、t が 0.0 ～ 1.0 の範囲にあれば線分上で交差している
        return (t >= 0.0f && t <= 1.0f);
    }

    return false;
}



bool IsCollision(const Line& line, const Triangle& triangle) {
    // 1) 法線を計算する
    Vector3 edge1 = Subtract(triangle.vertices[1], triangle.vertices[0]);
    Vector3 edge2 = Subtract(triangle.vertices[2], triangle.vertices[0]);
    Vector3 normalize = Normalize(Cross(edge1, edge2));

    // 2) 線と「三角形が乗っている無限平面」の交点を求める
    float dot = Dot(normalize, line.diff);
    if (std::abs(dot) < 1e-6f) {
        return false; // 線が平面と平行であるため、通常は交差しない
    }

    // 平面の方程式のd（原点からの距離）を三角形の頂点から計算
    float planeDistance = Dot(normalize, triangle.vertices[0]);
    float t = (planeDistance - Dot(line.origin, normalize)) / dot;

    // 線の種類（線分や射線）に応じて t の範囲をチェック
    if (line.type == LineType::Segment && (t < 0.0f || t > 1.0f)) return false;
    if (line.type == LineType::Ray && t < 0.0f) return false;

    // 交点 p の3D座標を実際に計算する
    Vector3 p = {
        line.origin.x + t * line.diff.x,
        line.origin.y + t * line.diff.y,
        line.origin.z + t * line.diff.z
    };

    // 3) 【重構】交点 p が三角形の内側にあるか判定（ループ処理）
    for (int i = 0; i < 3; i++) {
        // 次の頂点のインデックスを計算 (0->1, 1->2, 2->0 と綺麗に循環します)
        int next = (i + 1) % 3;

        // 現在の辺のベクトルを計算
        Vector3 v_edge = {
            triangle.vertices[next].x - triangle.vertices[i].x,
            triangle.vertices[next].y - triangle.vertices[i].y,
            triangle.vertices[next].z - triangle.vertices[i].z
        };

        // 頂点から交差点pへのベクトルを計算
        Vector3 v_p = {
            p.x - triangle.vertices[next].x,
            p.y - triangle.vertices[next].y,
            p.z - triangle.vertices[next].z
        };

        // クロス積（外積）を取る
        Vector3 crossVec = Cross(v_edge, v_p);

        if (Dot(crossVec, normalize) < 0.0f) {
            return false;
        }
    }

    return true;
}

// 線と平面多角形の衝突判定関数(平面凸多角形だけ)
bool IsCollision(const Line& line, const PolygonV2& polygon) {
    // 安全性チェック：頂点数が3未満の場合、多角形を構成できないため終了
    size_t numVertices = polygon.vertices.size();
    if (numVertices < 3) {
        return false;
    }

    // 1) 法線を計算する（何角形であっても、最初の3頂点から平面の法線を決定できる）
    Vector3 edge1 = Subtract(polygon.vertices[1], polygon.vertices[0]);
    Vector3 edge2 = Subtract(polygon.vertices[2], polygon.vertices[0]);
    Vector3 normalize = Normalize(Cross(edge1, edge2));

    // 2) 線と「多角形が乗っている無限平面」の交点（パラメータ t）を求める
    float dot = Dot(normalize, line.diff);
    if (std::abs(dot) < 1e-6f) {
        return false; // 線が平面と平行であるため、通常は交差しない
    }

    // 平面の方程式のd（原点からの距離）を多角形の最初の頂点から計算
    float planeDistance = Dot(normalize, polygon.vertices[0]);
    float t = (planeDistance - Dot(line.origin, normalize)) / dot;

    // 線の種類（線分や射線）に応じて t の有効範囲をチェック
    if (line.type == LineType::Segment && (t < 0.0f || t > 1.0f)) return false;
    if (line.type == LineType::Ray && t < 0.0f) return false;

    // 交差点 p の3D空間上の座標を実際に計算する (P = O + t * d)
    Vector3 p = {
        line.origin.x + t * line.diff.x,
        line.origin.y + t * line.diff.y,
        line.origin.z + t * line.diff.z
    };

    // 3) 交差点 p が多角形の内側にあるか判定（抽象化されたループ処理）
    for (size_t i = 0; i < numVertices; i++) {
        // 次の頂点のインデックス（最後の頂点の場合は 0 に戻って循環させる）
        size_t next = (i + 1) % numVertices;

        // 現在の辺のベクトルを計算 (v_i -> v_next)
        Vector3 v_edge = {
            polygon.vertices[next].x - polygon.vertices[i].x,
            polygon.vertices[next].y - polygon.vertices[i].y,
            polygon.vertices[next].z - polygon.vertices[i].z
        };

        // 頂点から交差点 p へのベクトルを計算 (v_next -> p)
        Vector3 v_p = {
            p.x - polygon.vertices[next].x,
            p.y - polygon.vertices[next].y,
            p.z - polygon.vertices[next].z
        };

        // クロス積（外積）を計算する
        Vector3 crossVec = Cross(v_edge, v_p);

        // 早期離脱（Early Return）：
        // 交差点がどれか1つの辺に対しても外側にあれば（内積 < 0）、衝突していないため即座に終了
        if (Dot(crossVec, normalize) < 0.0f) {
            return false;
        }
    }

    // すべての辺の内側にあれば衝突（ポリゴンの内部にヒット）
    return true;
}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
    if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x) {
        return false;
    }

    if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y) {
        return false;
    }

    if (aabb1.max.z < aabb2.min.z || aabb1.min.z > aabb2.max.z) {
        return false;
    }

    return true;
}

bool IsCollision(const AABB& aabb, const Sphere& sphere) {
    float closestX = max(aabb.min.x, min(sphere.center.x, aabb.max.x));
    float closestY = max(aabb.min.y, min(sphere.center.y, aabb.max.y));
    float closestZ = max(aabb.min.z, min(sphere.center.z, aabb.max.z));

    float distanceX = sphere.center.x - closestX;
    float distanceY = sphere.center.y - closestY;
    float distanceZ = sphere.center.z - closestZ;

    float distanceSquared = (distanceX * distanceX) +
        (distanceY * distanceY) +
        (distanceZ * distanceZ);

    return distanceSquared <= (sphere.radius * sphere.radius);
}

bool IsCollision(const AABB& aabb, const Line& line) {
    constexpr float eps = 1e-6f;

    float tMin = -std::numeric_limits<float>::infinity();
    float tMax = std::numeric_limits<float>::infinity();

    auto CheckAxis = [&](float origin, float diff, float minValue, float maxValue) -> bool {
        // 線がこの軸方向にほぼ動かない場合
        if (std::abs(diff) < eps) {
            // origin が slab の外にあるなら交差しない
            if (origin < minValue || origin > maxValue) {
                return false;
            }

            // slab の中にあるなら、この軸では制限なし
            return true;
        }

        float t1 = (minValue - origin) / diff;
        float t2 = (maxValue - origin) / diff;

        if (t1 > t2) {
            std::swap(t1, t2);
        }

        tMin = max(tMin, t1);
        tMax = min(tMax, t2);

        // 進入時刻が退出時刻を超えたら交差しない
        if (tMin > tMax) {
            return false;
        }

        return true;
        };

    if (!CheckAxis(line.origin.x, line.diff.x, aabb.min.x, aabb.max.x)) {
        return false;
    }

    if (!CheckAxis(line.origin.y, line.diff.y, aabb.min.y, aabb.max.y)) {
        return false;
    }

    if (!CheckAxis(line.origin.z, line.diff.z, aabb.min.z, aabb.max.z)) {
        return false;
    }

    // 線の種類ごとに t の範囲を制限する
    if (line.type == LineType::Ray) {
        // 射線は t >= 0 の範囲だけ有効
        if (tMax < 0.0f) {
            return false;
        }
    }

    if (line.type == LineType::Segment) {
        // 線分は 0 <= t <= 1 の範囲だけ有効
        if (tMax < 0.0f || tMin > 1.0f) {
            return false;
        }
    }

    return true;
}

bool IsCollision(const OBB& obb, const Sphere& sphere) {
    // 1)球心からOBBの中心への方向ベクトルを計算
    Vector3 dir = {
        sphere.center.x - obb.center.x,
        sphere.center.y - obb.center.y,
        sphere.center.z - obb.center.z
    };

    Vector3 closestPoint = obb.center;

    float extents[3] = { obb.size.x, obb.size.y, obb.size.z };

    // 2)方向ベクトルをOBBの各ローカル軸に射影し、境界内にクランプする
    for (int i = 0; i < 3; ++i) {
        // ベクトル dir を OBB の i 番目の軸に射影した距離
        float dist = Dot(dir, obb.orientations[i]);

        dist = max(-extents[i], min(dist, extents[i]));

        // クランプされた距離を使って最近点の座標を更新
        closestPoint.x += dist * obb.orientations[i].x;
        closestPoint.y += dist * obb.orientations[i].y;
        closestPoint.z += dist * obb.orientations[i].z;
    }

    // 3)算出された最近点と球心との距離の平方を計算
    Vector3 v = {
        sphere.center.x - closestPoint.x,
        sphere.center.y - closestPoint.y,
        sphere.center.z - closestPoint.z
    };
    float distanceSquared = Dot(v, v);

    // 4)距離の平方と半径の平方を比較
    return distanceSquared <= (sphere.radius * sphere.radius);
}

bool IsCollision(const OBB& obb, const Line& line) {
    Matrix4x4 obbInverse = MakeRigidInverse(obb.center, obb.orientations[0], obb.orientations[1], obb.orientations[2]);
    // 2) OBBのローカル座標系に線を変換する
    Vector3 localOrigin = Transform(line.origin, obbInverse);
    Vector3 localDiff = Transform(line.diff, obbInverse);
    // 3) OBBを軸平行な立方体（AABB）として扱い、線とAABBの衝突判定を行う
    AABB aabb{
        .min = { -obb.size.x, -obb.size.y, -obb.size.z },
        .max = { obb.size.x, obb.size.y, obb.size.z }
    };
    Line localLine{
        .origin = localOrigin,
        .diff = localDiff,
        .type = line.type
    };
    return IsCollision(aabb, localLine);
}