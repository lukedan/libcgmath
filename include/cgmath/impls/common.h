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
		> Mod<Derived> *_cast_sequence(Mod<Current> *ptr) const {
			return static_cast<Mod<Derived>*>(ptr);
		}
		/// Performs \p static_cast along a sequence of types.
		template <
			template <typename> typename Mod, typename Current,
			typename Mid, typename ...Others
		> Mod<Derived> *_cast_sequence(Mod<Current> *ptr) const {
			static_assert(std::is_base_of_v<Current, Mid>, "Incorrect base type");
			return _cast_sequence<Mod, Mid, Others...>(static_cast<Mod<Mid>*>(ptr));
		}
	protected:
		/// Non-const \p this.
		Derived *_this() {
			return _cast_sequence<_identity, this_ptr, CastSeq...>(this);
		}
		/// Const \p this.
		const Derived *_this() const {
			return _cast_sequence<std::add_const_t, this_ptr, CastSeq...>(this);
		}
	};
}
