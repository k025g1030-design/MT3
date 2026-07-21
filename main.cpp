#include "GraphicsUtils.hpp"
#include "Algebra.hpp"
#include "GeometryTypes.hpp"
#include "Collision.hpp"

const char kWindowTitle[] = "GC2B_08_ラ_ケツブン";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    CameraObj camera = {
        { 0.0f, 3.5f, -7.0f },
        { 0.4f, 0.0f, 0.0f },
        { 1, 1, 1 },
    };


    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, kScreenWidth, kScreenHeight);

    // キー入力結果を受け取る箱
    char keys[256] = {0};
    char preKeys[256] = {0};

    Vector3 rotate1{ 0, 0, 0 };
    Vector3 rotate2{ -0.05f, -2.49f, 0.15f };

    OBB obb1{
        .center{0, 0, 0},
        .orientations {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1},
        },
        .size{0.83f, 0.26f, 0.24f}
    };

    OBB obb2{
        .center{0.9f, 0.66f, 0.78f},
        .orientations {
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1},
        },
        .size{0.5f, 0.37f, 0.5f}
    };

    auto RotateOBB = [](OBB& obb, const Vector3& rotation) {
        Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotation.x), Multiply(MakeRotateYMatrix(rotation.y), MakeRotateZMatrix(rotation.z)));
        obb.orientations[0].x = rotateMatrix.m[0][0];
        obb.orientations[0].y = rotateMatrix.m[0][1];
        obb.orientations[0].z = rotateMatrix.m[0][2];
        obb.orientations[1].x = rotateMatrix.m[1][0];
        obb.orientations[1].y = rotateMatrix.m[1][1];
        obb.orientations[1].z = rotateMatrix.m[1][2];
        obb.orientations[2].x = rotateMatrix.m[2][0];
        obb.orientations[2].y = rotateMatrix.m[2][1];
        obb.orientations[2].z = rotateMatrix.m[2][2];
    };


    float fovY = Deg2Rad(-45.0f);

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // 1. カメラのワールド行列を計算し、その逆行列から「ビュー変換行列」を生成
        // (世界空間の座標を、カメラから見た座標空間へ変換する)
        Matrix4x4 cameraMatrix = MakeAffineMatrix(camera.scale, camera.rotation, camera.position);
        Matrix4x4 viewMatrix = Inverse(cameraMatrix);

        // 2. アスペクト比を計算し、透視投影（ペルスペクティブ）による「プロジェクション変換行列」を生成
        // (遠くのものを小さく、近くのものを大きく表現し、3D空間をクリッピング空間へ変換する)
        float aspect = (float)kScreenWidth / (float)kScreenHeight; 
        Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(fovY, aspect, 0.1f, 100.0f);

        // 3. ビュー行列とプロジェクション行列を合成 (View-Projection 行列)
        // (カメラの視点と画面への投影計算を一本化する)
        Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

        // 4. 正規化デバイス座標(NDC)から、実際の画面ピクセル解像度(1280x720)へマッピングする「ビューポート変換行列」を生成
        Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, (float)kScreenWidth, (float)kScreenHeight, 0.0f, 1.0f);

        // rotate
        RotateOBB(obb1, rotate1);
        RotateOBB(obb2, rotate2);

        // フレームの開始
        Novice::BeginFrame();

        DebugWin(&rotate1, &obb1, &rotate2, &obb2, &camera);

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        DrawGridV2(viewProjectionMatrix, viewportMatrix);
        

        uint32_t color = WHITE;
        if (IsCollision(obb1, obb2)) {
            color = RED;
        }

        DrawOBB(obb1, viewProjectionMatrix, viewportMatrix, color);
        DrawOBB(obb2, viewProjectionMatrix, viewportMatrix, color);
        
        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}


