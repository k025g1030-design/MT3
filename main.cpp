#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "GC2B_08_ラ_ケツブン";
const int kColumnCount = 60;
const int kRowCount = 20;
const int kScreenWidth = 1280;
const int kScreenHeight = 720;

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

float Deg2Rad(float degree) {
    // 2. 逆三角関数を使って、正確な円周率（π = 3.1415926...）を計算します。
    // cos(π) = -1 利用し、std::acos(-1.0f) で精度の高い π を取得しています。
    const float pi = std::acos(-1.0f);

    // 3. 度数法（Degree）から弧度法（Radian）へ変換します。
    // 公式：ラジアン = 度 × (π / 180)
    // これにより、透視投影行列（MakePerspectiveFovMatrix）の引数として使える値になります。
    return degree * (pi / 180.0f);
}

Matrix4x4 MakeIdentity4x4() {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        result.m[i][i] = 1.0f;
    }
    return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }
    return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translation) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[3][0] = translation.x;
    result.m[3][1] = translation.y;
    result.m[3][2] = translation.z;
    return result;
}




Matrix4x4 MakeRotateXMatrix(float radian) {
    Matrix4x4 result = MakeIdentity4x4();
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    result.m[1][1] = cosTheta;
    result.m[1][2] = sinTheta;
    result.m[2][1] = -sinTheta;
    result.m[2][2] = cosTheta;
    return result;
}
Matrix4x4 MakeRotateYMatrix(float radian) {
    Matrix4x4 result = MakeIdentity4x4();
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    result.m[0][0] = cosTheta;
    result.m[0][2] = -sinTheta;
    result.m[2][0] = sinTheta;
    result.m[2][2] = cosTheta;
    return result;
}
Matrix4x4 MakeRotateZMatrix(float radian) {
    Matrix4x4 result = MakeIdentity4x4();
    float cosTheta = std::cos(radian);
    float sinTheta = std::sin(radian);
    result.m[0][0] = cosTheta;
    result.m[0][1] = sinTheta;
    result.m[1][0] = -sinTheta;
    result.m[1][1] = cosTheta;
    return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
    Matrix4x4 result = MakeIdentity4x4();
    Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
    Matrix4x4 rotateX = MakeRotateXMatrix(rotation.x);
    Matrix4x4 rotateY = MakeRotateYMatrix(rotation.y);
    Matrix4x4 rotateZ = MakeRotateZMatrix(rotation.z);
    Matrix4x4 translateMatrix = MakeTranslateMatrix(translation);
    
    Matrix4x4 rotateXYZ = Multiply(rotateX, Multiply(rotateY, rotateZ));
    //result = Multiply(rotateXYZ, translateMatrix);
    //result = Multiply(scaleMatrix, result);
    result = Multiply(Multiply(scaleMatrix, rotateXYZ), translateMatrix);

    return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = 2.0f / (right - left);
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[2][2] = -2.0f / (farClip - nearClip);
    result.m[3][0] = -(right + left) / (right - left);
    result.m[3][1] = -(top + bottom) / (top - bottom);
    result.m[3][2] = -(farClip + nearClip) / (farClip - nearClip);
    return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspect, float nearClip, float farClip) {
    Matrix4x4 result{};
    float f = 1.0f / std::tan(fovY / 2.0f);
    result.m[0][0] = f / aspect;
    result.m[1][1] = f;
    result.m[2][2] = (farClip + nearClip) / (nearClip - farClip);
    result.m[2][3] = -1.0f;
    result.m[3][2] = (2.0f * farClip * nearClip) / (nearClip - farClip);
    return result;
}


Matrix4x4 MakeViewportMatrix(float x, float y, float width, float height, float minDepth, float maxDepth) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = width / 2.0f;
    result.m[1][1] = -height / 2.0f; // Y軸を反転
    result.m[2][2] = maxDepth - minDepth;
    result.m[3][0] = x + width / 2.0f;
    result.m[3][1] = y + height / 2.0f;
    result.m[3][2] = minDepth;
    return result;
}



Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m1.m[i][j] + m2.m[i][j];
        }
    }
    return result;
}

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m1.m[i][j] - m2.m[i][j];
        }
    }
    return result;
}



Matrix4x4 Inverse(const Matrix4x4& mat) {
    Matrix4x4 res;

    res.m[0][0] = mat.m[1][1] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[1][2] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) +
        mat.m[1][3] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]);
    res.m[0][1] = -(mat.m[0][1] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) +
        mat.m[0][3] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]));
    res.m[0][2] = mat.m[0][1] * (mat.m[1][2] * mat.m[3][3] - mat.m[1][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[1][1] * mat.m[3][3] - mat.m[1][3] * mat.m[3][1]) +
        mat.m[0][3] * (mat.m[1][1] * mat.m[3][2] - mat.m[1][2] * mat.m[3][1]);
    res.m[0][3] = -(mat.m[0][1] * (mat.m[1][2] * mat.m[2][3] - mat.m[1][3] * mat.m[2][2]) -
        mat.m[0][2] * (mat.m[1][1] * mat.m[2][3] - mat.m[1][3] * mat.m[2][1]) +
        mat.m[0][3] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]));

    res.m[1][0] = -(mat.m[1][0] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[1][2] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[1][3] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]));
    res.m[1][1] = mat.m[0][0] * (mat.m[2][2] * mat.m[3][3] - mat.m[2][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]);
    res.m[1][2] = -(mat.m[0][0] * (mat.m[1][2] * mat.m[3][3] - mat.m[1][3] * mat.m[3][2]) -
        mat.m[0][2] * (mat.m[1][0] * mat.m[3][3] - mat.m[1][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[3][2] - mat.m[1][2] * mat.m[3][0]));
    res.m[1][3] = mat.m[0][0] * (mat.m[1][2] * mat.m[2][3] - mat.m[1][3] * mat.m[2][2]) -
        mat.m[0][2] * (mat.m[1][0] * mat.m[2][3] - mat.m[1][3] * mat.m[2][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]);

    res.m[2][0] = mat.m[1][0] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) -
        mat.m[1][1] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[1][3] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]);
    res.m[2][1] = -(mat.m[0][0] * (mat.m[2][1] * mat.m[3][3] - mat.m[2][3] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[2][0] * mat.m[3][3] - mat.m[2][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]));
    res.m[2][2] = mat.m[0][0] * (mat.m[1][1] * mat.m[3][3] - mat.m[1][3] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[3][3] - mat.m[1][3] * mat.m[3][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[3][1] - mat.m[1][1] * mat.m[3][0]);
    res.m[2][3] = -(mat.m[0][0] * (mat.m[1][1] * mat.m[2][3] - mat.m[1][3] * mat.m[2][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[2][3] - mat.m[1][3] * mat.m[2][0]) +
        mat.m[0][3] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]));

    res.m[3][0] = -(mat.m[1][0] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]) -
        mat.m[1][1] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]) +
        mat.m[1][2] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]));
    res.m[3][1] = mat.m[0][0] * (mat.m[2][1] * mat.m[3][2] - mat.m[2][2] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[2][0] * mat.m[3][2] - mat.m[2][2] * mat.m[3][0]) +
        mat.m[0][2] * (mat.m[2][0] * mat.m[3][1] - mat.m[2][1] * mat.m[3][0]);
    res.m[3][2] = -(mat.m[0][0] * (mat.m[1][1] * mat.m[3][2] - mat.m[1][2] * mat.m[3][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[3][2] - mat.m[1][2] * mat.m[3][0]) +
        mat.m[0][2] * (mat.m[1][0] * mat.m[3][1] - mat.m[1][1] * mat.m[3][0]));
    res.m[3][3] = mat.m[0][0] * (mat.m[1][1] * mat.m[2][2] - mat.m[1][2] * mat.m[2][1]) -
        mat.m[0][1] * (mat.m[1][0] * mat.m[2][2] - mat.m[1][2] * mat.m[2][0]) +
        mat.m[0][2] * (mat.m[1][0] * mat.m[2][1] - mat.m[1][1] * mat.m[2][0]);

    float det = mat.m[0][0] * res.m[0][0] + mat.m[0][1] * res.m[1][0] + mat.m[0][2] * res.m[2][0] + mat.m[0][3] * res.m[3][0];

    float invDet = 1.0f / det;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res.m[i][j] *= invDet;
        }
    }

    return res;
}

Matrix4x4 Transpose(const Matrix4x4& m) {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = m.m[j][i];
        }
    }
    return result;
}



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





Vector3 Transform(const Vector3& point, const Matrix4x4& matrix) {
    Vector3 result = {};
    result.x = point.x * matrix.m[0][0] + point.y * matrix.m[1][0] + point.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
    result.y = point.x * matrix.m[0][1] + point.y * matrix.m[1][1] + point.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
    result.z = point.x * matrix.m[0][2] + point.y * matrix.m[1][2] + point.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
    float w = point.x * matrix.m[0][3] + point.y * matrix.m[1][3] + point.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

    if (w != 0.0f) {
        result = { result.x / w, result.y / w, result.z / w };
    }

    return result;
}


Vector3 Cross(const Vector3& v1, const Vector3& v2) {
    return Vector3(
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x)
    );
}


void TestCorss() {
    Vector3 v1{ 1.2f, -3.9f, 2.5f };
    Vector3 v2{ 2.8f, 0.4f, -1.3f };
    VectorScreenPrintf(0, 0, Cross(v1, v2), "Cross");

}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
    const float kGridHalfWidth = 2.0f;
    const uint32_t kSubdivision = 10;
    const float kGridEvery = (kGridHalfWidth * 2.0f) / (float)kSubdivision;
    for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
        float x = -kGridHalfWidth + ((float)xIndex * kGridEvery);
        
        Vector3 startWorld = {x, 0.0f, -kGridHalfWidth };
        Vector3 endWord = { x, 0.0f, kGridHalfWidth };

        Vector3 startNdc = Transform(startWorld, viewProjectionMatrix);
        Vector3 startScreen = Transform(startNdc, viewportMatrix);

        Vector3 endNdc = Transform(endWord, viewProjectionMatrix);
        Vector3 endScreen = Transform(endNdc, viewportMatrix);

        uint32_t color = 0xAAAAAAFF;
        if (std::abs(x) < 0.001f) {
            color = 0x000000FF;
        }

        Novice::DrawLine(
            (int)startScreen.x, (int)startScreen.y,
            (int)endScreen.x, (int)endScreen.y,
            color
        );
    }

    for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
        float z = -kGridHalfWidth + ((float)zIndex * kGridEvery);

        Vector3 startWorld = { -kGridHalfWidth, 0.0f,  z};
        Vector3 endWord = { kGridHalfWidth, 0.0f,  z};

        Vector3 startNdc = Transform(startWorld, viewProjectionMatrix);
        Vector3 startScreen = Transform(startNdc, viewportMatrix);

        Vector3 endNdc = Transform(endWord, viewProjectionMatrix);
        Vector3 endScreen = Transform(endNdc, viewportMatrix);

        uint32_t color = 0xAAAAAAFF;
        if (std::abs(z) < 0.001f) {
            color = 0x000000FF;
        }

        Novice::DrawLine(
            (int)startScreen.x, (int)startScreen.y,
            (int)endScreen.x, (int)endScreen.y,
            color
        );
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
        Novice::DrawLine((int)sScreenX.x, (int)sScreenX.y, (int)eScreenX.x, (int)eScreenX.y, color);
    }
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, kScreenWidth, kScreenHeight);

    // キー入力結果を受け取る箱
    char keys[256] = {0};
    char preKeys[256] = {0};

	
    Vector3 rotate{};
    Vector3 translate{kScreenWidth / 2, kScreenHeight / 2, 0.0f};
    CameraObj camera = {
        { 0.0f, 3.5f, -7.0f },
        { 0.4f, 0.0f, 0.0f },
        { 1, 1, 1},
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

        

        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        DrawGridV2(viewProjectionMatrix, viewportMatrix);

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


