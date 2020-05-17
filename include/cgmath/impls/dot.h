#pragma once

/// \file
/// Implementation of dot products.

#include "../common.h"
#include "common.h"

namespace math::impls {
	/// Dot product.
	template <typename Derived> struct dot_op {
	private:
		using _value_type = array_value_type_t<Derived>; ///< The value type.
	public:
		/// Dot product.
		[[nodiscard]] constexpr inline static _value_type dot(const Derived &lhs, const Derived &rhs) {
			_value_type result{};
			arr::for_each(
				[&result](const _value_type &l, const _value_type &r) {
					result += l * r;
				},
				lhs, rhs
					);
			return result;
		}
	};
}
