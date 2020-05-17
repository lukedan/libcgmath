#pragma once

/// \file
/// N-dimensional arrays.

#include <cstddef>
#include <utility>

namespace math {
	/// N-dimensional arrays.
	template <typename T, std::size_t FirstSize, std::size_t ...Sizes> struct array {
	private:
		/// Used to correctly obtain \ref value_type without creating invalid specializations.
		template <bool UseSubarrays> struct _value_type {
			using type = array<T, Sizes...>; ///< Element type.
		};
		/// Specialization for 1D arrays.
		template <> struct _value_type<false> {
			using type = T; ///< Element type.
		};
	public:
		using element_type = T; ///< The type of elements stored in this array.
		using dimensions = std::index_sequence<FirstSize, Sizes...>; ///< The dimensions of this array.
		/// The type of elements of this `layer' of the array.
		using value_type = typename _value_type<sizeof...(Sizes) != 0>::type;

		/// Returns the size of this array. For multidimensional arrays, this is the size of the first dimension.
		[[nodiscard]] constexpr static std::size_t size() {
			return FirstSize;
		}

		/// Indexing.
		[[nodiscard]] constexpr value_type &operator[](std::size_t i) {
			return storage[i];
		}
		/// \overload
		[[nodiscard]] constexpr const value_type &operator[](std::size_t i) const {
			return storage[i];
		}

		/// Storage, value-initialized (basic scalar types will be zero-initialized).
		value_type storage[FirstSize]{};
	};
}
