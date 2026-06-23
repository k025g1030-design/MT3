#pragma once
#include "GeometryTypes.hpp"
#include "Algebra.hpp"
#include "MathUtility.hpp"
/**
 * 幾何学的なプリミティブ（線分、球など）間の空間的関係、最短距離の計算、および交差（衝突）判定を行う。
 */

Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
    Vector3 v = Subtract(point, segment.origin);

    float lengthSq = Dot(segment.diff, segment.diff);
    float t = 0.0f;

    if (lengthSq != 0.0f) {
        t = Dot(v, segment.diff) / lengthSq;
    }

    t = Clamp(t, 0.0f, 1.0f);

    return Add(segment.origin, Multiply(t, segment.diff));
}