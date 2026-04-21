#include <Novice.h>
#include <cmath>

const char kWindowTitle[] = "GC2B_08_ラ_ケツブン";
const int kColumnCount = 60;
const int kRowCount = 20;

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


void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
    Novice::ScreenPrintf(x, y, "%s", label);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            Novice::ScreenPrintf(x + j * kColumnCount, y + (i + 1) * kRowCount, "%6.2f", m.m[i][j]);
        }
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = {0};
    char preKeys[256] = {0};

    Matrix4x4 m1 = {
        3.2f, 0.7f, 9.6f, 4.4f,
        5.5f, 1.3f, 7.8f, 2.1f,
        6.9f, 8.0f, 2.6f, 1.0f,
        0.5f, 7.2f, 5.1f, 3.3f,
    };

    Matrix4x4 m2 = {
        4.1f, 6.5f, 3.3f, 2.2f,
        8.8f, 0.6f, 9.9f, 7.7f,
        1.1f, 5.5f, 6.6f, 0.0f,
        3.3f, 9.9f, 8.8f, 2.2f,
	};

    


    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {



        // フレームの開始
        Novice::BeginFrame();


        
        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);
		Matrix4x4 resultAdd = Add(m1, m2);
		Matrix4x4 resultMultiply = Multiply(m1, m2);
		Matrix4x4 resultSubtract = Subtract(m1, m2);
		Matrix4x4 inverseM1 = Inverse(m1);
		Matrix4x4 inverseM2 = Inverse(m2);
		Matrix4x4 transposeM1 = Transpose(m1);
		Matrix4x4 transposeM2 = Transpose(m2);
		Matrix4x4 identity = MakeIdentity4x4();

        MatrixScreenPrintf(0, 0, resultAdd, "Add");
		MatrixScreenPrintf(0, kRowCount * 5, resultSubtract, "Subtract");
        MatrixScreenPrintf(0, kRowCount * 5 * 2, resultMultiply, "Multiply");
        MatrixScreenPrintf(0, kRowCount * 5 * 3, inverseM1, "Inverse M1");
        MatrixScreenPrintf(0, kRowCount * 5 * 4, inverseM2, "Inverse M2");
        MatrixScreenPrintf(kColumnCount * 5, 0, transposeM1, "Transpose M1");
        MatrixScreenPrintf(kColumnCount * 5, kRowCount * 5, transposeM2, "Transpose M2");
        MatrixScreenPrintf(kColumnCount * 5, kRowCount * 5 * 2, identity, "Identity");

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


