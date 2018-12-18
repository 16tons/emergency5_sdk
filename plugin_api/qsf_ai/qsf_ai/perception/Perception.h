// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Global defintions                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    The PerceptionState describes
		*    which state the perception is in.
		*/
		enum PerceptionState
		{
			// order matters. Do not touch!
			//@{
			PERCEPTION_BEGAN = 0,	// the perceiver was just perceived, for example when an entity just entered a sensor's range.
			PERCEPTION_CONTINUED,	// the perceiver is still perceived, for example when it is still inside a sensor's range and was already "seen" by the sensor the last time an update was performed.
			PERCEPTION_ENDED,		// a former perceiver is not perceived anymore, for example when the entity left a sensor's range.
			//@}
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A perception describes an entity perceived by a sensor.
		*    It also contains information on the perception's state.
		*/
		class Perception {


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class SensorConfiguration;  // grant sensor-configuration direct access to the internal entity-pointer


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			Perception(); // necessary for std::vector::resize
			Perception(Entity& collider);

			// Serialize this perception.
			void serialize(BinarySerializer& serializer);

			// get the id of the perceived entity.
			uint64 getEntityID() const;

			// get/set the state this perception is in (see PerceptionState)
			//@{
			PerceptionState getState() const;
			void setState(PerceptionState state);
			//@}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// direct access function used internally by the sensor processing code.
			//@{
			Entity* getEntity() const;
			void invalidate();
			//@}


		//[-------------------------------------------------------]
		//[ Private data                                       ]
		//[-------------------------------------------------------]
		private:
			uint64 mEntityID; // the id of the perceived entity
			Entity* mEntity; // a direct pointer to the perceived entity. Used internally to speed up sensor processing.
			PerceptionState mState; // the perception's state; e.g. is the entity seen "for the first time" or is the sensor perceiving it for several time already
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} //qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/Perception-inl.h"
