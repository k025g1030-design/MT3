#pragma once
#include <Novice.h>
#include <imgui.h>
#include "Transformations.hpp"

const int kColumnCount = 60;
const int kRowCount = 20;
const int kScreenWidth = 1280;
const int kScreenHeight = 720;

/**
 * メモリ上の数学・幾何データを視覚化（レンダリング）し、デバッグ用のインターフェースを提供すること。
 * グラフィックスAPI（NoviceやImGui）と直接通信することを許された唯一の層です。
 **/

void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label) {
    Novice::ScreenPrintf(x, y, "%.02f", v.x);
    Novice::ScreenPrintf(x + kColumnCount, y, "%.02f", v.y);
    Novice::ScreenPrintf(x + kColumnCount * 2, y, "%.02f", v.z);
    Novice::ScreenPrintf(x + kColumnCount * 3, y, "%s", label);
}

void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            Novice::ScreenPrintf(x + j * kColumnCount, y + (i + 1) * kRowCount, "%6.2f", m.m[i][j]);
        }
    }
}

void DrawLine(const Vector3& start, const Vector3& end, uint32_t color) {
    Novice::DrawLine((int)start.x, (int)start.y, (int)end.x, (int)end.y, color);
}

void DrawLine(const Line& line, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    Vector3 start = Transform(Transform(line.origin, viewProjectionMatrix), viewportMatrix);
    Vector3 end = Transform(Transform(line.diff, viewProjectionMatrix), viewportMatrix);
    DrawLine(start, end, color);
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
    const float kGridHalfWidth = 2.0f;
    const uint32_t kSubdivision = 10;
    const float kGridEvery = (kGridHalfWidth * 2.0f) / (float)kSubdivision;
    for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
        float x = -kGridHalfWidth + ((float)xIndex * kGridEvery);

        Vector3 startWorld = { x, 0.0f, -kGridHalfWidth };
        Vector3 endWord = { x, 0.0f, kGridHalfWidth };

        Vector3 startNdc = Transform(startWorld, viewProjectionMatrix);
        Vector3 startScreen = Transform(startNdc, viewportMatrix);

        Vector3 endNdc = Transform(endWord, viewProjectionMatrix);
        Vector3 endScreen = Transform(endNdc, viewportMatrix);

        uint32_t color = 0xAAAAAAFF;
        if (std::abs(x) < 0.001f) {
            color = 0x000000FF;
        }

        DrawLine(startScreen, endScreen, color );
    }

    for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
        float z = -kGridHalfWidth + ((float)zIndex * kGridEvery);

        Vector3 startWorld = { -kGridHalfWidth, 0.0f, z };
        Vector3 endWord = { kGridHalfWidth, 0.0f, z };

        Vector3 startNdc = Transform(startWorld, viewProjectionMatrix);
        Vector3 startScreen = Transform(startNdc, viewportMatrix);

        Vector3 endNdc = Transform(endWord, viewProjectionMatrix);
        Vector3 endScreen = Transform(endNdc, viewportMatrix);

        uint32_t color = 0xAAAAAAFF;
        if (std::abs(z) < 0.001f) {
            color = 0x000000FF;
        }

        DrawLine(startScreen, endScreen, color);
    }
}

void DrawGridV2(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
    const float kGridHalfWidth = 2.0f;               // グリッドの半分の幅 (中心から端までの距離)
    const uint32_t kSubdivision = 10;                // 分割数 (何マスに区切るか)
    const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1マス分の幅 (0.4)

    // 1つのループで縦線（Z軸平行）と横線（X軸平行）を同時に描画
    for (uint32_t i = 0; i <= kSubdivision; ++i) {
        // 現在の線が通る世界座標（World Space）上の位置（オフセット量）を計算
        float value = -kGridHalfWidth + (float(i) * kGridEvery);

        // 原点（軸線）に近い線は黒色(0x000000FF)、それ以外は薄い灰色(0xAAAAAAFF)に設定
        uint32_t color = (std::abs(value) < 0.001f) ? 0x000000FF : 0xAAAAAAFF;

        // ==========================================
        // 1. 縦線の描画（Z軸に平行：X座標を固定し、Zの奥から手前へ繋ぐ）
        // ==========================================
        Vector3 startZ = { value, 0.0f, -kGridHalfWidth }; // 始点（奥）
        Vector3 endZ = { value, 0.0f, kGridHalfWidth }; // 終点（手前）

        // 世界座標 -> 3D空間（WVP変換） -> 画面ピクセル座標（ビューポート変換）へ一気に変換
        Vector3 sScreenZ = Transform(Transform(startZ, viewProjectionMatrix), viewportMatrix);
        Vector3 eScreenZ = Transform(Transform(endZ, viewProjectionMatrix), viewportMatrix);

        // 2Dの画面上にラインを描画
        Novice::DrawLine((int)sScreenZ.x, (int)sScreenZ.y, (int)eScreenZ.x, (int)eScreenZ.y, color);

        // ==========================================
        // 2. 横線の描画（X軸に平行：Z座標を固定し、Xの左から右へ繋ぐ）
        // ==========================================
        Vector3 startX = { -kGridHalfWidth, 0.0f, value }; // 始点（左）
        Vector3 endX = { kGridHalfWidth, 0.0f, value }; // 終点（右）

        // 同じく座標変換を行う
        Vector3 sScreenX = Transform(Transform(startX, viewProjectionMatrix), viewportMatrix);
        Vector3 eScreenX = Transform(Transform(endX, viewProjectionMatrix), viewportMatrix);

        // 2Dの画面上にラインを描画
        DrawLine(sScreenX, eScreenX, color);
    }
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    const uint32_t kSubdivision = 16;                         // 分割数（16や20くらいが綺麗です）
    const float kLonEvery = (2.0f * (float)M_PI) / float(kSubdivision); // 経度分割1つ分の角度 (360度 / 分割数)
    const float kLatEvery = (float)M_PI / float(kSubdivision);          // 緯度分割1つ分の角度 (180度 / 分割数)

    // 緯度の方向に分割 -π/2 ～ π/2
    for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
        float lat = -(float)M_PI / 2.0f + kLatEvery * latIndex; // 現在の緯度

        // 経度の方向に分割 0 ～ 2π
        for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
            float lon = lonIndex * kLonEvery; // 現在の経度

            // world座標系でのa, b, cを求める
            Vector3 a, b, c;

            // 点a: 現在の緯度・経度からXYZを計算
            a.x = sphere.center.x + sphere.radius * std::cos(lat) * std::cos(lon);
            a.y = sphere.center.y + sphere.radius * std::sin(lat);
            a.z = sphere.center.z + sphere.radius * std::cos(lat) * std::sin(lon);

            // 点b: 次の経度 (緯度はそのまま。経度方向に線を引くため)
            float nextLon = lon + kLonEvery;
            b.x = sphere.center.x + sphere.radius * std::cos(lat) * std::cos(nextLon);
            b.y = sphere.center.y + sphere.radius * std::sin(lat);
            b.z = sphere.center.z + sphere.radius * std::cos(lat) * std::sin(nextLon);

            // 点c: 次の緯度 (経度はそのまま。緯度方向に線を引くため)
            float nextLat = lat + kLatEvery;
            c.x = sphere.center.x + sphere.radius * std::cos(nextLat) * std::cos(lon);
            c.y = sphere.center.y + sphere.radius * std::sin(nextLat);
            c.z = sphere.center.z + sphere.radius * std::cos(nextLat) * std::sin(lon);

            // a, b, cをScreen座標系まで変換
            Vector3 aScreen = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
            Vector3 bScreen = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
            Vector3 cScreen = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

            // ab, acで線を引く (ワイヤーフレームの四角形を構成する2辺)
            DrawLine(aScreen, bScreen, color);
            DrawLine(aScreen, cScreen, color);
        }
    }
}



void DrawPolygon(const PolygonV2& polygon, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    // 安全性チェック：頂点数が3未満（多角形を形成できない）の場合は描画しない
    size_t numVertices = polygon.vertices.size();
    if (numVertices < 3) {
        return;
    }

    // スクリーン座標に変換された頂点を格納する動的配列
    std::vector<Vector3> screenPoints(numVertices);

    // 1) 座標変換ループ: すべての頂点を3D世界空間からスクリーン空間へ
    for (size_t i = 0; i < numVertices; ++i) {
        // 視点・投影変換マトリックスを適用
        Vector3 projected = Transform(polygon.vertices[i], viewProjectionMatrix);

        // ビューポートマトリックスを適用し、最終的なスクリーン座標（ピクセル単位）を算出
        // ※この内部で Z の消費（透視除法）が行われます
        screenPoints[i] = Transform(projected, viewportMatrix);
    }

    // 2) 描画ループ: 変換されたスクリーン座標を線で結んで閉じた多角形を作る
    for (size_t i = 0; i < numVertices; ++i) {
        // 次の頂点のインデックスを計算
        // 剰余演算（%）を使うことで、最後の頂点の次は 0（最初の頂点）に戻り、ループが閉じる
        size_t nextIndex = (i + 1) % numVertices;

        // トポロジーの描画（隣り合う頂点同士を線分で結ぶ）
        DrawLine(screenPoints[i], screenPoints[nextIndex], color);
    }
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    Vector3 screenPoints[3];

    // 1) 座標変換: 3つの頂点を3D空間から2Dスクリーン空間へ
    for (int i = 0; i < 3; ++i) {
        // ビュー・プロジェクション変換（カメラ空間 -> クリップ空間へ）
        Vector3 projected = Transform(triangle.vertices[i], viewProjectionMatrix);

        // ビューポート変換（クリップ空間 -> スクリーンピクセル座標へ）
        // ※この Transform の内部で Z の消費（透視除法）が行われている前提です
        screenPoints[i] = Transform(projected, viewportMatrix);
    }

    // 2) 描画: 頂点を結んで三角形のトポロジー（ワイヤーフレーム）を構築
    // 動的な for ループや剰余演算（%）を使わず、直接展開して処理を高速化（Loop Unrolling）
    DrawLine(screenPoints[0], screenPoints[1], color);
    DrawLine(screenPoints[1], screenPoints[2], color);
    DrawLine(screenPoints[2], screenPoints[0], color); // 最後に始点へ戻って閉じる
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    // Zの役割 1 & 2: 空間基準と直交基底の構築
    Vector3 center = Multiply(plane.distance, plane.normal);
    Vector3 right = Normalize(Perpendicular(plane.normal));
    Vector3 up = Cross(plane.normal, right);
    float size = 2.0f; // 平面を描画する際のスケール

    // 四隅の方向ベクトル乗数 (Ring Order: 左上, 右上, 右下, 左下)
    const float signs[4][2] = {
        { -1.0f, 1.0f },   // 0: Top-Left    (-right, +up)
        { 1.0f, 1.0f },    // 1: Top-Right   (+right, +up)
        { 1.0f, -1.0f },   // 2: Bottom-Right(+right, -up)
        { -1.0f, -1.0f }   // 3: Bottom-Left (-right, -up)
    };

    Vector3 points[4];
    for (uint32_t i = 0; i < 4; ++i) {
        // 頂点の生成: 基準点に対して、rightとupを配列の符号に従って加算
        Vector3 offset = Add(Multiply(size * signs[i][0], right), Multiply(size * signs[i][1], up));
        Vector3 point = Add(center, offset);

        // Zの役割: 透視投影のトリガー】
        // point.z を透視除法の係数として消費し、スクリーン座標へ変換
        points[i] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
    }


    for (int i = 0; i < 4; ++i) {
        // 次の頂点のインデックスを計算 (0->1, 1->2, 2->3, 3->0)
        // 剰余演算 (%) を使うことで、3の次は0に戻るようにループを閉じる
        int nextIndex = (i + 1) % 4;
        // トポロジーの描画と Z の破棄
        DrawLine(points[i], points[nextIndex], color);
    }
    
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
    // 1) AABB の min と max から、3D空間上の8つの頂点を構築する
    Vector3 localVertices[8];

    // 手前側の4頂点 (Z = min.z)
    localVertices[0] = { aabb.min.x, aabb.min.y, aabb.min.z }; // 左下奥
    localVertices[1] = { aabb.max.x, aabb.min.y, aabb.min.z }; // 右下奥
    localVertices[2] = { aabb.min.x, aabb.max.y, aabb.min.z }; // 左上奥
    localVertices[3] = { aabb.max.x, aabb.max.y, aabb.min.z }; // 右上奥

    // 奥側の4頂点 (Z = max.z)
    localVertices[4] = { aabb.min.x, aabb.min.y, aabb.max.z }; // 左下手前
    localVertices[5] = { aabb.max.x, aabb.min.y, aabb.max.z }; // 右下手前
    localVertices[6] = { aabb.min.x, aabb.max.y, aabb.max.z }; // 左上手前
    localVertices[7] = { aabb.max.x, aabb.max.y, aabb.max.z }; // 右上手前

    // 2) 座標変換: 8つの頂点を3D空間から2Dスクリーン空間へ
    Vector3 screenPoints[8];
    for (int i = 0; i < 8; ++i) {
        Vector3 projected = Transform(localVertices[i], viewProjectionMatrix);
        screenPoints[i] = Transform(projected, viewportMatrix);
    }

    // 3) 描画: 12本の辺を結んでワイヤーフレームの立方体を構築

    // 手前側の面 (Z = min.z の四角形)
    DrawLine(screenPoints[0], screenPoints[1], color);
    DrawLine(screenPoints[1], screenPoints[3], color);
    DrawLine(screenPoints[3], screenPoints[2], color);
    DrawLine(screenPoints[2], screenPoints[0], color);

    // 奥側の面 (Z = max.z の四角形)
    DrawLine(screenPoints[4], screenPoints[5], color);
    DrawLine(screenPoints[5], screenPoints[7], color);
    DrawLine(screenPoints[7], screenPoints[6], color);
    DrawLine(screenPoints[6], screenPoints[4], color);

    // 手前と奥を繋ぐ4本の柱（縦の辺）
    DrawLine(screenPoints[0], screenPoints[4], color);
    DrawLine(screenPoints[1], screenPoints[5], color);
    DrawLine(screenPoints[2], screenPoints[6], color);
    DrawLine(screenPoints[3], screenPoints[7], color);
}


void DebugWin(AABB* aabb1, Sphere* sphere, CameraObj* camera) {
    ImGui::Begin("DEBUG");
    ImGui::DragFloat3("CameraTranslate", &camera->position.x, 0.01f);
    ImGui::DragFloat3("CameraRotate", &camera->rotation.x, 0.01f);
    ImGui::DragFloat3("aabb1.min", &aabb1->min.x, 0.01f);
    ImGui::DragFloat3("aabb1.max", &aabb1->max.x, 0.01f);
    ImGui::DragFloat3("sphere.center", &sphere->center.x, 0.01f);
    ImGui::DragFloat("sphere.radius", &sphere->radius, 0.01f);
    ImGui::End();
}


void DebugWin(AABB* aabb1, Line* line, CameraObj* camera) {
    ImGui::Begin("DEBUG");
    ImGui::DragFloat3("CameraTranslate", &camera->position.x, 0.01f);
    ImGui::DragFloat3("CameraRotate", &camera->rotation.x, 0.01f);
    ImGui::DragFloat3("aabb1.min", &aabb1->min.x, 0.01f);
    ImGui::DragFloat3("aabb1.max", &aabb1->max.x, 0.01f);
    ImGui::DragFloat3("line.origin", &line->origin.x, 0.01f);
    ImGui::DragFloat3("line.diff", &line->diff.x, 0.01f);
    ImGui::End();
}

void TestCorss() {
    Vector3 v1{ 1.2f, -3.9f, 2.5f };
    Vector3 v2{ 2.8f, 0.4f, -1.3f };
    VectorScreenPrintf(0, 0, Cross(v1, v2), "Cross");

}
