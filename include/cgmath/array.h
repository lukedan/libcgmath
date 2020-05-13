#pragma once

/// \file
/// N-dimensional arrays.

#include <cstddef>
#include <utility>

namespace math {
	/// N-dimensional arrays.
	template <typename T, std::size_t FirstSize, std::size_t ...Sizes> struct array {
	private:
		/// Used to correctly obtain \ref element_type without creating invalid specializations.
		template <bool UseSubarrays> struct _element_type {
			using type = array<T, Sizes...>; ///< Element type.
		};
		/// Specialization for 1D arrays.
		template <> struct _element_type<false> {
			using type = T; ///< Element type.
		};
	public:
		using value_type = T; ///< Value type.
		using dimensions = std::index_sequence<FirstSize, Sizes...>; ///< The dimensions of this array.
		/// The type of elements of this `layer' of the array.
		using element_type = typename _element_type<sizeof...(Sizes) != 0>::type;

		/// Returns the size of this array. For multidimensional arrays, this is the size of the first dimension.
		constexpr static std::size_t size() {
			return FirstSize;
		}

	private:
		/// End of recursion.
		template <std::size_t...> void _set() {
		}
		/// Sets all elements of this array.
		template <
			std::size_t FirstId, std::size_t ...OtherIds, typename FirstArg, typename ...OtherArgs
		> void _set(FirstArg &&first, OtherArgs &&...others) {
			storage[FirstId] = std::forward<FirstArg>(first);
			_set<OtherIds...>(std::forward<OtherArgs>(others)...);
		}
		/// Wrapper around \ref _set().
		template <std::size_t ...Ids, typename ...Args> void _set_wrapper(
			std::index_sequence<Ids...>, Args &&...args
		) {
			static_assert(sizeof...(Args) == FirstSize, "Array size mismatch");
			_set<Ids...>(std::forward<Args>(args)...);
		}
	public:
		/// Constructs a new \ref array from its elements. There must be the same number of elements as the length of
		/// the array.
		template <typename ...Args> inline static array from_elements(Args &&...args) {
			array result;
			result._set_wrapper(std::make_index_sequence<FirstSize>{}, std::forward<Args>(args)...);
			return result;
		}

		/// Indexing.
		element_type &operator[](std::size_t i) {
			return storage[i];
		}
		/// \overload
		const element_type &operator[](std::size_t i) const {
			return storage[i];
		}

		/// Storage, value-initialized (basic scalar types will be zero-initialized).
		element_type storage[FirstSize]{};
	};
}
