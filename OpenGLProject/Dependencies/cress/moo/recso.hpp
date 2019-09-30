/*
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2019 Media Design School
//
//	File		:	cress/moo/recso.hpp
//
//	Summary		:	Reference Counted Static Object.
//
//	Project		:	moo
//	Namespace	:	cress::moo
//	Author		:	Elijah Shadbolt
//	Email		:	elijah.sha7979@mediadesign.school.nz
//	Date Edited	:	28/09/2019
*/

#pragma once

#include <memory>
#include <mutex>
#include <atomic>



/*
//	This macro can be defined before including this header, to provide custom assert behaviour.
//	This is called in the event of an unrecoverable error: the reference count is about to overflow max or underflow negative.
*/
#ifndef CRESS_MOO_RECSO_ASSERT
#include <cassert>
#include <iostream>
#define CRESS_MOO_RECSO_ASSERT(condition)\
	do {\
		bool const b = static_cast<bool>(condition);\
		assert(b);\
		if (!b) { std::cerr << "CRESS_MOO_RECSO_ASSERT assertion failed.\n"; }\
	} while(0)
#endif



namespace cress
{
	namespace moo
	{
		/*
		//	Recso
		//	Reference Counted Static Object.
		//
		//	This pointer-like object shares ownership of statically allocated storage with all other instances of this concrete type.
		//	The shared value always exists (is never null) if at least one instance of the Resco class exists.
		//	The type parameter |Traits| is the tag that determines which shared variable is used by the instance.
		//	Recso is thread safe for reference counting.
		//	The dereferenced value is not inherently thread safe.
		//
		//	The |Traits| type parameter must have the following members:
		//
		//		type Value
		//			Type of the value that comes from dereferencing the shared storage.
		//
		//		type Storage
		//			Type of the static shared variable.
		//			For example, |Value*|.
		//
		//		type Pointer
		//			For example, |Value*|.
		//
		//		type ConstPointer
		//			For example, |Value const*|.
		//
		//		type Reference
		//			For example, |Value&|.
		//
		//		type ConstReference
		//			For example, |Value const&|.
		//
		//		static Pointer address(Storage& storage);
		//			For example, if Storage is Value*, returns |storage|.
		//
		//		static ConstPointer addressConst(Storage const& storage);
		//			For example, if Storage is Value*, returns |storage|.
		//
		//		static Reference deref(Storage& storage);
		//			For example, if Storage is Value*, returns |*storage|.
		//
		//		static ConstReference derefConst(Storage const& storage);
		//			For example, if Storage is Value*, returns |*storage|.
		//
		//		static void destroy(Storage const& storage) noexcept;
		//			This is called when the last reference is destructed.
		//			For example, if Storage is Value*, |delete storage|.
		//
		//		static void create(Storage& storage, Args...);
		//			This is called when the first reference is constructed.
		//			It can have any number of Args after the storage.
		//			The args are forwarded from the constructor call of the Recso that is the first reference.
		//			The Recso constructor overload set is a mirror of the Traits::create overload set.
		//			For example, if Storage is Value*, |storage = new Value(args...)|.
		*/
		template<class Traits_>
		class Recso
		{
		public:
			using This = Recso;
			using Traits = Traits_;

			using Storage = typename Traits_::Storage;
			using Value = typename Traits_::Value;
			using Pointer = typename Traits_::Pointer;
			using ConstPointer = typename Traits_::ConstPointer;
			using Reference = typename Traits_::Reference;
			using ConstReference = typename Traits_::ConstReference;

			~Recso() noexcept
			{
				std::scoped_lock lock{ mutex };
				CRESS_MOO_RECSO_ASSERT(refcount > static_cast<size_t>(0));
				--refcount;
				if (0 == refcount)
				{
					Traits_::destroy(storage);
				}
			}

			Recso(This const&) noexcept = default;
			This& operator=(This const&) noexcept = default;
			Recso(This&&) noexcept = default;
			This& operator=(This&&) noexcept = default;

			/*
			//	Forwards arguments to |Traits::Create(storage, args...)|
			//	if this is the first reference that was created.
			*/
			template<class...Args>
			explicit Recso(Args&& ...args)
			{
				std::scoped_lock lock{ mutex };
				if (0 == refcount)
				{
					Traits_::create(storage, std::forward<Args>(args)...);
				}
				CRESS_MOO_RECSO_ASSERT(refcount != std::numeric_limits<size_t>::max());
				++refcount;
			}

			Recso()
			{
				std::scoped_lock lock{ mutex };
				if (0 == refcount)
				{
					Traits_::create(storage);
				}
				CRESS_MOO_RECSO_ASSERT(refcount != std::numeric_limits<size_t>::max());
				++refcount;
			}

			// refcount
			static size_t s_refCount() noexcept { return refcount.load(); }
			size_t refCount() const noexcept { return s_refCount(); }

			// address
			Pointer address() { return s_address(); }
			ConstPointer address() const { return s_addressConst(); }
			ConstPointer addressConst() const { return s_addressConst(); }

			Pointer operator->() { return s_address(); }
			ConstPointer operator->() const { return s_addressConst(); }

			// deref
			Reference deref() { return s_deref(); }
			ConstReference deref() const { return s_derefConst(); }
			Reference derefConst() const { return s_derefConst(); }

			Reference operator*() { return s_deref(); }
			ConstReference operator*() const { return s_derefConst(); }

		private:
			static Pointer s_address()
			{
				return Traits_::address(static_cast<Storage&>(storage));
			}
			static ConstPointer s_addressConst()
			{
				return Traits_::addressConst(static_cast<Storage const&>(storage));
			}
			static Reference s_deref()
			{
				return Traits_::deref(static_cast<Storage&>(storage));
			}
			static ConstReference s_derefConst()
			{
				return Traits_::derefConst(static_cast<Storage const&>(storage));
			}

			inline static Storage storage{};
			inline static std::atomic_size_t refcount = 0;
			inline static std::mutex mutex;
		};



		/*
		//	Base class of an example user-defined Traits type which is used as the type parameter for Rcso<Traits>.
		//	Allocates and deallocates the Value in aligned storage directly in the static Rcso variable.
		//
		//	The derived class must implement |static void create(Storage& storage, Args...)|
		//	and have it call |createImpl(storage, args...)|.
		//
		//	WARNING: You should not use this type as the Traits argument without deriving from it,
		//	because all other |Recso<StaticRecsoTraits<Value>>| with the same Value type will use the same storage.
		//
		//	Example:

			struct MyTraits : public cress::moo::StaticRecsoTraits<std::string>
			{
				using Super = cress::moo::StaticRecsoTraits<std::string>;
				static void create(typename Super::Storage& storage)
				{
					Super::createImpl(storage, "my data"); // constructs std::string(char const*)
				}
			};
			using MyRecso = cress::moo::Recso<MyTraits>;

			MyRecso a;
			std::cout << *a << '\n'; // prints "my data".
			MyRecso b;
			*b = "hello";
			std::cout << *a << '\n'; // prints "hello".

		*/
		template<class Value_>
		struct StaticAllocRecsoTraits
		{
			using This = StaticAllocRecsoTraits;
			using Value = Value_;

			using Storage = std::aligned_storage_t<sizeof(Value), alignof(Value)>;
			using Pointer = Value *;
			using ConstPointer = Value const*;
			using Reference = Value &;
			using ConstReference = Value const&;



			static Pointer address(Storage& storage) noexcept
			{
				return reinterpret_cast<Value*>(&storage);
			}

			static ConstPointer addressConst(Storage const& storage) noexcept
			{
				return reinterpret_cast<Value const*>(&storage);
			}

			static Reference deref(Storage& storage) noexcept
			{
				return *address(storage);
			}

			static ConstReference derefConst(Storage const& storage) noexcept
			{
				return *addressConst(storage);
			}

			static void destroy(Storage& storage) noexcept
			{
				address(storage)->~Value();
			}

			//
			//	This is named |createImpl| to prevent unwanted overloading of |Traits::create|.
			//
			template<class...Args>
			static void createImpl(Storage& storage, Args&& ...args)
				noexcept(std::is_nothrow_constructible_v<Value, Args&&...>)
			{
				new (address(storage)) Value(std::forward<Args>(args)...);
			}
		};



		template<class Value_>
		struct HeapAllocRecsoTraits
		{
			using This = HeapAllocRecsoTraits;
			using Value = Value_;

			using Storage = Value *;
			using Pointer = Value *;
			using ConstPointer = Value const*;
			using Reference = Value &;
			using ConstReference = Value const&;



			static Pointer address(Storage storage) noexcept
			{
				return storage;
			}

			static ConstPointer addressConst(Storage storage) noexcept
			{
				return storage;
			}

			static Reference deref(Storage storage) noexcept
			{
				return *storage;
			}

			static ConstReference derefConst(Storage storage) noexcept
			{
				return *storage;
			}

			static void destroy(Storage storage) noexcept
			{
				delete storage;
			}

			template<class...Args>
			static void createImpl(Storage& storage, Args&& ...args)
				noexcept(std::is_nothrow_constructible_v<Value, Args&& ...>)
			{
				storage = new Value(std::forward<Args>(args)...);
			}
		};



		/*
		//	Example:
				std::string createApple()
				{
					return "apple";
				}
				using AppleRecso = typename cress::moo::StaticAllocRecsoMetaFactory<std::string, createApple>::type;
		*/
		template<class Value, Value(*maker)()>
		struct StaticAllocRecsoMetaFactory
		{
			struct Traits : public StaticAllocRecsoTraits<Value>
			{
				using Super = StaticAllocRecsoTraits<Value>;
				static void create(typename Super::Storage& storage)
				{
					Super::createImpl(storage, maker());
				}
			};
			using type = Recso<Traits>;
		};

		template<class Value, Value(*maker)()>
		using StaticAllocRecsoMetaFactory_t = typename StaticAllocRecsoMetaFactory<Value, maker>::type;
	}
}
