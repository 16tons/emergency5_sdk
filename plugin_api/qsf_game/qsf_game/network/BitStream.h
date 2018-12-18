// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class IBitStream;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Game bit stream class (just a wrapper for linnet bit stream)
		*/
		class QSF_GAME_API_EXPORT BitStream : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			enum ResetHint
			{
				RESET_ALL,
				RESET_READ,
				RESET_WRITE
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			BitStream();

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] bitStream
			*    The actual bit stream object we're going to wrap up
			*
			*  @note
			*    - all classes that are meant to use this constructor know about IBitStream
			*/
			BitStream(IBitStream& bitStream, bool isOwner = false);

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] bitStream
			*    The actual bit stream object we're going to wrap up
			*
			*  @note
			*    - all classes that are meant to use this constructor know about IBitStream
			*/
			BitStream(const IBitStream& bitStream);

			BitStream(const std::vector<char>& buffer);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BitStream();

			void serializeToBuffer(std::vector<char>& buffer) const;

			/** resets the read write position of the stream (without purging the existing memory) */
			void reset(ResetHint hint = RESET_ALL);

			/** retrieves the current size of the bit stream in bits */
			size_t getBitLength() const;

			/** sets the read position in bits */
			void setReadPosition(size_t position) const;

			/** gets the read position in bits */
			size_t getReadPosition() const;

			/** writes a bool value in 1 bit */
			void write(bool value);

			/** reads a 1-bit bool value */
			bool read(bool& value) const;

			/** writes a byte value in number bits */
			void write(uint8 value, size_t number = 8);

			/** reads an unsigned byte value in number bits */
			bool read(uint8& value, size_t number = 8) const;

			/** writes a signed char value in number bits (including a sign indicating bit) */
			void write(int8 value, size_t number = 8);

			/** reads a signed char value in number bits (including a sign indicating bit) */
			bool read(int8& value, size_t number = 8) const;

			/** writes an unsigned 16bit value in number bits */
			void write(uint16 value, size_t number = 16);

			/** reads an unsigned 16bit value in number bits */
			bool read(uint16& value, size_t number = 16) const;

			/** writes a signed 16bit value in number bits (including a sign indicating bit) */
			void write(int16 value, size_t number = 16);

			/** reads a signed 16bit value in number bits (including a sign indicating bit) */
			bool read(int16& value, size_t number = 16) const;

			/** writes an unsigned 32bit value in number bits */
			void write(uint32 value, size_t number = 32);

			/** reads an unsigned 32bit value in number bits */
			bool read(uint32& value, size_t number = 32) const;

			/** writes a signed 32bit value in number bits (including a sign indicating bit) */
			void write(int32 value, size_t number = 32);

			/** reads a signed 32bit value in number bits (including a sign indicating bit) */
			bool read(int32& value, size_t number = 32) const;

			/** writes an unsigned 64bit value in number bits */
			void write(uint64 value, size_t number = 64);

			/** reads an unsigned 64bit value in number bits */
			bool read(uint64& value, size_t number = 64) const;

			/** writes a signed 64bit value in number bits (including a sign indicating bit) */
			void write(int64 value, size_t number = 64);

			/** reads a signed 64bit value in number bits (including a sign indicating bit) */
			bool read(int64& value, size_t number = 64) const;

			/** writes a complete float value */
			void write(float value);

			/** reads a complete float value */
			bool read(float& value) const;

			/** writes a complete double value */
			void write(double value);

			/** reads a complete double value */
			bool read(double& value) const;

			/** writes a byte array in length bytes */
			void write(uint8 buffer[], size_t length);

			/** reads a byte array in length bytes */
			bool read(uint8 buffer[], size_t length) const;

			/** writes number bits of stream into "this" stream */
			void write(const BitStream& stream, size_t number);

			/** reads number bits of "this" stream into stream */
			bool read(BitStream& stream, size_t number) const;

			/** creates a compressed copy of this bit stream */
			BitStream* huffmanCompress() const;

			/** creates an decompressed copy of this bit stream */
			BitStream* huffmanDecompress() const;

			/** Retrieves the internal IBitStream instance */
			IBitStream* getIBitStream();

			/** Retrieves the internal const IBitStream instance */
			const IBitStream* getIBitStream() const;

			// Some helpers
			template<typename T, typename O>
			inline bool readAs(O& value, size_t number = sizeof(T)*CHAR_BIT) const
			{
				T val;
				if (read(val, number))
				{
					value = static_cast<O>(val);
					return true;
				}
				return false;
			}

			template<typename T, typename O>
			inline void writeAs(const O& value, size_t number = sizeof(T)*CHAR_BIT)
			{
				write(static_cast<const T&>(value), number);
			}

			inline bool read(std::string& string, size_t lengthBits = 16) const
			{
				size_t length = 0;

				if (!readAs<uint32>(length, lengthBits))
				{
					return false;
				}

				string.resize(length);

				// When string is empty no need to read any data -> IBitStream even returns false when zero is given as second parameter to read
				if (length == 0)
				{
					return true;
				}

				return read((uint8*)string.data(), string.size());
			}

			inline void write(const std::string& string, size_t lengthBits = 16)
			{
				writeAs<uint32>(string.size(), lengthBits);
				write((uint8*)string.data(), string.size());
			}


		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
		private:
			static void serializeIBitStreamToBuffer(const IBitStream& bitStream, std::vector<char>& buffer);
			static std::string dumpIBitStreamAsString(const IBitStream& bitStream);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			IBitStream& mBitStream;
			bool mIsConst;
			bool mIsOwner;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
