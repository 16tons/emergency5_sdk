// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/serialization/Serializer.h"
#include "qsf/base/GetUninitialized.h"

#include <iosfwd> // For forward declarations of std::istream and std::ostream
#include <string>


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
	*    Binary serializer class
	*/
	class QSF_API_EXPORT BinarySerializer : public Serializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const bool DEFAULT_IS_BIG_ENDIAN;	///< "true" if big endian is default endianness, i.e. byte order won't be reversed

		static const uint32 FLAG_EXTENDED_HEADER;
		static const uint32 FLAG_HAS_FORMAT_TYPE;

		static const uint32 INTERNAL_VERSION_BITMASK;
		static const uint32 FORMAT_VERSION_BITMASK;

		static const uint8 INTERNAL_VERSION;
		static const uint8 INTERNAL_VERSION_READABLE[];

		enum TokenMode
		{
			TOKEN_FLAG_NONE = 0,
			TOKEN_FLAG_TYPE = 1,
			TOKEN_FLAG_NAME = 2,
			TOKEN_ALL_FLAGS = 3
		};

		struct DataBlockInfo
		{
			friend class BinarySerializer;

		public:
			inline DataBlockInfo() :
				mBeginPosition(getUninitialized<uint64>()),
				mEndPosition(getUninitialized<uint64>())
			{
				// Nothing to do in here
			}

		protected:
			uint64 mBeginPosition;
			uint64 mEndPosition;
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		template<int N, typename T>
		static inline bool isVersionSupported(const T version, const T (&supportedVersions)[N]);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor: Open for reading, i.e. deserialization
		*/
		BinarySerializer(std::istream& stream);

		/**
		*  @brief
		*    Constructor: Open for writing, i.e. serialization
		*/
		BinarySerializer(std::ostream& stream, TokenMode tokenMode, uint16 formatVersion = 1, const std::string& formatType = "");

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BinarySerializer();

		/**
		*  @brief
		*    Return whether the serializer is currently reading/deserializing
		*/
		inline bool isReading() const;

		/**
		*  @brief
		*    Return whether the serializer is currently writing/serializing
		*/
		inline bool isWriting() const;

		/**
		*  @brief
		*    Return input stream the serializer operates on
		*
		*  @note
		*    - Throws an exception in case the serializer is not in read mode
		*/
		std::istream& getInputStream() const;

		/**
		*  @brief
		*    Return input stream the serializer operates on
		*
		*  @note
		*    - Throws an exception in case the serializer is not in read mode
		*/
		std::ostream& getOutputStream() const;

		//[-------------------------------------------------------]
		//[ Templatized serialization                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize a value with optional name (for usage as additional security token)
		*/
		template<typename T>
		void serialize(T& value, const char* name = nullptr);

		/**
		*  @brief
		*    Short version of (de-)serialization by an operator
		*/
		template<typename T>
		BinarySerializer& operator&(T& value);

		/**
		*  @brief
		*    Serialize or deserialize a value as another data type
		*/
		template<typename TARGETTYPE, typename ORIGINALTYPE>
		void serializeAs(ORIGINALTYPE& value, const char* name = nullptr);

		/**
		*  @brief
		*    Deserialize a value with optional name (for usage as additional security token)
		*
		*  @todo
		*    - TODO(fw): May be optimized internally
		*/
		template<typename T>
		T read(const char* name = nullptr);

		/**
		*  @brief
		*    Deserialize a value with optional name (for usage as additional security token)
		*
		*  @todo
		*    - TODO(fw): May be optimized internally
		*/
		template<typename T>
		void read(T& value, const char* name = nullptr);

		/**
		*  @brief
		*    Serialize a value with optional name (for usage as additional security token)
		*
		*  @todo
		*    - TODO(fw): May be optimized internally
		*/
		template<typename T>
		void write(const T& value, const char* name = nullptr);

		/**
		*  @brief
		*    Serialize a value with optional name (for usage as additional security token)
		*
		*  @todo
		*    - TODO(fw): May be optimized internally
		*/
		template<typename TARGETTYPE, typename ORIGINALTYPE>
		void writeAs(const ORIGINALTYPE& value, const char* name = nullptr);

		//[-------------------------------------------------------]
		//[ Raw memory                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize a raw block of memory
		*/
		void serializeRawBlock(void* address, uint32 bytes);

		/**
		*  @brief
		*    Serialize or deserialize a raw block of memory
		*/
		void serializeRawBlock(void* address, uint32 bytes, bool checkEndianness);

		/**
		*  @brief
		*    Deserialize a raw block of memory
		*/
		void readRawBlock(void* address, uint32 bytes);

		/**
		*  @brief
		*    Deserialize a raw block of memory
		*/
		void readRawBlock(void* address, uint32 bytes, bool checkEndianness);

		/**
		*  @brief
		*    Serialize a raw block of memory
		*/
		void writeRawBlock(const void* address, uint32 bytes);

		/**
		*  @brief
		*    Serialize a raw block of memory
		*/
		void writeRawBlock(const void* address, uint32 bytes, bool checkEndianness);

		/**
		*  @brief
		*    Skip a block of memory when reading
		*/
		void skip(uint32 bytes);

		//[-------------------------------------------------------]
		//[ Data blocks                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Serialize or deserialize the begin of a data block
		*/
		void beginDataBlock(DataBlockInfo& dataBlockInfo);

		/**
		*  @brief
		*    Serialize or deserialize the end of the given data block
		*/
		void endDataBlock(DataBlockInfo& dataBlockInfo);

		/**
		*  @brief
		*    Move the read position to the start of the given data block
		*/
		void jumpToBeginOfDataBlock(DataBlockInfo& dataBlockInfo);

		/**
		*  @brief
		*    Move the read position to the end of the given data block
		*/
		void jumpToEndOfDataBlock(DataBlockInfo& dataBlockInfo);

		//[-------------------------------------------------------]
		//[ Format and version                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the stream format type
		*/
		inline const std::string& getFormatType() const;

		/**
		*  @brief
		*    Return the stream format version
		*/
		inline uint16 getFormatVersion() const;

		/**
		*  @brief
		*    Return the internal stream format version
		*/
		inline uint8 getInternalVersion() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void tokenSerialization(uint32 token, const char* name);

		void readPortable(void* address, uint32 bytes, bool checkEndianness);
		void writePortable(const void* address, uint32 bytes, bool checkEndianness);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::istream*	mInputStream;
		std::ostream*	mOutputStream;
		TokenMode		mTokenMode;
		bool			mIsLittleEndianMachine;
		std::string		mFormatType;
		uint16			mFormatVersion;
		uint8			mInternalVersion;


	};


	// Default template definitions for serialization of variables with a binary serializer
	namespace serialization
	{

		// We use a function object template instead of a function template.
		// A function template specialisation with an own template parameters doesn't comply to the c++ standard! (Bad VC++ very bad from you to support such things)
		// Only with a class/struct template a specialisation with own template parameters works.
		template<typename T>
		struct serializer
		{
			inline static void serialize(qsf::BinarySerializer& serializer, T& value)
			{
				// When you get a compiler error here, it's because the type you try to serialize is not supported
				//  -> Most probably, you're just missing the right include for that type, see predefined serialization in "qsf/serialization/binary/"
#ifdef _MSC_VER
				T::UNDEFINED_SERIALIZATION_TYPE;
#endif
			}
		};

		template<typename T>
		struct serializer<const T>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, const T& value)
			{
				// When you get a compiler error here, it's because the type you try to serialize is const
#ifdef _MSC_VER
				T::CONST_SERIALIZATION_TYPE;
#endif
			}
		};

		template<typename T>
		uint32 getToken()
		{
			// Return default token
			return *(uint32*)("type");
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BinarySerializer-inl.h"
