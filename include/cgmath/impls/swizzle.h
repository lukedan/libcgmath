#pragma once

/// \file
/// Swizzling.

#include "common.h"

namespace math::impls {
	/// Swizzling. This requires \p operator[], \p from_elements(), and \p size().
	template <
		template <std::size_t> typename Derived, std::size_t DefaultDim
	> struct swizzle_op : private this_ptr<Derived<DefaultDim>, swizzle_op<Derived, DefaultDim>> {
		/// Swizzle implementation.
		template <std::size_t ...Is> Derived<sizeof...(Is)> swizzle() const {
			static_assert(((Is < Derived<DefaultDim>::size()) && ...), "Swizzle indices start at 0");
			return Derived<sizeof...(Is)>::from_elements((*_this())[Is]...);
		}
	};
}
