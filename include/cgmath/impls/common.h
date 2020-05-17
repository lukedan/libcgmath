#pragma once

/// \file
/// Common utilities.

#include <type_traits>

namespace math::impls {
	/// Used to \p static_cast this object to a derived type.
	template <typename Derived, typename ...CastSeq> struct this_ptr {
	private:
		// TODO std::type_identity in C++20.
		template <typename T> using _identity = T; ///< Identity type.

		/// End of recursion.
		template <
			template <typename> typename Mod, typename Current
		> constexpr Mod<Derived> &_cast_sequence(Mod<Current> &ptr) const {
			return static_cast<Mod<Derived>&>(ptr);
		}
		/// Performs \p static_cast along a sequence of types.
		template <
			template <typename> typename Mod, typename Current,
			typename Mid, typename ...Others
		> constexpr Mod<Derived> &_cast_sequence(Mod<Current> &ptr) const {
			static_assert(std::is_base_of_v<Current, Mid>, "Incorrect base type");
			return _cast_sequence<Mod, Mid, Others...>(static_cast<Mod<Mid>&>(ptr));
		}
	public:
		/// Non-const \p this.
		constexpr Derived &get() {
			return _cast_sequence<_identity, this_ptr, CastSeq...>(*this);
		}
		/// Const \p this.
		constexpr const Derived &get() const {
			return _cast_sequence<std::add_const_t, this_ptr, CastSeq...>(*this);
		}
	};

	/// Used to retrieve \p T::value_type.
	template <typename T> struct array_traits;
	/// Shorthand to \ref array_traits::dimension.
	template <typename T> constexpr inline std::size_t array_dimension_t = array_traits<T>::dimension;
	/// Shorthand for \ref get_value_type::type.
	template <typename T> using array_value_type_t = typename array_traits<T>::value_type;
}
