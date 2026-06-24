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
bool IsCollision(const Line& line, const Plane& plane) {

    // 1. まず垂直判定を行うために、法線と線の内積を求める
    float dot = Dot(plane.normal, line.diff);

    // 垂直＝平行であるので、衝突しているはずがない
    if (dot == 0.0f) {
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