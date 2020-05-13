#pragma once

/// \file
/// Norm and normalization.

#include "common.h"

namespace math::impls {
	/// Norm. This requires \p dot().
	template <typename Derived> struct norm_op : private this_ptr<Derived, norm_op<Derived>> {
		/// Squared norm.
		decltype(auto) squared_norm() const {
			return Derived::dot(*_this(), *_this());
		}
	};
}
