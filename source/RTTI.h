
#pragma once

#include "pch.h"

namespace Ecosim
{
	/**	Base class for a runtime type ID system.
	 */
	class RTTI
	{
	public:

		virtual ~RTTI() = default;

		virtual std::uint64_t instanceTypeID() const = 0;

		virtual RTTI* queryInterface(const std::uint64_t id) const
		{
			UNREFERENCED_PARAMETER(id);
			return nullptr;
		}

		virtual bool is(std::uint64_t id) const
		{
			UNREFERENCED_PARAMETER(id);
			return false;
		}

		virtual bool is(const std::string& name) const
		{
			UNREFERENCED_PARAMETER(name);
			return false;
		}

		template <typename T>
		T* as() const
		{
			if(is(T::classTypeID()))
			{
				return (T*)this;
			}

			return nullptr;
		}

		virtual std::string toString() const
		{
			return "RTTI";
		}

		virtual bool equals(const RTTI* rhs) const
		{
			return this == rhs;
		}
	};

#define RTTI_DECLARATIONS(Type, ParentType)																	 \
		public:                                                                                              \
			typedef ParentType Parent;                                                                       \
			static std::string typeName() { return std::string(#Type); }                                     \
			static std::uint64_t classTypeID() { return sRunTimeTypeID; }                                    \
			virtual std::uint64_t instanceTypeID() const override { return Type::classTypeID(); }            \
			virtual Ecosim::RTTI* queryInterface(const std::uint64_t id) const override					     \
            {                                                                                                \
                if (id == sRunTimeTypeID)                                                                    \
					{ return (RTTI*)this; }                                                                  \
                else                                                                                         \
					{ return Parent::queryInterface(id); }                                                   \
            }                                                                                                \
			virtual bool is(std::uint64_t id) const override                                                 \
			{                                                                                                \
				if (id == sRunTimeTypeID)                                                                    \
					{ return true; }                                                                         \
				else                                                                                         \
					{ return Parent::is(id); }                                                               \
			}                                                                                                \
			virtual bool is(const std::string& name) const override                                          \
			{                                                                                                \
				if (name == typeName())                                                                      \
					{ return true; }                                                                         \
				else                                                                                         \
					{ return Parent::is(name); }                                                             \
			}                                                                                                \
			private:                                                                                         \
				static std::uint64_t sRunTimeTypeID;

#define RTTI_DEFINITIONS(Type) std::uint64_t Type::sRunTimeTypeID = reinterpret_cast<std::uint64_t>(&Type::sRunTimeTypeID);
}
