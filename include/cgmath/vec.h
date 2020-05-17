#pragma once

/// \file
/// Vector types.

#include <cstdio>
#include <typeinfo>

#include "common.h"
#include "arithmetic.h"
#include "array.h"
#include "impls/dot.h"
#include "impls/norm.h"
#include "impls/swizzle.h"

namespace math {
	template <typename, std::size_t> struct vec;
	template <typename, std::size_t> struct point;
	template <typename, std::size_t> struct unit_vec;
	namespace _details {
		/// For use in impl inheritance.
		template <typename T> struct typed_vec {
			template <std::size_t Dim> using type = vec<T, Dim>; ///< The corresponding \ref vec type.
		};
		/// Shorthand for \ref typed_vec::type.
		template <typename T> using typed_vec_t = typename typed_vec<T>::type;
		/// For use in impl inheritance.
		template <typename T> struct typed_unit_vec {
			template <std::size_t Dim> using type = unit_vec<T, Dim>; ///< The corresponding \ref unit_vec type.
		};
		/// Shorthand for \ref typed_unit_vec::type.
		template <typename T> using typed_unit_vec_t = typename typed_unit_vec<T>::type;
	}

	/// Unit vectors.
	template <typename T, std::size_t Dim> struct unit_vec :
		public impls::unit_norm_op<unit_vec<T, Dim>>,
		public impls::swizzle_op<_details::typed_unit_vec_t<T>, Dim, _details::typed_vec_t<T>> {

		friend vec<T, Dim>;
		friend impls::unit_norm_op<unit_vec<T, Dim>>;
		friend impls::norm_op<vec<T, Dim>, unit_vec<T, Dim>>;
	public:
		/// No default constructor.
		unit_vec() = delete;

		/// Size.
		[[nodiscard]] constexpr static std::size_t size() {
			return Dim;
		}

		/// Only const indexing.
		[[nodiscard]] constexpr const T &operator[](std::size_t id) const {
			return _storage[id];
		}

		/// Negation.
		[[nodiscard]] constexpr unit_vec operator-() const;
	private:
		array<T, Dim> _storage; ///< Private storage.

		/// Private conversion constructor from vectors to unit vectors.
		constexpr explicit unit_vec(const vec<T, Dim>&);
	};

	namespace arithmetic_traits {
		/// \ref unit_vec + \ref unit_vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<unit_vec<T, Dim>, unit_vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref unit_vec - \ref unit_vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<unit_vec<T, Dim>, unit_vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref unit_vec * \ref unit_vec is explicitly disabled.
		template <typename T, std::size_t Dim> struct scalar_multiplication<unit_vec<T, Dim>, unit_vec<T, Dim>> {
			using result_type = void; ///< Disabled.
		};
		/// \ref unit_vec * \p scalar.
		template <typename T, std::size_t Dim, typename U> struct scalar_multiplication<unit_vec<T, Dim>, U> {
			using result_type = vec<T, Dim>; ///< Assumes that the multiplication does not change the value type.
			using scalar_side = right_hand_side; ///< Right hand side.
		};
		/// \p scalar * \ref unit_vec.
		template <typename T, std::size_t Dim, typename U> struct scalar_multiplication<U, unit_vec<T, Dim>> {
			constexpr static bool enabled = std::is_arithmetic_v<U>;

			/// Assumes that the multiplication does not change the value type.
			using result_type = std::conditional_t<enabled, vec<T, Dim>, void>;
			using scalar_side = left_hand_side; ///< Left hand side.
		};
		/// \ref unit_vec / \p scalar.
		template <typename T, std::size_t Dim, typename U> struct scalar_division<unit_vec<T, Dim>, U> {
			constexpr static bool enabled = std::is_arithmetic_v<U>;

			/// Assumes that the multiplication does not change the value type.
			using result_type = std::conditional_t<enabled, vec<T, Dim>, void>;
			using scalar_side = right_hand_side; ///< Right hand side.
		};

		/// \ref unit_vec == \ref unit_vec.
		template <typename T, std::size_t Dim> struct equality<unit_vec<T, Dim>, unit_vec<T, Dim>> {
			constexpr static bool enabled = std::is_integral_v<T>; ///< Only enable for integers.
		};
	}

	/// Vectors.
	template <typename T, std::size_t Dim> struct vec :
		public array<T, Dim>,
		public impls::dot_op<vec<T, Dim>>,
		public impls::norm_op<vec<T, Dim>, unit_vec<T, Dim>>,
		public impls::swizzle_op<typename _details::typed_vec<T>::type, Dim> {

		friend unit_vec<T, Dim>;
		friend point<T, Dim>;
	public:
		/// Default constructor.
		constexpr vec() = default;
		/// Implicit conversion from unit vectors to normal vectors.
		constexpr vec(const unit_vec<T, Dim> &unit) : array<T, Dim>(unit._storage) {
		}
		/// Initializes all elements of this vector.
		template <
			typename ...Args, typename = std::enable_if_t<sizeof...(Args) == Dim>
		> constexpr explicit vec(Args &&...args) : array<T, Dim>{ { std::forward<Args>(args)... } }
		{
		}

		/// Constructs a new vector from its elements.
		template <
			typename ...Args, typename = std::enable_if_t<sizeof...(Args) == Dim>
		> inline constexpr static vec from_elements(Args &&...args) {
			return vec(std::forward<Args>(args)...);
		}
	private:
		/// Initializes this vector using the underlying storage.
		constexpr vec(const array<T, Dim> &storage) : array<T, Dim>(storage) {
		}
	};

	namespace arithmetic_traits {
		/// \ref vec + \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<vec<T, Dim>, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref vec + \ref unit_vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<vec<T, Dim>, unit_vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref unit_vec + \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<unit_vec<T, Dim>, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref vec - \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<vec<T, Dim>, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref vec - \ref unit_vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<vec<T, Dim>, unit_vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref unit_vec - \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<unit_vec<T, Dim>, vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// -\ref vec.
		template <typename T, std::size_t Dim> struct negation<vec<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref vec * \ref vec is explicitly disabled.
		template <typename T, std::size_t Dim> struct scalar_multiplication<vec<T, Dim>, vec<T, Dim>> {
			using result_type = void; ///< Disabled.
		};
		/// \ref vec * \p scalar.
		template <typename T, std::size_t Dim, typename U> struct scalar_multiplication<vec<T, Dim>, U> {
			using result_type = vec<T, Dim>; ///< Assumes that the multiplication does not change the value type.
			using scalar_side = right_hand_side; ///< Right hand side.
		};
		/// \p scalar * \ref vec.
		template <typename T, std::size_t Dim, typename U> struct scalar_multiplication<U, vec<T, Dim>> {
			constexpr static bool enabled = std::is_arithmetic_v<U>;

			/// Assumes that the multiplication does not change the value type.
			using result_type = std::conditional_t<enabled, vec<T, Dim>, void>;
			using scalar_side = left_hand_side; ///< Left hand side.
		};
		/// \ref vec / \p scalar.
		template <typename T, std::size_t Dim, typename U> struct scalar_division<vec<T, Dim>, U> {
			constexpr static bool enabled = std::is_arithmetic_v<U>;

			/// Assumes that the multiplication does not change the value type.
			using result_type = std::conditional_t<enabled, vec<T, Dim>, void>;
			using scalar_side = right_hand_side; ///< Right hand side.
		};

		/// \ref vec == \ref vec.
		template <typename T, std::size_t Dim> struct equality<vec<T, Dim>, vec<T, Dim>> {
			constexpr static bool enabled = std::is_integral_v<T>; ///< Only enable for integers.
		};
		/// \ref vec == \ref unit_vec.
		template <typename T, std::size_t Dim> struct equality<vec<T, Dim>, unit_vec<T, Dim>> {
			constexpr static bool enabled = std::is_integral_v<T>; ///< Only enable for integers.
		};
		/// \ref unit_vec == \ref vec.
		template <typename T, std::size_t Dim> struct equality<unit_vec<T, Dim>, vec<T, Dim>> {
			constexpr static bool enabled = std::is_integral_v<T>; ///< Only enable for integers.
		};
	}

	namespace impls {
		/// Specialization of \ref array_traits for \ref unit_vec.
		template <typename T, std::size_t Dim> struct array_traits<unit_vec<T, Dim>> {
			using value_type = T; ///< Value type.
			constexpr static std::size_t dimension = Dim; ///< Dimension.
		};

		/// Specialization of \ref array_traits for \ref vec.
		template <typename T, std::size_t Dim> struct array_traits<vec<T, Dim>> {
			using value_type = T; ///< Value type.
			constexpr static std::size_t dimension = Dim; ///< Dimension.
		};
	}


	template <typename T> using vec2 = vec<T, 2>; ///< Shorthand for 2D vectors.
	using vec2f = vec2<float>; ///< Shorthand for 2D \p float vectors.
	using vec2d = vec2<double>; ///< Shorthand for 2D \p double vectors.
	using vec2i = vec2<int>; ///< Shorthand for 2D \p int vectors.
	using vec2s = vec2<std::size_t>; ///< Shorthand for 2D \p std::size_t vectors.

	template <typename T> using unit_vec2 = unit_vec<T, 2>; ///< Shorthand for 2D unit vectors.
	using unit_vec2f = unit_vec2<float>; ///< Shorthand for 2D unit \p float vectors.
	using unit_vec2d = unit_vec2<double>; ///< Shorthand for 2D unit \p double vectors.
	using unit_vec2i = unit_vec2<int>; ///< Shorthand for 2D unit \p int vectors.
	using unit_vec2s = unit_vec2<std::size_t>; ///< Shorthand for 2D unit \p std::size_t vectors.


	template <typename T> using vec3 = vec<T, 3>; ///< Shorthand for 3D vectors.
	using vec3f = vec3<float>; ///< Shorthand for 3D \p float vectors.
	using vec3d = vec3<double>; ///< Shorthand for 3D \p double vectors.
	using vec3i = vec3<int>; ///< Shorthand for 3D \p int vectors.
	using vec3s = vec3<std::size_t>; ///< Shorthand for 3D \p std::size_t vectors.

	template <typename T> using unit_vec3 = unit_vec<T, 3>; ///< Shorthand for 3D unit vectors.
	using unit_vec3f = unit_vec3<float>; ///< Shorthand for 3D unit \p float vectors.
	using unit_vec3d = unit_vec3<double>; ///< Shorthand for 3D unit \p double vectors.
	using unit_vec3i = unit_vec3<int>; ///< Shorthand for 3D unit \p int vectors.
	using unit_vec3s = unit_vec3<std::size_t>; ///< Shorthand for 3D unit \p std::size_t vectors.


	template <typename T> using vec4 = vec<T, 4>; ///< Shorthand for 4D vectors.
	using vec4f = vec4<float>; ///< Shorthand for 4D \p float vectors.
	using vec4d = vec4<double>; ///< Shorthand for 4D \p double vectors.
	using vec4i = vec4<int>; ///< Shorthand for 4D \p int vectors.
	using vec4s = vec4<std::size_t>; ///< Shorthand for 4D \p std::size_t vectors.

	template <typename T> using unit_vec4 = unit_vec<T, 4>; ///< Shorthand for 4D unit vectors.
	using unit_vec4f = unit_vec4<float>; ///< Shorthand for 4D unit \p float vectors.
	using unit_vec4d = unit_vec4<double>; ///< Shorthand for 4D unit \p double vectors.
	using unit_vec4i = unit_vec4<int>; ///< Shorthand for 4D unit \p int vectors.
	using unit_vec4s = unit_vec4<std::size_t>; ///< Shorthand for 4D unit \p std::size_t vectors.


	template <
		typename T, std::size_t Dim
	> constexpr unit_vec<T, Dim>::unit_vec(const vec<T, Dim> &src) : _storage(src) {
	}

	template <typename T, std::size_t Dim> [[nodiscard]] constexpr unit_vec<T, Dim> unit_vec<T, Dim>::operator-() const {
		return unit_vec(-vec<T, Dim>(_storage));
	}
}
