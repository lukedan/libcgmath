#pragma once

/// \file
/// Vector types.

#include "arithmetic.h"
#include "array.h"
#include "impls/norm.h"
#include "impls/swizzle.h"

namespace math {
	template <typename, std::size_t> struct vec;
	namespace _details {
		/// For use in impl inheritance.
		template <typename T> struct typed_vec {
			template <std::size_t Dim> using type = vec<T, Dim>; ///< The corresponding \ref vec type.
		};
	}

	/// Vectors.
	template <typename T, std::size_t Dim> struct vec :
		public array<T, Dim>,
		public impls::norm_op<vec<T, Dim>>,
		public impls::swizzle_op<typename _details::typed_vec<T>::type, Dim>
	{
		/// Default constructor.
		vec() = default;
		/// Initializes all elements of this vector.
		template <typename ...Args> explicit vec(Args &&...args) :
			array(array::from_elements(std::forward<Args>(args)...)) {
		}

		/// Constructs a new vector from its elements.
		template <typename ...Args> inline static vec from_elements(Args &&...args) {
			return vec(std::forward<Args>(args)...);
		}

		/// Dot product.
		inline static T dot(const vec &lhs, const vec &rhs) {
			T result{};
			arr::for_each(
				/*[&result](const auto &l, const auto &r) {
					result += l * r;
				},*/
				[](const auto &l, const auto &r) {
					std::cout << typeid(l).name() << " " << typeid(T).name() << "\n";
				},
				lhs, rhs
					);
			return result;
		}
	};

	namespace arithmetic_traits {
		/// \ref vec + \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<vec<T, Dim>, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref vec - \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<vec<T, Dim>, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// -\ref vec.
		template <typename T, std::size_t Dim> struct negation<vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref vec * \p scalar.
		template <typename T, std::size_t Dim, typename U> struct scalar_multiplication<vec<T, Dim>, U> {
			using result_type = vec<T, Dim>; ///< Assumes that the multiplication does not change the value type.
			using scalar_side = right_hand_side; ///< Right hand side.
		};
		/// \p scalar * \ref vec.
		template <typename T, std::size_t Dim, typename U> struct scalar_multiplication<U, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Assumes that the multiplication does not change the value type.
			using scalar_side = left_hand_side; ///< Left hand side.
		};
		/// \ref vec / \p scalar.
		template <typename T, std::size_t Dim, typename U> struct scalar_division<vec<T, Dim>, U> {
			using result_type = vec<T, Dim>; ///< Assumes that the multiplication does not change the value type.
			using scalar_side = right_hand_side; ///< Right hand side.
		};

		/// \ref vec == \ref vec.
		template <typename T, std::size_t Dim> struct equality<vec<T, Dim>, vec<T, Dim>> {
			constexpr static bool enabled = std::is_integral_v<T>; ///< Only enable for integers.
		};
	}

	template <typename T> using vec2 = vec<T, 2>; ///< Shorthand for 2D vectors.
	using vec2f = vec2<float>; ///< Shorthand for 2D \p float vectors.
	using vec2d = vec2<double>; ///< Shorthand for 2D \p double vectors.
	using vec2i = vec2<int>; ///< Shorthand for 2D \p int vectors.
	using vec2s = vec2<std::size_t>; ///< Shorthand for 2D \p std::size_t vectors.

	template <typename T> using vec3 = vec<T, 3>; ///< Shorthand for 3D vectors.
	using vec3f = vec3<float>; ///< Shorthand for 3D \p float vectors.
	using vec3d = vec3<double>; ///< Shorthand for 3D \p double vectors.
	using vec3i = vec3<int>; ///< Shorthand for 3D \p int vectors.
	using vec3s = vec3<std::size_t>; ///< Shorthand for 3D \p std::size_t vectors.

	template <typename T> using vec4 = vec<T, 4>; ///< Shorthand for 4D vectors.
	using vec4f = vec4<float>; ///< Shorthand for 4D \p float vectors.
	using vec4d = vec4<double>; ///< Shorthand for 4D \p double vectors.
	using vec4i = vec4<int>; ///< Shorthand for 4D \p int vectors.
	using vec4s = vec4<std::size_t>; ///< Shorthand for 4D \p std::size_t vectors.
}
