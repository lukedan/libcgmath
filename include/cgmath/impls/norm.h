#pragma once

/// \file
/// Norm and normalization.

#include "common.h"

namespace math::impls {
	/// Stores the result of the \p normalize() functions.
	template <typename Res, typename Norm> struct normalization_result {
		/// Initializes all fields of this struct.
		constexpr normalization_result(Res res, Norm sn, Norm n) :
			result(std::move(res)), squared_norm(std::move(sn)), norm(std::move(n)) {
		}

		Res result; ///< The resulting object.
		Norm
			squared_norm{}, ///< Squared norm of the original object.
			norm{}; ///< Norm of the original object.
	};

	/// Norm. This requires \p dot().
	template <typename Derived, typename Unit> struct norm_op : private this_ptr<Derived, norm_op<Derived, Unit>> {
	private:
		using _value_type = array_value_type_t<Derived>; ///< Value type.
		using _this = this_ptr<Derived, norm_op<Derived, Unit>>; ///< The base type used to retrieve the \p this pointer.
		friend _this;
	public:
		/// Squared norm.
		[[nodiscard]] constexpr decltype(auto) squared_norm() const {
			return Derived::dot(_this::get(), _this::get());
			/*return Derived::dot(static_cast<const Derived&>(*this), static_cast<const Derived&>(*this));*/
		}
		/// Norm.
		template <
			typename T = std::conditional_t<std::is_floating_point_v<_value_type>, _value_type, void>
		> [[nodiscard]] std::enable_if_t<!std::is_same_v<T, void>, T> norm() const {
			return std::sqrt(static_cast<T>(squared_norm()));
		}

	private:
		/// Shorthand to \p std::enable_if_t for non-template methods.
		template <bool Value, typename Res, typename Dummy> using _enable_if_t =
			std::enable_if_t<std::is_same_v<Dummy, void> && Value, Res>;
		/// Shorthand to \p std::enable_if_t if the vector contains floating-point values.
		template <typename Res, typename Dummy> using _enable_if_floating_point_t =
			_enable_if_t<std::is_floating_point_v<_value_type>, Res, Dummy>;
	public:
		/// Normalizes this vector without checking its length.
		template <typename Dummy = void> [[nodiscard]] constexpr _enable_if_floating_point_t<
			normalization_result<Unit, _value_type>, Dummy
		> normalized_nocheck() const {
			_value_type sn = squared_norm();
			_value_type n = std::sqrt(sn);
			return normalization_result<Unit, _value_type>(Unit(_this::get() / n), sn, n);
		}
	};

	/// Norm for unit vectors.
	template <typename Derived> struct unit_norm_op : private this_ptr<Derived, unit_norm_op<Derived>> {
	private:
		using _value_type = array_value_type_t<Derived>; ///< Value type.
		/// The base type used to retrieve the \p this pointer.
		using _this = this_ptr<Derived, unit_norm_op<Derived>>;
		friend _this;

		constexpr static _value_type _one = static_cast<_value_type>(1); ///< One.
	public:
		/// Squared norm is 1.
		[[nodiscard]] _value_type constexpr squared_norm() const {
			return _one;
		}
		/// Norm is 1.
		template <typename T = _value_type> [[nodiscard]] constexpr T norm() const {
			return static_cast<T>(1);
		}

		/// Returns this vector itself.
		normalization_result<Derived, _value_type> normalized_nocheck() const {
			return normalization_result<Derived, _value_type>(*_this::get(), _one, _one);
		}
	};
}
