/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/moo/result.hpp
//
//	Summary		:	Either an expected value or an error value.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	29/09/2019
*/

#pragma once

#include <type_traits>
#include <stdexcept>



/*
//	This macro can be defined before including this header, to provide custom assert behaviour.
//	This is called in the event of an invalid dereference.
//	This must be considered noexcept.
*/
#ifndef CRESS_MOO_RESULT_ASSERT
#include <cassert>
#include <iostream>
#define CRESS_MOO_RESULT_ASSERT(condition)\
	do {
		bool const b = static_cast<bool>(condition);\
		assert(b);\
		if (!b) { std::cerr << "CRESS_MOO_RESULT_ASSERT assertion failed.\n"; }\
	} while(0)
#endif



namespace cress
{
	namespace moo
	{
		namespace ResultTags
		{
			struct EmplaceError {};
			struct EmplaceValue {};
		}

		/*
		//	Thrown when a Result is unwrapped while in an error state.
		*/
		struct BadResultException : std::exception
		{
			char const* what() const override
			{
				return "cress::moo::BadResultException";
			}
		};



		/*
		//	Result
		//
		//	Either an expected value or an error value.
		//
		//	Example:

			cress::moo::Result<int, std::string> tryDivide(int n, int d)
			{
				if (0 == d) { return "divide by zero error"; }
				return n / d;
			}

			int main()
			{
				auto result = tryDivide(8, 4);
				assert(result.hasValue());
				assert(result.value() == 2);

				result = tryDivide(3, 0);
				assert(!result.hasValue());
				assert(result.error() == std::string("divide by zero error"));

				int x = tryDivide(4, 3).coalesce([]{ return 13; });
				assert(x == 1);

				x = tryDivide(4, 0).coalesce([]{ return 13; });
				assert(x == 13);
			}

		*/
		template<class Value_, class Error_>
		class [[nodiscard]] Result
		{
		public:
			using This = Result;
			using Value = Value_;
			using Error = Error_;
		private:
			bool m_hasValue;
			
			struct UninitalisedStorage {};

			union Storage {
				UninitalisedStorage u_uninitialised;
				Value_ u_value;
				Error_ u_error;

				~Storage() noexcept {}

#pragma warning(push)
#pragma warning(disable: 26495)
				explicit Storage(UninitalisedStorage) noexcept : u_uninitialised() {}
#pragma warning(pop)

				template<class...X>
				explicit Storage(ResultTags::EmplaceValue, X&& ...a)
					noexcept(std::is_nothrow_constructible_v<Value_, X&&...>)
					: u_value(std::forward<X>(a)...) {}

				template<class...X>
				explicit Storage(ResultTags::EmplaceError, X&& ...a)
					noexcept(std::is_nothrow_constructible_v<Error_, X&&...>)
					: u_error(std::forward<X>(a)...) {}

			} m_storage;



			template<class E>
			using EnableOverloadError_t = std::enable_if_t<std::is_convertible_v<E, Error_> && !std::is_same_v<Error_, Value_>>;

		public:
			~Result() noexcept
			{
				if (m_hasValue)
				{
					m_storage.u_value.Value_::~Value_();
				}
				else
				{
					m_storage.u_error.Error_::~Error_();
				}
			}

			Result(Result&& that) noexcept : m_hasValue(that.m_hasValue), m_storage(UninitalisedStorage{})
			{
				if (that.m_hasValue)
				{
					new (&m_storage.u_value) Value_(std::move(that.m_storage.u_value));
				}
				else
				{
					new (&m_storage.u_error) Error_(std::move(that.m_storage.u_error));
				}
			}

			Result& operator=(Result&& that) noexcept
			{
				if (m_hasValue)
				{
					if (that.m_hasValue)
					{
						m_storage.u_value = std::move(that.m_storage.u_value);
					}
					else
					{
						m_storage.u_value.Value_::~Value_();
						new (&m_storage.u_error) Error_(std::move(that.m_storage.u_error));
					}
				}
				else
				{
					if (that.m_hasValue)
					{
						m_storage.u_error.Error_::~Error_();
						new (&m_storage.u_value) Value_(std::move(that.m_storage.u_value));
					}
					else
					{
						m_storage.u_error = std::move(that.m_storage.u_error);
					}
				}
				m_hasValue = that.m_hasValue;
				return *this;
			}

			void swap(Result& that) noexcept
			{
				using std::swap;
				if (m_hasValue)
				{
					if (that.m_hasValue)
					{
						swap(m_storage.u_value, that.m_storage.u_value);
					}
					else
					{
						Value_ temp = std::move(m_storage.u_value);
						m_storage.u_value.Value_::~Value_();
						new (&m_storage.u_error) Error_(std::move(that.m_storage.u_error));
						that.m_storage.u_error.Error_::~Error_();
						new (&that.m_storage.u_value) Value_(std::move(temp));
					}
				}
				else
				{
					if (that.m_hasValue)
					{
						Value_ temp = std::move(that.m_storage.u_value);
						that.m_storage.u_value.Value_::~Value_();
						new (&that.m_storage.u_error) Error_(std::move(m_storage.u_error));
						m_storage.u_error.Error_::~Error_();
						new (&m_storage.u_value) Value_(std::move(temp));
					}
					else
					{
						swap(m_storage.u_error, that.m_storage.u_error);
					}
				}
				swap(m_hasValue, that.m_hasValue);
			}

			friend void swap(Result& a, Result& b) noexcept { a.swap(b); }

			Result(Result const& that)
				noexcept(std::is_nothrow_copy_constructible_v<Value_>
					&& std::is_nothrow_copy_constructible_v<Error_>)
				: m_hasValue(that.m_hasValue), m_storage(UninitalisedStorage{})
			{
				if (that.m_hasValue)
				{
					new (&m_storage.u_value) Value_(that.m_storage.u_value);
				}
				else
				{
					new (&m_storage.u_error) Error_(that.m_storage.u_error);
				}
			}

			Result& operator=(Result const& that)
				noexcept(std::is_nothrow_copy_constructible_v<Value_>
					&& std::is_nothrow_copy_assignable_v<Value_>
					&& std::is_nothrow_copy_constructible_v<Error_>
					&& std::is_nothrow_copy_assignable_v<Error_>)
			{
				if (m_hasValue)
				{
					if (that.m_hasValue)
					{
						m_storage.u_value = that.m_storage.u_value;
					}
					else
					{
						m_storage.u_value.Value_::~Value_();
						new (&m_storage.u_error) Error_(that.m_storage.u_error);
					}
				}
				else
				{
					if (that.m_hasValue)
					{
						m_storage.u_error.Error_::~Error_();
						new (&m_storage.u_value) Value_(that.m_storage.u_value);
					}
					else
					{
						m_storage.u_error = that.m_storage.u_error;
					}
				}
				m_hasValue = that.m_hasValue;
				return *this;
			}

			Result() noexcept(std::is_nothrow_default_constructible_v<Error_>)
				: m_hasValue(false), m_storage(ResultTags::EmplaceError{}) {}

			Result(Value_ const& value) noexcept(std::is_nothrow_copy_constructible_v<Value_>)
				: m_hasValue(true), m_storage(ResultTags::EmplaceValue{}, value) {}

			Result(Value_&& value) noexcept
				: m_hasValue(true), m_storage(ResultTags::EmplaceValue{}, std::move(value)) {}

			template<class E, class = EnableOverloadError_t<E>>
			Result(E const& error) noexcept(std::is_nothrow_copy_constructible_v<Error_>)
				: m_hasValue(false), m_storage(ResultTags::EmplaceError{}, error) {}

			template<class E, class = EnableOverloadError_t<E>>
			Result(E&& error) noexcept
				: m_hasValue(false), m_storage(ResultTags::EmplaceError{}, std::move(error)) {}

			template<class...X>
			explicit Result(ResultTags::EmplaceValue em, X&& ...a)
				noexcept(std::is_nothrow_constructible_v<Value_, X&&...>)
				: m_hasValue(true), m_storage(em, std::forward<X>(a)...) {}

			template<class...X>
			explicit Result(ResultTags::EmplaceError em, X&& ...a)
				noexcept(std::is_nothrow_constructible_v<Error_, X&&...>)
				: m_hasValue(false), m_storage(em, std::forward<X>(a)...) {}

			template<class...X>
			Value_& emplaceValue(X&& ...a)
				noexcept(std::is_nothrow_constructible_v<Value_, X&&...>)
			{
				if (m_hasValue)
				{
					m_storage.u_value = Value_(std::forward<X>(a)...);
				}
				else
				{
					m_storage.u_error.Error_::~Error_();
					new (&m_storage.u_value) Value_(std::forward<X>(a)...);
					m_hasValue = true;
				}
				return m_storage.u_value;
			}

			template<class...X>
			Error_& emplaceError(X&& ...a)
				noexcept(std::is_nothrow_constructible_v<Error_, X&& ...>)
			{
				if (m_hasValue)
				{
					m_storage.u_value.Value_::~Value_();
					new (&m_storage.u_error) Error_(std::forward<X>(a)...);
					m_hasValue = false;
				}
				else
				{
					m_storage.u_error = Error_(std::forward<X>(a)...);
				}
				return m_storage.u_error;
			}

			Result& operator=(Value_&& value) noexcept
			{
				emplaceValue(std::move(value));
				return *this;
			}

			Result& operator=(Value_ const& value)
				noexcept(std::is_nothrow_copy_constructible_v<Value_>
					&& std::is_nothrow_copy_assignable_v<Value_>)
			{
				emplaceValue(value);
				return *this;
			}

			template<class E, class = EnableOverloadError_t<E>>
			Result& operator=(E&& error) noexcept
			{
				emplaceError(std::move(error));
				return *this;
			}

			template<class E, class = EnableOverloadError_t<E>>
			Result& operator=(E const& error)
				noexcept(std::is_nothrow_copy_constructible_v<Error_>
					&& std::is_nothrow_copy_assignable_v<Error_>)
			{
				emplaceError(error);
				return *this;
			}



			[[nodiscard]]
			bool hasValue() const noexcept { return m_hasValue; }

			[[nodiscard]]
			Value_& value() noexcept
			{
				CRESS_MOO_RESULT_ASSERT(m_hasValue);
				return (m_storage.u_value);
			}

			[[nodiscard]]
			Value_ const& value() const noexcept
			{
				CRESS_MOO_RESULT_ASSERT(m_hasValue);
				return (m_storage.u_value);
			}

			[[nodiscard]]
			Error_& error() noexcept
			{
				CRESS_MOO_RESULT_ASSERT(!m_hasValue);
				return (m_storage.u_error);
			}

			[[nodiscard]]
			Error_ const& error() const noexcept
			{
				CRESS_MOO_RESULT_ASSERT(!m_hasValue);
				return (m_storage.u_error);
			}



			[[nodiscard]]
			explicit operator bool() const noexcept { return m_hasValue; }

			[[nodiscard]]
			Value_& operator*() noexcept { return value(); }
			[[nodiscard]]
			Value_ const& operator*() const noexcept { return value(); }
			[[nodiscard]]
			Value_* operator->() noexcept { return &value(); }
			[[nodiscard]]
			Value_ const* operator->() const noexcept { return &value(); }



			/*
			//	Converts  Result<Value,E>  ->  Value.
			*/
			template<class F>
			[[nodiscard]]
			Value_ coalesce(F&& alternativeCreator) const&
				noexcept(std::is_nothrow_copy_constructible_v<Value_>
					&& std::is_nothrow_invocable_v<F&&>
					&& std::is_nothrow_constructible_v<Value_, std::invoke_result_t<F&&>>)
			{
				return m_hasValue ? m_storage.u_value : std::forward<F>(alternativeCreator)();
			}

			/*
			//	Converts  Result<Value,E>  ->  Value.
			*/
			template<class F>
			[[nodiscard]]
			Value_ coalesce(F&& alternativeCreator) &&
				noexcept(std::is_nothrow_invocable_v<F&&>
					&& std::is_nothrow_constructible_v<Value_, std::invoke_result_t<F&&>>)
			{
				return m_hasValue ? std::move(m_storage.u_value) : std::forward<F>(alternativeCreator)();
			}



			/*
			//	Converts  Result<T,E>  ->  Result<U,E>.
			*/
			template<class F, class R = std::invoke_result_t<F&&, Value_ const&>>
			[[nodiscard]]
			Result<R, Error_> map(F&& selector) const&
				noexcept(std::is_nothrow_invocable_v<F&&, Value_ const&>
					&& std::is_nothrow_copy_constructible_v<Error_>)
			{
				return m_hasValue
					? Result<R, Error_>(ResultTags::EmplaceValue{}, std::forward<F>(selector)(m_storage.u_value))
					: Result<R, Error_>(ResultTags::EmplaceError{}, m_storage.u_error);
			}

			/*
			//	Converts  Result<T,E>  ->  Result<U,E>.
			*/
			template<class F, class R = std::invoke_result_t<F&&, Value_&&>>
			[[nodiscard]]
			Result<R, Error_> map(F&& selector)&&
				noexcept(std::is_nothrow_invocable_v<F&&, Value_&&>)
			{
				return m_hasValue
					? Result<R, Error_>(ResultTags::EmplaceValue{}, std::forward<F>(selector)(std::move(m_storage.u_value)))
					: Result<R, Error_>(ResultTags::EmplaceError{}, std::move(m_storage.u_error));
			}



			/*
			//	Converts  Result<Result<V,E>,E>  ->  Result<V,E>.
			*/
			template<class V = std::enable_if_t<std::is_constructible_v<Value_, Error_ const&>, Value_>>
			[[nodiscard]]
			V flat() const &
				noexcept(std::is_nothrow_copy_constructible_v<Value_>
					&& std::is_nothrow_constructible_v<Value_, Error_ const&>)
			{
				if (!m_hasValue) { return Value_(ResultTags::EmplaceError{}, m_storage.u_error); }
				return m_storage.u_value;
			}

			/*
			//	Converts  Result<Result<V,E>,E>  ->  Result<V,E>.
			*/
			template<class V = std::enable_if_t<std::is_constructible_v<Value_, Error_&&>, Value_>>
			[[nodiscard]]
			V flat() &&
				noexcept(std::is_nothrow_constructible_v<Value_, Error_&&>)
			{
				if (!m_hasValue) { return Value_(ResultTags::EmplaceError{}, std::move(m_storage.u_error)); }
				return std::move(m_storage.u_value);
			}



			/*
			//	Throws an exception if this result is in an error state.
			*/
			void throwIfError() const& noexcept(false)
			{
				if (!m_hasValue) { throw BadResultException(); }
			}



			/*
			//	Returns value or throws.
			*/
			[[nodiscard]]
			Value_& unwrap() & noexcept(false)
			{
				throwIfError();
				return m_storage.u_value;
			}

			/*
			//	Returns value or throws.
			*/
			[[nodiscard]]
			Value_ const& unwrap() const& noexcept(false)
			{
				throwIfError();
				return m_storage.u_value;
			}

			/*
			//	Returns value or throws.
			*/
			[[nodiscard]]
			Value_ const& unwrap() const&& noexcept(false)
			{
				throwIfError();
				return m_storage.u_value;
			}
			
			/*
			//	Returns value or throws.
			*/
			[[nodiscard]]
			Value_ unwrap()&& noexcept(false)
			{
				throwIfError();
				return std::move(m_storage.u_value);
			}
		};



		template<class Error_, class Value_>
		[[nodiscard]]
		Result<Value_, Error_> ResultFromValue(Value_&& value) noexcept
		{
			return Result<Value_, Error_>(ResultTags::EmplaceValue{}, std::move(value));
		}

		template<class Error_, class Value_>
		[[nodiscard]]
		Result<Value_, Error_> ResultFromValue(Value_ const& value)
			noexcept(std::is_nothrow_copy_constructible_v<Value_>)
		{
			return Result<Value_, Error_>(ResultTags::EmplaceValue{}, value);
		}

		template<class Value_, class Error_>
		[[nodiscard]]
		Result<Value_, Error_> ResultFromError(Error_&& error) noexcept
		{
			return Result<Value_, Error_>(ResultTags::EmplaceError{}, std::move(error));
		}

		template<class Value_, class Error_>
		[[nodiscard]]
		Result<Value_, Error_> ResultFromError(Error_ const& error)
			noexcept(std::is_nothrow_copy_constructible_v<Error_>)
		{
			return Result<Value_, Error_>(ResultTags::EmplaceError{}, error);
		}



		/*
		//	void-like type to signify success.
		*/
		struct Ok {};
		constexpr Ok const ok{};

		template<class Error_>
		using OkResult = Result<Ok, Error_>;



		namespace ResultOps
		{
			template<class Value_, class Error_, class F>
			[[nodiscard]]
			Value_ operator|(Result<Value_, Error_> const& result, F&& alternativeCreator)
				noexcept(noexcept(std::declval<Result<Value_, Error_> const&>().coalesce(std::declval<F&&>())))
			{
				return result.coalesce(std::forward<F>(alternativeCreator));
			}

			template<class Value_, class Error_, class F>
			[[nodiscard]]
			Value_ operator|(Result<Value_, Error_>&& result, F&& alternativeCreator)
				noexcept(noexcept(std::declval<Result<Value_, Error_>&&>().coalesce(std::declval<F&&>())))
			{
				return std::move(result).coalesce(std::forward<F>(alternativeCreator));
			}

			template<class Value_, class Error_, class F>
			[[nodiscard]]
			auto operator&(Result<Value_, Error_> const& result, F&& selector)
				noexcept(noexcept(std::declval<Result<Value_, Error_> const&>().map(std::declval<F&&>())))
			{
				return result.map(std::forward<F>(selector));
			}

			template<class Value_, class Error_, class F>
			[[nodiscard]]
			auto operator&(Result<Value_, Error_>&& result, F&& selector)
				noexcept(noexcept(std::declval<Result<Value_, Error_>&&>().map(std::declval<F&&>())))
			{
				return std::move(result).map(std::forward<F>(selector));
			}
		}
	}
}
