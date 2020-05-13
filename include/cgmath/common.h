#pragma once

/// \file
/// Common infrastructure of the library.

#include <cstddef>
#include <utility>
#include <type_traits>

namespace math {
	/// Wrapper around a boolean that indicates whether to break a loop.
	struct break_loop {
		/// Explicit constructor.
		explicit break_loop(bool bk) : do_break(bk) {
		}

		bool do_break = false; ///< Whether to break.
	};

	/// Operations for statically-sized array-like objects.
	namespace arr {
		namespace _details {
			/// End of recursion.
			template <typename Callback, typename ...Args> void _for_each_impl(Callback&&, Args&&...) {
			}
			/// Implementation of \ref for_each().
			template <
				std::size_t FirstId, std::size_t ...OtherIds, typename Callback, typename ...Args
			> void _for_each_impl(Callback &&cb, Args &&...args) {
				using return_type = std::invoke_result_t<Callback&&, Args&&...>;
				if constexpr (std::is_same_v<return_type, break_loop>) {
					if (std::forward<Callback>(cb)(args[FirstId]...).do_break) {
						return;
					}
				} else {
					std::forward<Callback>(cb)(args[FirstId]...);
				}
				_for_each_impl<OtherIds...>(std::forward<Callback>(cb), std::forward<Args>(args)...);
			}

			/// Wrapper around \ref _for_each_impl().
			template <typename Callback, typename ...Args, std::size_t ...Indices> void _for_each_impl_wrapper(
				std::index_sequence<Indices...>, Callback &&cb, Args &&...args
			) {
				_for_each_impl<Indices...>(std::forward<Callback>(cb), std::forward<Args>(args)...);
			}
		}
		/// Calls the callback function using each element in each input array.
		template <typename Callback, typename FirstArg, typename ...OtherArgs> void for_each(
			Callback &&cb, FirstArg &&first, OtherArgs &&...others
		) {
			static_assert(
				((std::decay_t<FirstArg>::size() == others.size()) && ...),
				"All arrays must have the same size."
			);
			_details::_for_each_impl_wrapper(
				std::make_index_sequence<first.size()>{},
				std::forward<Callback>(cb), std::forward<FirstArg>(first), std::forward<OtherArgs>(others)...
			);
		}
	}
}
