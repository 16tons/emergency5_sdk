// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheBase.h"

#include <qsf/base/WeakPtr.h>

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class MovableComponent;
	namespace game
	{
		class BitStream;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 entity with a movable componet data cache for a single entity for multiplayer
		*/
		class MovableData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			MovableData(const qsf::Entity& targetEntity);
			virtual ~MovableData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool includeForPrepare(const int32 currentTickCount, bool entityIsVisible);
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Checks if the entity is linked to a another entity as child
			bool isEntityLinked(const qsf::Entity& entity) const;
			bool preparePositionForUpdate(const qsf::MovableComponent& movableComponent, bool includeHeight = false, bool force = false);
			void writePositionToStream(qsf::game::BitStream& bitStream);
			bool readPositionFromStream(const qsf::game::BitStream& bitStream, glm::vec3& position);

			bool prepareRotationForUpdate(const qsf::MovableComponent& movableComponent, bool force = false);
			void writeRotationToStream(qsf::game::BitStream& bitStream);
			bool readRotationFromStream(const qsf::game::BitStream& bitStream, float& rotation);

			bool prepareVelocityForUpdate(const qsf::MovableComponent& movableComponent);
			void writeVelocityToStream(qsf::game::BitStream& bitStream);
			bool readVelocityFromStream(const qsf::game::BitStream& bitStream, glm::vec3& velocity);

			glm::vec3 calculateNewPosition(const glm::vec3& position, const glm::vec3& velocity, float timePassedInSeconds);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<qsf::MovableComponent> mComponent;
			bool		mInitialTransfer;
			bool		mForceTransfer;
			bool		mUnuseable;
			bool		mEntityVisible;
			bool		mIsUnit;

			bool		mVelocityChanged;
			glm::vec3	mLastVelocity;
			uint32		mChangedVelocity[2];
			bool		mChangedVelocityChanged[2];
			bool		mPositionChanged;
			glm::vec3	mLastPosition;
			uint32		mChangedPosition[3];
			bool		mChangedPositionChanged[3];
			bool		mRotationChanged;
			glm::quat	mLastRotation;
			int8		mChangedRotation;
			uint64		mContainerEntityId;	///< The entity id of the container to which this entity is assigned
			bool		mContainerEntityChanged;
			uint8		mContainerCategory;		///< Currently we have only 3 categories (value range 0-2)
			bool		mIsMoving;

			// Only for client side
			glm::vec3	mCurrentVelocity;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				bool		velocityChanged;
				glm::vec3	velocity;
				bool		positionChanged;
				glm::vec3	position;
				bool		rotationChanged;
				float		rotationAroundYAxis;
				uint64		containerEntityId;	///< The entity id of the container to which this entity is assigned
				bool		containerEntityChanged;
				uint8		mContainerCategory;
				bool		mIsMoving;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
