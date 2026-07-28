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

    Plane plane;
    plane.normal = Normalize({ -0.2f, 0.9f, -0.3f});
    plane.distance = 0.0f;

    Ball ball;
    ball.position = { 0.8f, 1.2f, 0.3f };
    ball.mass = 2.0f;
    ball.radius = 0.05f;
    ball.color = WHITE;
    ball.velocity = { 0.0f, 0.0f, 0.0f };

    ball.acceleration = { 0.0f, -9.8f, 0.0f }; // 重力加速度を設定

    float fovY = Deg2Rad(45.0f);
    bool start = false;

    float deltaTime = 1.0f / 60.0f; // 60FPSを想定したデルタタイム
    float e = 0.8f; // 反発係数


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
        Matrix4x4 viewProjectionMatrix = (viewMatrix * projectionMatrix);

        // 4. 正規化デバイス座標(NDC)から、実際の画面ピクセル解像度(1280x720)へマッピングする「ビューポート変換行列」を生成
        Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, (float)kScreenWidth, (float)kScreenHeight, 0.0f, 1.0f);


       

        if (start) {
            // 加速度を速度に加算する
            ball.velocity = ball.velocity + ball.acceleration * deltaTime;

            // 速度を位置に加算する
            ball.position = ball.position + ball.velocity * deltaTime;

            // ボールの中心から平面までの符号付き距離を計算する
            float signedDistance = Dot(ball.position, plane.normal) - plane.distance;

            // ボールが平面に接触、または平面にめり込んでいる場合
            if (signedDistance <= ball.radius) {
                // めり込み量を計算する
                float penetration = ball.radius - signedDistance;

                // ボールを平面の法線方向へ移動させ、めり込みを解消する
                ball.position = ball.position + plane.normal * penetration;

                // 速度の平面法線方向成分を計算する
                float normalVelocity = Dot(ball.velocity, plane.normal);

                // ボールが平面に向かって移動している場合のみ反射処理を行う
                if (normalVelocity < 0.0f) {
                    // 速度を平面の法線方向へ射影する
                    Vector3 normalComponent = plane.normal * normalVelocity;

                    // 速度の平面に沿った接線方向成分を求める
                    Vector3 tangentComponent = ball.velocity - normalComponent;

                    // 法線方向の速度を反転し、反発係数を適用する
                    // 接線方向の速度はそのまま維持する
                    ball.velocity = tangentComponent - normalComponent * e;
                }
            }
        }



        // フレームの開始
        Novice::BeginFrame();

        DebugWin(&camera, start);


        DrawGridV2(viewProjectionMatrix, viewportMatrix);

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        DrawPlane(plane, viewProjectionMatrix, viewportMatrix, GREEN);

        DrawBall(ball.position, ball.radius, viewProjectionMatrix, viewportMatrix, ball.color);


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


