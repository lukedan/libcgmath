#pragma once

/// \file
/// Unit tests.

#include <gtest/gtest.h>

#include <cgmath/vec.h>

using namespace math;

TEST(array, construction) {
	auto arr1 = array<int, 3>::from_elements(6, 4, 2);
	EXPECT_EQ(arr1[0], 6);
	EXPECT_EQ(arr1[1], 4);
	EXPECT_EQ(arr1[2], 2);
}

TEST(vec, arithmetic) {
	vec3i a(2, 5, 7), b(9, 0, 1);
	EXPECT_EQ(a[2], 7);
	EXPECT_EQ(a + b, vec3i(11, 5, 8));
	EXPECT_EQ(a - b, vec3i(-7, 5, 6));
	EXPECT_EQ(a * 3, vec3i(6, 15, 21));
	EXPECT_EQ(5 * a, vec3i(10, 25, 35));
	EXPECT_EQ(a / 2, vec3i(1, 2, 3));
	a += b;
	EXPECT_NE(a, vec3i(2, 5, 7));
	EXPECT_EQ(a, vec3i(11, 5, 8));
	a *= 2;
	EXPECT_EQ(a, vec3i(22, 10, 16));
	a /= 3;
	EXPECT_EQ(a, vec3i(7, 3, 5));
}

TEST(vec, norm) {
	vec3i a(9, 4, 6);
	EXPECT_EQ(a.squared_norm(), 133);
}

TEST(vec, swizzle) {
	vec3i a(1, 3, 5);
	vec2i b = a.swizzle<1, 2>();
	EXPECT_EQ(b, vec2i(3, 5));
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
