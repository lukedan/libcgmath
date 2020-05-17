#pragma once

/// \file
/// Points.

#include "array.h"
#include "arithmetic.h"
#include "vec.h"
#include "impls/swizzle.h"

namespace math {
	template <typename, std::size_t> struct point;
	namespace _details {
		/// For use in impl inheritance.
		template <typename T> struct typed_point {
			template <std::size_t Dim> using type = point<T, Dim>; ///< The corresponding \ref point type.
		};
		/// Shorthand for \ref typed_vec::type.
		template <typename T> using typed_point_t = typename typed_point<T>::type;
	}

	/// A point in space.
	template <typename T, std::size_t Dim> struct point :
		public array<T, Dim>,
		public impls::swizzle_op<_details::typed_point_t<T>, Dim> {

	public:
		/// Default constructor.
		constexpr point() = default;
		/// Initializes all elements of this point.
		template <
			typename ...Args, typename = std::enable_if_t<sizeof...(Args) == Dim>
		> constexpr point(Args &&...args) : array<T, Dim>{ { std::forward<Args>(args)... } }
		{
		}

		/// Constructs a new point from its elements.
		template <
			typename ...Args, typename = std::enable_if_t<sizeof...(Args) == Dim>
		> inline constexpr static point from_elements(Args &&...args) {
			return point(std::forward<Args>(args)...);
		}

		/// Converts this \ref point into a \ref vec.
		constexpr vec<T, Dim> as_vec() const {
			return vec<T, Dim>(*this);
		}
	};

	namespace arithmetic_traits {
		/// \ref point + \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<point<T, Dim>, vec<T, Dim>> {
			using result_type = point<T, Dim>; ///< Returns \ref point.
		};
		/// \ref point + \ref unit_vec.
		template <typename T, std::size_t Dim> struct memberwise_addition<point<T, Dim>, unit_vec<T, Dim>> {
			using result_type = point<T, Dim>; ///< Returns \ref point.
		};

		/// \ref point - \ref point.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<point<T, Dim>, point<T, Dim>> {
			using result_type = vec<T, Dim>; ///< Returns \ref vec.
		};
		/// \ref point - \ref vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<point<T, Dim>, vec<T, Dim>> {
			using result_type = point<T, Dim>; ///< Returns \ref point.
		};
		/// \ref point - \ref unit_vec.
		template <typename T, std::size_t Dim> struct memberwise_subtraction<point<T, Dim>, unit_vec<T, Dim>> {
			using result_type = point<T, Dim>; ///< Returns \ref point.
		};

		/// \ref point == \ref point.
		template <typename T, std::size_t Dim> struct equality<point<T, Dim>, point<T, Dim>> {
			constexpr static bool enabled = std::is_integral_v<T>; ///< Only enable for integers.
		};
	}

	template <typename T> using point2 = point<T, 2>; ///< Shorthand for 2D points.
	using point2f = point2<float>; ///< Shorthand for 2D \p float points.
	using point2d = point2<double>; ///< Shorthand for 2D \p double points.
	using point2i = point2<int>; ///< Shorthand for 2D \p int points.
	using point2s = point2<std::size_t>; ///< Shorthand for 2D \p std::size_t points.

	template <typename T> using point3 = point<T, 3>; ///< Shorthand for 3D points.
	using point3f = point3<float>; ///< Shorthand for 3D \p float points.
	using point3d = point3<double>; ///< Shorthand for 3D \p double points.
	using point3i = point3<int>; ///< Shorthand for 3D \p int points.
	using point3s = point3<std::size_t>; ///< Shorthand for 3D \p std::size_t points.

	template <typename T> using point4 = point<T, 4>; ///< Shorthand for 4D points.
	using point4f = point4<float>; ///< Shorthand for 4D \p float points.
	using point4d = point4<double>; ///< Shorthand for 4D \p double points.
	using point4i = point4<int>; ///< Shorthand for 4D \p int points.
	using point4s = point4<std::size_t>; ///< Shorthand for 4D \p std::size_t points.
}
