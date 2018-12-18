// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/track/SequenceTrack.h"
#include "qsf_logic/sequence/track/SequenceTrackKey.h"

#include <qsf/reflection/type/CampUint64.h>	// The 64 bit unsigned integer ID of the sequencer entity component property track class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type
#include <qsf/job/JobProxy.h>

#include <glm/glm.hpp>				// TODO(fw): Only needed for simulation-synchronous updates
#include <glm/gtc/quaternion.hpp>	// TODO(fw): Only needed for simulation-synchronous updates


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
	class Component;
	class PathSegmenter;
	class BoostSignalComponent;
	namespace logic
	{
		class SequenceTrackPathMovement;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Path movement sequence track key class
		*
		*  @note
		*    - All "value" variables (Bezier control points values, and mPathPosition) are expressed as a position along the path, measured in world units
		*    - Nevertheless, the key's value is stored as node index, because that's what designers actually define when editing a path movement sequence
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackKeyPathMovement : public SequenceTrackKey
		{


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			float bezierControlPointIncomingValue;	///< See the class documentation for info about control point values
			float bezierControlPointIncomingTime;	///< See the class documentation for info about control point times
			float mPathPosition;					///< Position on the path, in world units from the first node
			float bezierControlPointOutgoingValue;	///< See the class documentation for info about control point values
			float bezierControlPointOutgoingTime;	///< See the class documentation for info about control point times
			bool  bezierSplitTangents;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceTrackKeyPathMovement(uint32 frameNumber, SequenceTrackPathMovement& track);

			float getPathPosition() const;
			void setPathPosition(float pathPosition);

			float getNodeIndex() const;
			void setNodeIndex(float nodeIndex);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceTrackPathMovement& mTrack;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


		/**
		*  @brief
		*    Path movement sequence track class
		*/
		class QSF_LOGIC_API_EXPORT SequenceTrackPathMovement : public SequenceTrack
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceTrackPathMovement(Sequence* sequence, uint32 id);
			virtual ~SequenceTrackPathMovement();

			uint64 getActorEntityId() const;
			void setActorEntityId(uint64 actorEntityId);
			Entity* getActorEntity() const;

			uint64 getPathEntityId() const;
			void setPathEntityId(uint64 pathEntityId);
			Entity* getPathEntity() const;

			const PathSegmenter& getPathSegmenter() const;

			bool isInsideKeyInterval(float time) const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::logic::SequenceTrack methods      ]
		//[-------------------------------------------------------]
		public:
			virtual void serializeOriginalControlledValue(BinarySerializer& serializer) override;
			virtual uint32 getKeyCount() const override;
			virtual const SequenceTrackKey* getKeyFromIndex(uint32 index) const override;
			virtual SequenceTrackKey* getKeyFromIndex(uint32 index) override;
			virtual SequenceTrackKeyWithIndex createKeyAtFrame(uint32 frame) override;
			virtual void removeKeyAtIndex(uint32 index) override;
			virtual void deleteAllKeys() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::logic::SequenceTrack methods   ]
		//[-------------------------------------------------------]
		protected:
			virtual void notifyTimePassed(float oldTime, float newTime) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateSimulation(const JobArguments&);

			/**
			*  @brief
			*    Get "qsf::BoostSignalComponent" instance from the path entity
			*
			*  @return
			*    "qsf::BoostSignalComponent" instance, can be a null pointer, do not destroy the returned instance
			*/
			BoostSignalComponent* getPathBoostSignal() const;

			/**
			*  @brief
			*    Called in case a component property value was changed
			*
			*  @param[in] component
			*    Component instance which had a property value change
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*
			*  @note
			*    - The unique class property identifier is generated by "qsf::StringHash" by using the class property name
			*      (e.g. qsf::StringHash("Position") generating the hash value 3091144222)
			*/
			void onComponentPropertyChange(const Component& component, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<SequenceTrackKeyPathMovement*> KeyList;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const Map&			   mMap;			///< Map instance, don't destroy the instance
			uint64				   mActorEntityId;
			uint64				   mPathEntityId;
			KeyList				   mKeys;
			mutable PathSegmenter* mPathSegmenter;

			// Movement update synchronously to the simulation
			// TODO(fw): This is a quite ugly solution, but it works for now
			//  -> We should rethink our general sequencer update strategy:
			//      - Updates in real-time are necessary for non-game-world effects (screen fading, UI, also camera movement)
			//      - Some updates must be done inside the simulation update (especially person/vehicle movement along a path; everything accessing the "qsf::MovableComponent")
			JobProxy		  mSimulationUpdateJob;
			mutable bool	  mSimulationTransformationValid;
			mutable	glm::vec3 mPosition;
			mutable glm::quat mRotation;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::logic::SequenceTrackKeyPathMovement)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::SequenceTrackPathMovement)
