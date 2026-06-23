#pragma once
#include <cmath>
#define M_PI 3.14159265358979323846
/**
 * 独立した数学の補助関数や定数の提供。特定のデータ構造（Vector3など）に依存しない、汎用的な計算ツールです。
 **/

float Deg2Rad(float degree) {

    // 度数法（Degree）から弧度法（Radian）へ変換します。
    // 公式：ラジアン = 度 × (π / 180)
    // これにより、透視投影行列（MakePerspectiveFovMatrix）の引数として使える値になります。
    return degree * ((float)M_PI / 180.0f);
}

float Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}