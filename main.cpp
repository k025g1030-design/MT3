#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "GC2B_08_ラ_ケツブン";
const int kColumnCount = 60;
const int kRowCount = 20;

struct Vector3 {
    float x, y, z;
};

struct Matrix4x4 {
    float m[4][4];
};

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

Matrix4x4 MakeIdentity4x4() {
    Matrix4x4 result{};
    for (int i = 0; i < 4; ++i) {
        result.m[i][i] = 1.0f;
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


Matrix4x4 MakeTranslateMatrix(const Vector3& translation) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[3][0] = translation.x;
    result.m[3][1] = translation.y;
    result.m[3][2] = translation.z;
    return result;
}

Matrix4x4 makeScaleMatrix(const Vector3& scale) {
    Matrix4x4 result = MakeIdentity4x4();
    result.m[0][0] = scale.x;
    result.m[1][1] = scale.y;
    result.m[2][2] = scale.z;
    return result;
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


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = {0};
    char preKeys[256] = {0};

	


    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {

        Vector3 translate = { 4.1f, 2.6f ,0.8f };
        Vector3 scale = { 1.5f, 5.2f, 7.3f };

        Vector3 point = { 2.3f, 3.8f, 1.4f };
        Matrix4x4 transformMatrix = {
            1.0f, 2.0f, 3.0f, 4.0f,
            3.0f, 1.0f, 1.0f, 2.0f,
            1.0f, 4.0f, 2.0f, 3.0f,
            2.0f, 2.0f, 1.0f, 3.0f
        };

        Vector3 transformed = Transform(point, transformMatrix);

		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
		Matrix4x4 scaleMatrix = makeScaleMatrix(scale);

		

        // フレームの開始
        Novice::BeginFrame();


        
        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

		VectorScreenPrintf(0, 0, transformed, "transformed");
		MatrixScreenPrintf(0, kRowCount, translateMatrix, "translateMatrix");
		MatrixScreenPrintf(0, kRowCount * 6, scaleMatrix, "scaleMatrix");



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


