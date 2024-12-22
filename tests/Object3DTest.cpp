#include <gtest/gtest.h>
#include "../Object3D.h"

class Object3DTest : public ::testing::Test {
protected:
    Object3D object;

    void SetUp() override {
        // Initialize the object
        object = Object3D();
    }
};

TEST_F(Object3DTest, DefaultConstructorInitializesFields) {
    EXPECT_EQ(object.texturemap, -1);
    EXPECT_EQ(object.local[0], 0.0f);
    EXPECT_EQ(object.local[1], 0.0f);
    EXPECT_EQ(object.local[2], 0.0f);
}

TEST_F(Object3DTest, SetDiffuseUpdatesDiffuse) {
    object.SetDiffuse(0.5f, 0.6f, 0.7f, 0.8f);
    const float* diffuse = object.GetDiffuse();
    EXPECT_EQ(diffuse[0], 0.5f);
    EXPECT_EQ(diffuse[1], 0.6f);
    EXPECT_EQ(diffuse[2], 0.7f);
    EXPECT_EQ(diffuse[3], 0.8f);
}

TEST_F(Object3DTest, SetNameUpdatesName) {
    object.SetName("TestObject");
    EXPECT_STREQ(object.getName(), "TestObject");
}
