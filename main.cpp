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


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, kScreenWidth, kScreenHeight);

    // キー入力結果を受け取る箱
    char keys[256] = {0};
    char preKeys[256] = {0};

	
    Vector3 rotate{};
    Vector3 translate{kScreenWidth / 2, kScreenHeight / 2, 0.0f};
    Vector3 cameraPostion{};
    Vector3 kLocalVertices[3] = {
        { 0.0f, -50.0f, 0.0f },
        { 50.0f, 50.0f, 0.0f },
        { -50.0f, 50.0f, 0.0f }
    };


    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {

        Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, rotate, translate);
        Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, cameraPostion);
        Matrix4x4 viewMatrix = Inverse(cameraMatrix);
        Matrix4x4 projectionMatrix = MakeOrthographicMatrix(0.0f, 0.0f, (float)kScreenWidth, (float)kScreenHeight, 0.0f, 1.0f);
        Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
        Matrix4x4 viewportMatrix = MakeViewportMatrix(0.0f, 0.0f, (float)kScreenWidth, (float)kScreenHeight, 0.0f, 1.0f);
        Vector3 screenVertices[3];
        for (int i = 0; i < 3; ++i) {
            Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
            screenVertices[i] = Transform(ndcVertex, viewportMatrix);
        }

        

        // フレームの開始
        Novice::BeginFrame();

        if (keys[DIK_A] != 0) {
            translate.x -= 1;
        }

        if (keys[DIK_D] != 0) {
            translate.x += 1;
        }

        if (keys[DIK_W] != 0) {
            translate.y -= 1;
        }

        if (keys[DIK_S] != 0) {
            translate.y += 1;
        }

        if (keys[DIK_Q] != 0) {
            rotate.z -= 0.01f;
        }

        if (keys[DIK_E] != 0) {
            rotate.z += 0.01f;
        }

        if (keys[DIK_UP] != 0) {
            rotate.x += 0.01f;
        }

        if (keys[DIK_DOWN] != 0) {
            rotate.x -= 0.01f;
        }

        if (keys[DIK_LEFT] != 0) {
            rotate.y-= 0.01f;
        }

        if (keys[DIK_RIGHT] != 0) {
            rotate.y += 0.01f;
        }




        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        Novice::DrawTriangle(
            (int)screenVertices[0].x, (int)screenVertices[0].y,
            (int)screenVertices[1].x, (int)screenVertices[1].y,
            (int)screenVertices[2].x, (int)screenVertices[2].y,
            RED, kFillModeSolid);



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


