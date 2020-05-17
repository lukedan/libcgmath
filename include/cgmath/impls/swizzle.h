#pragma once

/// \file
/// Swizzling.

#include "common.h"

namespace math::impls {
	/// Swizzling. This requires \p operator[], \p from_elements(), and \p size().
	template <
		template <std::size_t> typename Derived, std::size_t DefaultDim,
		template <std::size_t> typename OutTy = Derived
	> struct swizzle_op : private this_ptr<Derived<DefaultDim>, swizzle_op<Derived, DefaultDim, OutTy>> {
	private:
		/// The base type used to retrieve the \p this pointer.
		using _this = this_ptr<Derived<DefaultDim>, swizzle_op<Derived, DefaultDim, OutTy>>;
		friend _this;
	public:
		/// Swizzle implementation.
		template <std::size_t ...Is> constexpr OutTy<sizeof...(Is)> swizzle() const {
			static_assert(((Is < DefaultDim) && ...), "Swizzle indices start at 0");
			return OutTy<sizeof...(Is)>::from_elements(_this::get()[Is]...);
		}
	};
}
