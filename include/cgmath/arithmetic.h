#pragma once

/// \file
/// Arithmetic.

#include <type_traits>

#include "common.h"

namespace math {
	/// Dummy struct that marks the left hand side.
	struct left_hand_side {
	};
	/// Dummy struct that marks the right hand side.
	struct right_hand_side {
	};

	/// Arithmetic traits of types. Each \p result_type type indicates the return type of the operation, or \p void
	/// if it's not supported.
	namespace arithmetic_traits {
		/// Memberwise addition.
		template <typename Lhs, typename Rhs> struct memberwise_addition {
			using result_type = void; ///< Not supported by default.
		};
		/// Memberwise subtraction.
		template <typename Lhs, typename Rhs> struct memberwise_subtraction {
			using result_type = void; ///< Not supported by default.
		};
		/// Negation.
		template <typename T> struct negation {
			using result_type = void; ///< Not supported by default.
		};
		/// Scalar multiplication.
		template <typename Lhs, typename Rhs> struct scalar_multiplication {
			using result_type = void; ///< Not supported by default.
			/// Indicates which side is the scalar. Should be one of \ref left_hand_side or \ref right_hand_side.
			using scalar_side = void;
		};
		/// Scalar division.
		template <typename Lhs, typename Rhs> struct scalar_division {
			using result_type = void; ///< Not supported by default.
			/// Indicates which side is the scalar. Should be one of \ref left_hand_side or \ref right_hand_side.
			using scalar_side = void;
		};

		/// Equality.
		template <typename Lhs, typename Rhs> struct equality {
			constexpr static bool enabled = false; ///< No equality operator.
		};
	}

	namespace _details {
		/// \p std::enable_if_t with the type itself as the resulting type.
		template <typename T> using enable_if_nonvoid_t = std::enable_if_t<!std::is_same_v<T, void>, T>;
	}


	/// Memberwise addition.
	template <
		typename Lhs, typename Rhs
	> _details::enable_if_nonvoid_t<
		typename arithmetic_traits::memberwise_addition<Lhs, Rhs>::result_type
	> operator+(const Lhs &lhs, const Rhs &rhs) {
		typename arithmetic_traits::memberwise_addition<Lhs, Rhs>::result_type result;
		arr::for_each(
			[](auto &res, const auto &l, const auto &r) {
				res = l + r;
			},
			result, lhs, rhs
		);
		return result;
	}
	/// In-place memberwise addition.
	template <typename Lhs, typename Rhs> std::enable_if_t<
		std::is_same_v<typename arithmetic_traits::memberwise_addition<Lhs, Rhs>::result_type, Lhs>, Lhs&
	> operator+=(Lhs &lhs, const Rhs &rhs) {
		arr::for_each(
			[](auto &l, const auto &r) {
				l += r;
			},
			lhs, rhs
		);
		return lhs;
	}

	/// Memberwise subtraction.
	template <typename Lhs, typename Rhs> _details::enable_if_nonvoid_t<
		typename arithmetic_traits::memberwise_subtraction<Lhs, Rhs>::result_type
	> operator-(const Lhs &lhs, const Rhs &rhs) {
		typename arithmetic_traits::memberwise_subtraction<Lhs, Rhs>::result_type result;
		arr::for_each(
			[](auto &res, const auto &l, const auto &r) {
				res = l - r;
			},
			result, lhs, rhs
		);
		return result;
	}
	/// In-place memberwise subtraction.
	template <typename Lhs, typename Rhs> std::enable_if_t<
		std::is_same_v<typename arithmetic_traits::memberwise_subtraction<Lhs, Rhs>::result_type, Lhs>, Lhs&
	> operator-=(Lhs &lhs, const Rhs &rhs) {
		arr::for_each(
			[](auto &l, const auto &r) {
				l -= r;
			},
			lhs, rhs
		);
		return lhs;
	}

	/// Scalar multiplication.
	template <typename Lhs, typename Rhs> _details::enable_if_nonvoid_t<
		typename arithmetic_traits::scalar_multiplication<Lhs, Rhs>::result_type
	> operator*(const Lhs &lhs, const Rhs &rhs) {
		typename arithmetic_traits::scalar_multiplication<Lhs, Rhs>::result_type result;
		if constexpr (
			std::is_same_v<typename arithmetic_traits::scalar_multiplication<Lhs, Rhs>::scalar_side, left_hand_side>
		) { // scalar on left hand side
			arr::for_each(
				[&lhs](auto &res, const auto &r) {
					res = lhs * r;
				},
				result, rhs
			);
		} else { // scalar on right hand side
			arr::for_each(
				[&rhs](auto &res, const auto &l) {
					res = l * rhs;
				},
				result, lhs
			);
		}
		return result;
	}
	/// In-place scalar multiplication.
	template <typename Lhs, typename Rhs> std::enable_if_t<
		std::is_same_v<typename arithmetic_traits::scalar_multiplication<Lhs, Rhs>::result_type, Lhs> &&
		std::is_same_v<typename arithmetic_traits::scalar_multiplication<Lhs, Rhs>::scalar_side, right_hand_side>,
		Lhs&
	> operator*=(Lhs &lhs, const Rhs &rhs) {
		arr::for_each(
			[&rhs](auto &l) {
				l *= rhs;
			},
			lhs
		);
		return lhs;
	}

	/// Scalar multiplication.
	template <typename Lhs, typename Rhs> _details::enable_if_nonvoid_t<
		typename arithmetic_traits::scalar_division<Lhs, Rhs>::result_type
	> operator/(const Lhs &lhs, const Rhs &rhs) {
		typename arithmetic_traits::scalar_division<Lhs, Rhs>::result_type result;
		if constexpr (
			std::is_same_v<typename arithmetic_traits::scalar_division<Lhs, Rhs>::scalar_side, left_hand_side>
		) { // scalar on left hand side
			arr::for_each(
				[&lhs](auto &res, const auto &r) {
					res = lhs / r;
				},
				result, rhs
					);
		} else { // scalar on right hand side
			arr::for_each(
				[&rhs](auto &res, const auto &l) {
					res = l / rhs;
				},
				result, lhs
					);
		}
		return result;
	}
	/// In-place scalar multiplication.
	template <typename Lhs, typename Rhs> std::enable_if_t<
		std::is_same_v<typename arithmetic_traits::scalar_division<Lhs, Rhs>::result_type, Lhs> &&
		std::is_same_v<typename arithmetic_traits::scalar_division<Lhs, Rhs>::scalar_side, right_hand_side>,
		Lhs&
	> operator/=(Lhs &lhs, const Rhs &rhs) {
		arr::for_each(
			[&rhs](auto &l) {
				l /= rhs;
			},
			lhs
				);
		return lhs;
	}


	/// Equality.
	template <typename Lhs, typename Rhs> std::enable_if_t<
		arithmetic_traits::equality<Lhs, Rhs>::enabled, bool
	> operator==(const Lhs &lhs, const Rhs &rhs) {
		bool result = true;
		arr::for_each(
			[&result](const auto &l, const auto &r) {
				if (l != r) {
					result = false;
					return break_loop(true);
				}
				return break_loop(false);
			},
			lhs, rhs
		);
		return result;
	}
	/// Inequality.
	template <typename Lhs, typename Rhs> std::enable_if_t<
		arithmetic_traits::equality<Lhs, Rhs>::enabled, bool
	> operator!=(const Lhs &lhs, const Rhs &rhs) {
		return !(lhs == rhs);
	}
}
