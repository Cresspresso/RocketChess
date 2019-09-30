/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/moo/craii.hpp
//
//	Summary		:	RAII wrapper for move semantics.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	28/09/2019
*/

#pragma once

#include <memory>

namespace cress
{
	namespace moo
	{
		/*
		//	Default traits argument for Craii.
		//	Can be replaced with user-defined traits.
		*/
		template<class T>
		struct DefaultCraiiTraits
		{
			static void destroy(T& data) noexcept
			{
				data.destroy();
			}

			static T exchangeWithDefault(T& data) noexcept
			{
				T temp = std::move(data);
				data = T();
				return temp;
			}
		};

		/*
		//	Craii
		//	RAII wrapper for move semantics.
		//	Does not have a default constructor.
		//
		//	Example:

			struct MyClassData
			{
				int index = -1;
				constexpr void destroy() noexcept { index = -1; }
			};

			class MyClass : private cress::Craii<MyClassData>
			{
			public:
				int getIndex() const noexcept { return m_data.index; }

				explicit MyClass(int index) noexcept
					: Craii<MyClassData>(MyClassData{ index })
				{}
			};

			MyClass obj = MyClass(4);

		*/
		template<class Value_, class Traits_ = DefaultCraiiTraits<Value_>>
		class Craii
		{
		public:
			using This = Craii;
			using Value = Value_;
			using Traits = Traits_;

			Value_ m_data;

			~Craii() noexcept
			{
				Traits_::destroy(m_data);
			}

			Craii(Craii&& that) noexcept : m_data(Traits_::exchangeWithDefault(that.m_data))
			{
			}

			Craii& operator=(Craii&& that) noexcept
			{
				Traits_::destroy(m_data);
				m_data = Traits_::exchangeWithDefault(that.m_data);
				return *this;
			}

			explicit Craii(Value_&& data) noexcept : m_data(Traits_::exchangeWithDefault(data))
			{
			}

			Craii() = delete;
			Craii(Craii const&) = delete;
			Craii& operator=(Craii const&) = delete;

			void swap(Craii& that) noexcept
			{
				using std::swap;
				swap(m_data, that.m_data);
			}

			friend void swap(Craii& a, Craii& b) noexcept
			{
				a.swap(b);
			}
		};
	}
}
