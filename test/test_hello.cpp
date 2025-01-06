#include <gtest/gtest.h>

// 测试函数
int Add(int a, int b) {
    return a + b;
}

// 测试用例
TEST(AddTest, HandlesPositiveInput) {
    EXPECT_EQ(Add(2, 3), 5);
}

// 主函数，运行所有测试
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}