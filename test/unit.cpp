/// \file
/// Unit tests.

#include <gtest/gtest.h>

#include <cgmath/vec.h>
#include <cgmath/point.h>

using namespace math;

// constexpr test
constexpr vec3d a(1.0, 2.0, 3.0);
constexpr vec3d b(5.0, 4.0, 6.0);
constexpr vec3d plus = a + b;
constexpr double a_dot_b = vec3d::dot(a, b);
/*constexpr double a_sqr_norm = a.squared_norm();*/

TEST(array, construction) {
	auto arr1 = array<int, 3>{ { 6, 4, 2 } };
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
	EXPECT_FLOAT_EQ(a.norm<float>(), std::sqrt(133.0f));
	vec3d b(1.0, 2.0, 3.0);
	EXPECT_DOUBLE_EQ(b.norm(), std::sqrt(14.0));
}

TEST(vec, swizzle) {
	vec3i a(1, 3, 5);
	EXPECT_EQ((a.swizzle<1, 2>()), vec2i(3, 5));
	EXPECT_EQ((a.swizzle<1, 2, 0, 0>()), vec4i(3, 5, 1, 1));
}

TEST(vec, normalize) {
	vec3d a(4.0, 2.0, 7.0);
	auto norm_res = a.normalized_nocheck();
	EXPECT_DOUBLE_EQ(norm_res.squared_norm, a.squared_norm());
	EXPECT_DOUBLE_EQ(norm_res.norm, a.norm());
	vec3d ua = norm_res.result;
	EXPECT_DOUBLE_EQ(ua.norm(), 1.0);
}

TEST(unit_vec, arithmetic) {
	vec3d a(1.0, 2.0, 3.0);
	unit_vec3d ua = a.normalized_nocheck().result;
	unit_vec3d ub = -ua;
	EXPECT_DOUBLE_EQ(ub[0], -ua[0]);
	EXPECT_DOUBLE_EQ(ub[1], -ua[1]);
	EXPECT_DOUBLE_EQ(ub[2], -ua[2]);
	vec3d c = ua + ub;
	vec3d d = c + ub;
	d += ua;
}

TEST(unit_vec, norm) {
	vec3d a(1.0, 2.0, 3.0), b(6.0, 5.0, 4.0);
	unit_vec3d ua = a.normalized_nocheck().result, ub = b.normalized_nocheck().result;
	EXPECT_DOUBLE_EQ(ua.norm(), 1.0);
	EXPECT_DOUBLE_EQ(ua.squared_norm(), 1.0);
}

TEST(point, arithmetic) {
	point3i pt(1, 2, 3);
	pt = pt + vec3i(3, 2, 1);
	EXPECT_EQ(pt, point3i(4, 4, 4));
	pt += vec3i(5, 6, 7);
	EXPECT_EQ(pt, point3i(9, 10, 11));
	pt -= vec3i(8, 7, 6);
	EXPECT_EQ(pt, point3i(1, 3, 5));
	EXPECT_EQ(pt.as_vec(), vec3i(1, 3, 5));
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
