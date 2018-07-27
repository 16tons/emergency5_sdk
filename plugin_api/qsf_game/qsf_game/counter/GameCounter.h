// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/NamedIdentifier.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}


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
		*    Game counter class
		*/
		class QSF_GAME_API_EXPORT GameCounter : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline explicit GameCounter(const std::string& name);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~GameCounter();

			/**
			*  @brief
			*    Returns the name of the game counter
			*/
			inline const std::string& getName() const;

			/**
			*  @brief
			*    Returns the ID of the game counter
			*/
			inline uint32 getId() const;

			/**
			*  @brief
			*    Reset the counter value to zero
			*/
			inline void reset();

			/**
			*  @brief
			*    Return the current counter value
			*/
			inline int32 getValue() const;

			/**
			*  @brief
			*    Set the current counter value
			*/
			void setValue(int32 value);

			/**
			*  @brief
			*    Increase the current counter value by one
			*/
			inline int32 operator++();

			/**
			*  @brief
			*    Decrease the current counter value by one
			*/
			inline int32 operator--();

			/**
			*  @brief
			*    Serialization of the timer
			*/
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NamedIdentifier mName;		///< Name and ID of this counter; must be unique for each counter
			int32			mValue;		///< Current counter value


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/counter/GameCounter-inl.h"
