#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "GC2B_08_ラ_ケツブン";
const int kColumnCount = 60;
const int kRowCount = 20;

struct Vector3 {
    float x;
    float y;
    float z;
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
    return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
}

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
    return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

// スカラー倍
Vector3 Multiply(const Vector3& v, float scalar) {
    return {v.x * scalar, v.y * scalar, v.z * scalar};
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}   

// ベクトルの長さ
float Length(const Vector3& v) {
    return sqrtf(Dot(v, v));
}

// 正規化
Vector3 Normalize(const Vector3& v) {
    float length = Length(v);
    if (length == 0.0f) {
        return {0.0f, 0.0f, 0.0f}; // Avoid division by zero
    }
    return Multiply(v, 1.0f / length);
}

void VectorScreenPrintf(int x, int y, const Vector3& v, const char* label) {
    Novice::ScreenPrintf(x, y, "%.02f", v.x);
    Novice::ScreenPrintf(x + kColumnCount, y, "%.02f", v.y);
    Novice::ScreenPrintf(x + kColumnCount * 2, y, "%.02f", v.z);
    Novice::ScreenPrintf(x + kColumnCount * 3, y, "%s", label);
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = {0};
    char preKeys[256] = {0};

    Vector3 v1 = { 1.0f, 3.0f, -5.0f };
    Vector3 v2 = { 4.0f, -1.0f, 2.0f };
    float k = { 4.0f };

    Vector3 addResult = Add(v1, v2);
    Vector3 subtractResult = Subtract(v1, v2);
    Vector3 multiplyResult = Multiply(v1, k);
    float dotResult = Dot(v1, v2);
    float lengthResult = Length(v1);
    Vector3 normalizeResult = Normalize(v2);


    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {



        // フレームの開始
        Novice::BeginFrame();


        
        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);


        VectorScreenPrintf(0, 0, addResult, " : Add");
        VectorScreenPrintf(0, kRowCount, subtractResult, " : Subtract");
        VectorScreenPrintf(0, kRowCount * 2, multiplyResult, " : Multiply");
        Novice::ScreenPrintf(0, kRowCount * 3, "%.02f : Dot", dotResult);
        Novice::ScreenPrintf(0, kRowCount * 4, "%.02f : Length", lengthResult);
        VectorScreenPrintf(0, kRowCount * 5, normalizeResult, " : Normalize");

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


