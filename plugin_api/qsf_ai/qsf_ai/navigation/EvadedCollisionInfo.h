// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/serialization/binary/BinarySerializer.h>

namespace qsf
{
	class Entity;
}

namespace qsf
{
	namespace ai
	{
		/**
		* Tiny helper class encapsulating information about a collision that is dynamically evaded.
		*/
		class EvadedCollisionInfo
		{
		public:
			EvadedCollisionInfo(); // default c'tor necessary for serialization. Initializes an invalid pointer
			EvadedCollisionInfo(Entity& entity);

			// Several flags that can be combined into a bitmask
			enum Flag
			{
				IS_PHYSICAL_COLLISION = 1 << 0, // detected by physical collision testing
				IS_RESERVATION_COLLSIION = 1 << 1, // detected by reservation collision testing
				NO_NEED_TO_EVADE = 1 << 2, // trivial integration, we didn't need to change the path at all to evade this entity
				EVADED_SUCCESSFULLY = 1 << 3, // we have integrated a successful evasion into our path
				EVADED_UNSUCCESSFULLY = 1 << 4, // we have integrated an evasion movement into our path that does not guarantee to evade it yet
				EVASION_FAILED = 1 << 5, // we have tried but failed to integrate the entity into our path. We didn't change anything
				BLOCKING_GOAL = 1 << 6, // the other entity is blocking our goal position. do not remove, but also do not evade
			};

			// comfort getters and setters for the flags
			//@{
			bool isPhysicalCollision() const;
			bool isReservationCollision() const;
			bool isTrivialEvasion() const;
			bool isSuccessfulEvasion() const;
			bool isUnsuccessfulEvasion() const;
			bool isEvasionFailed() const;
			bool isBlockingGoal() const;
			void makePhysicalCollision();
			void makeReservationCollision();
			void setNoNeedToEvade();
			void setEvadedSuccessfully();
			void setEvadedUnsuccessfully();
			void setEvasionFailed();
			void setBlockingGoal();
			//@}

			// derived information from the core flags
			bool wasPathChanged() const;

			uint64 getEntityId() const;
			void setEntityById(uint64 entityId);

			WeakPtr<Entity> mEntity;
			short mStateFlags;
		};
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::EvadedCollisionInfo>
		{
			inline static void serialize(BinarySerializer& serializer, ai::EvadedCollisionInfo& data)
			{
				if (serializer.isWriting())
				{
					uint64 entityId = data.getEntityId();
					serializer & entityId;
				}
				else
				{
					uint64 entityId = getUninitialized<uint64>();
					serializer & entityId;
					data.setEntityById(entityId);
				}
				serializer & data.mStateFlags;
			}
		};
	}
}
