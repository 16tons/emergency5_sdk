// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_game/network/BitStream.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{

		/**
		*  @brief
		*    Helper methods to read/write enum class values from/to bit stream
		*/
		template <class T>
		inline void writeToBitstream(qsf::game::BitStream& bitStream, const T& enumClassValue)
		{
			bitStream.write(static_cast<typename std::underlying_type<T>::type>(enumClassValue));
		}

		template <class T>
		inline void writeToBitstream(qsf::game::BitStream& bitStream, const T& enumClassValue, size_t bitCount)
		{
			bitStream.write(static_cast<typename std::underlying_type<T>::type>(enumClassValue), bitCount);
		}

		template <class T>
		inline bool readFromBitstream(const qsf::game::BitStream& bitStream, T& enumClassValue)
		{
			typename std::underlying_type<T>::type value;
			if (bitStream.read(value))
			{
				enumClassValue = static_cast<T>(value);
				return true;
			}

			return false;
		}

		template <class T>
		inline bool readFromBitstream(const qsf::game::BitStream& bitStream, T& enumClassValue, size_t bitCount)
		{
			typename std::underlying_type<T>::type value;
			if (bitStream.read(value, bitCount))
			{
				enumClassValue = static_cast<T>(value);
				return true;
			}

			return false;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
