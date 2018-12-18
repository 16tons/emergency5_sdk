// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"


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
	*    Bit flag set class template
	*
	*  @remarks
	*    This is an ease-of-use class for bitmasks.
	*    You may optionally base it upon an enum, which must then contain concrete bitmask values (i.e. enum entries
	*    with explicitly defined values that are powers of two, or a bitwise combination of such values).
	*
	*  @note
	*    - If a BitFlagSet is based an enum, it requires the enum values to be already bit-shifted bitmask values.
	*    - See EnumBitmask class template for an alternative bitmask class. EnumBitmask expects the enum it is based on
	*      to contain only consecutively numbered entries instead of power-of-two values.
	*/
	template<typename STORAGE, typename ENUM = STORAGE>
	class BitFlagSet
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef STORAGE Storage;
		typedef ENUM Enum;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BitFlagSet() :
			mFlags(0)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] bitmask
		*    Bitmask of an arbitrary number flags, composed using the bitwise OR operator |
		*/
		explicit BitFlagSet(STORAGE bitmask) :
			mFlags(bitmask)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] other
		*    Bit flag set instance to copy
		*/
		BitFlagSet(const BitFlagSet& other) :
			mFlags(other.mFlags)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Check whether a certain flag is set
		*
		*  @param[in] flag
		*    The flag to test
		*
		*  @return
		*    "true" in case the flag is set, "false" otherwise
		*/
		bool isSet(ENUM flag) const
		{
			return (mFlags & static_cast<STORAGE>(flag)) == static_cast<STORAGE>(flag);
		}

		/**
		*  @brief
		*    Check whether a certain flag is clear, i.e. not set
		*
		*  @param[in] flag
		*    The flag to test
		*
		*  @return
		*    "true" in case the flag is clear, "false" if it is set
		*/
		bool isClear(ENUM flag) const
		{
			return !isSet(flag);
		}

		/**
		*  @brief
		*    Check whether all given flags are set
		*
		*  @param[in] bitmask
		*    Bitmask of an arbitrary number flags, composed using the bitwise OR operator |
		*
		*  @return
		*    "true" in case all flags are set, "false" otherwise
		*/
		bool allSet(STORAGE bitmask) const
		{
			return (mFlags & bitmask) == bitmask;
		}

		/**
		*  @brief
		*    Check whether any of the given flags are set
		*
		*  @param[in] bitmask
		*    Bitmask of an arbitrary number flags, composed using the bitwise OR operator |
		*
		*  @return
		*    "true" in case any flag is set, "false" otherwise
		*/
		bool anySet(STORAGE bitmask) const
		{
			return (mFlags & bitmask) != 0;
		}

		/**
		*  @brief
		*    Check whether any flag is set at all
		*
		*  @return
		*    "true" in case any flag is set, "false" otherwise
		*/
		bool anySet() const
		{
			return (mFlags != 0);
		}

		/**
		*  @brief
		*    Set a single flag
		*
		*  @param[in] flag
		*    The flag to set
		*/
		void set(ENUM flag)
		{
			mFlags |= static_cast<STORAGE>(flag);
		}

		/**
		*  @brief
		*    Set or clear a single flag
		*
		*  @param[in] flag
		*    The flag to set
		*  @param[in] value
		*    "true" to set the flag, "false" to clear it
		*/
		void set(ENUM flag, bool value)
		{
			if (value)
			{
				mFlags |= static_cast<STORAGE>(flag);
			}
			else
			{
				mFlags &= ~static_cast<STORAGE>(flag);
			}
		}

		/**
		*  @brief
		*    Set multiple flag at once
		*
		*  @param[in] bitmask
		*    Bitmask of an arbitrary number flags, composed using the bitwise OR operator |
		*/
		void setMultiple(STORAGE bitmask)
		{
			mFlags |= bitmask;
		}

		/**
		*  @brief
		*    Clear all flags
		*/
		void clearAll()
		{
			mFlags = 0;
		}

		/**
		*  @brief
		*    Clear a single flag
		*
		*  @param[in] flag
		*    The flag to clear
		*/
		void clear(ENUM flag)
		{
			mFlags &= ~static_cast<STORAGE>(flag);
		}

		/**
		*  @brief
		*    Clear multiple flag at once
		*
		*  @param[in] bitmask
		*    Bitmask of an arbitrary number flags, composed using the bitwise OR operator |
		*/
		void clearMultiple(STORAGE bitmask)
		{
			mFlags &= ~bitmask;
		}

		/**
		*  @brief
		*    Invert a single flag
		*
		*  @param[in] flag
		*    The flag to invert
		*/
		void invert(ENUM flag)
		{
			if ((mFlags & static_cast<STORAGE>(flag)) == 0)
			{
				mFlags |= static_cast<STORAGE>(flag);
			}
			else
			{
				mFlags &= ~static_cast<STORAGE>(flag);
			}
		}

		/**
		*  @brief
		*    Return storage
		*
		*  @return
		*    Bitmask of the currently set flag, composed using bitwise OR
		*/
		STORAGE getStorage() const
		{
			return mFlags;
		}

		/**
		*  @brief
		*    Return A reference to the actual internal storage (for direct write access)
		*
		*  @return
		*    Bitmask of the currently set flag, composed using bitwise OR
		*/
		STORAGE& getStorageRef()
		{
			return mFlags;
		}

		/**
		*  @brief
		*    Equality comparison operator
		*
		*  @param[in] other
		*    Bit flag set instance to compare to
		*
		*  @return
		*    "true" in case this enum bitmask is equal in all flags to the given one, "false" if there's any difference
		*/
		bool operator==(const BitFlagSet& other) const
		{
			return (mFlags == other.mFlags);
		}

		/**
		*  @brief
		*    Equality comparison operator
		*
		*  @param[in] other
		*    Bit flag set instance to compare to
		*
		*  @return
		*    "false" in case this enum bitmask is equal in all flags to the given one, "true" if there's any difference
		*/
		bool operator!=(const BitFlagSet& other) const
		{
			return (mFlags != other.mFlags);
		}

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] other
		*    Bit flag set instance to copy
		*
		*  @return
		*    Reference to this instance
		*/
		BitFlagSet& operator=(const BitFlagSet& other)
		{
			mFlags = other.mFlags;
			return *this;
		}

		/**
		*  @brief
		*    Serialize or deserialize the bitmask using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer)
		{
			serializer.serialize(mFlags);
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		STORAGE mFlags;	///< Bitmask of the currently set flags, composed using bitwise OR


	};


	namespace serialization
	{
		// Serialization specialization for qsf::BitFlagSet for use with the QSF binary serialization
		template<typename STORAGE, typename ENUM>
		inline void serialize(BinarySerializer& serializer, BitFlagSet<STORAGE, ENUM>& bitFlagSet)
		{
			bitFlagSet.serialize(serializer);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
