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

    ConicalPendulum pendulum;
    pendulum.anchor = { 0.0f, 1.0f, 0.0f };
    pendulum.length = 0.8f;
    pendulum.halfApexAngle = 0.7f;
    pendulum.angle = 0.0f;
    pendulum.angularVelocity = 0.0f;

    Line line;

    Ball ball;
    ball.position = { 0.0f, 0.0f, 0.0f };
    ball.radius = 0.1f;
    ball.color = RED;

    float fovY = Deg2Rad(45.0f);
    bool start = false;

    float deltaTime = 1.0f / 60.0f; // 60FPSを想定したデルタタイム

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
            pendulum.angularVelocity = std::sqrtf(9.8f / (pendulum.length * std::cosf(pendulum.halfApexAngle)));
            pendulum.angle += pendulum.angularVelocity * deltaTime;

            float radius = pendulum.length * std::sinf(pendulum.halfApexAngle);
            float height = pendulum.length * std::cosf(pendulum.halfApexAngle);
            ball.position.x = pendulum.anchor.x + radius * std::cosf(pendulum.angle);
            ball.position.y = pendulum.anchor.y - height;
            ball.position.z = pendulum.anchor.z + radius * std::sinf(pendulum.angle);

        }

        line.diff = ball.position;
        line.origin = pendulum.anchor;

        // フレームの開始
        Novice::BeginFrame();

        DebugWin(&camera, start);


        DrawGridV2(viewProjectionMatrix, viewportMatrix);

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        DrawLine(line, viewProjectionMatrix, viewportMatrix, GREEN);


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


