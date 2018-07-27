// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base//NamedIdentifier.h"
#include "qsf/serialization/binary/BinarySerializer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Flyweight string class
	*
	*  @remarks
	*    All flyweight instances of the same string share the same string internally, instead of each one having a copy of the same string.
	*    The principle is borrowed from "boost::flyweights::flyweight", but here specialized for strings.
	*    The specialization includes having the string hash available at any time.
	*    Though the main reason for not using "boost::flyweights::flyweight<std::string>" any more was relatively poor performance,
	*    e.g. when just creating an empty flyweight string via default constructor. This was noticeable especially in release builds.
	*/
	class FlyweightString
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline FlyweightString();
		inline FlyweightString(const char* string);
		inline FlyweightString(const std::string& string);
		inline FlyweightString(const NamedIdentifier& string);
		inline FlyweightString(const FlyweightString& string);
		inline ~FlyweightString();

		inline FlyweightString& operator=(const char* string);
		inline FlyweightString& operator=(const std::string& string);
		inline FlyweightString& operator=(const NamedIdentifier& string);
		inline FlyweightString& operator=(const FlyweightString& string);

		inline bool operator==(const FlyweightString& string) const;
		inline bool operator!=(const FlyweightString& string) const;

		inline operator const std::string&() const;
		inline operator const NamedIdentifier&() const;

		inline bool empty() const;

		inline uint32 getHash() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Entry
		{
			NamedIdentifier mString;
			uint32 mReferenceCounter;

			Entry() : mReferenceCounter(0) {}
			Entry(const NamedIdentifier& string) : mString(string), mReferenceCounter(0) {}
		};

		typedef std::map<uint32, Entry> EntryMap;	// An alternative could be "std::unordered_map"; it's important that existing entries stay stable


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline void registerEntry(Entry& entry);
		void registerEntry(const NamedIdentifier& string);
		void unregisterEntry();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		static EntryMap	mStaticEntries;		///< Shared map of string entries
		static Entry	mStaticEmptyEntry;	///< Shared default entry for empty strings

		Entry* mEntry;		///< Pointer to own entry; this is always valid (except temporarily inside internal methods)


	};


	// Operators
	inline std::string operator+(const std::string& left, const FlyweightString& right)  { return left + static_cast<const std::string&>(right); }


	// Binary serialization
	namespace serialization
	{
		template <>
		struct serializer<FlyweightString>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, FlyweightString& data)
			{
				// TODO(fw): Better would serialization as "qsf::NamedIdentifier", but we can't change this without breaking compatibility
				serializer.serializeAs<std::string>(data);
			}
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


  //[-------------------------------------------------------]
  //[ Implementation                                        ]
  //[-------------------------------------------------------]
#include "qsf/base/flyweight/FlyweightString-inl.h"
