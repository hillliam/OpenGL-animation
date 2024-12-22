#include <gtest/gtest.h>
#include "../Matrix.h"

class MatrixTest : public ::testing::Test {
protected:
    float matrix[16];

    void SetUp() override {
        Matrix::SetIdentity(matrix);
    }
};

TEST_F(MatrixTest, SetIdentitySetsCorrectValues) {
    float identity[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    for (int i = 0; i < 16; ++i) {
        EXPECT_EQ(matrix[i], identity[i]);
    }
}

TEST_F(MatrixTest, TranslateUpdatesMatrix) {
    Matrix::Translate(matrix, 1.0f, 2.0f, 3.0f);
    // Add assertions to verify the translation
}

TEST_F(MatrixTest, ScaleUpdatesMatrix) {
    Matrix::Scale(matrix, 1.0f, 2.0f, 3.0f);
    // Add assertions to verify the scaling
}

TEST_F(MatrixTest, RotateXUpdatesMatrix) {
    Matrix::RotateX(matrix, 45.0f);
    // Add assertions to verify the rotation
}

TEST_F(MatrixTest, RotateYUpdatesMatrix) {
    Matrix::RotateY(matrix, 45.0f);
    // Add assertions to verify the rotation
}

TEST_F(MatrixTest, RotateZUpdatesMatrix) {
    Matrix::RotateZ(matrix, 45.0f);
    // Add assertions to verify the rotation
}
