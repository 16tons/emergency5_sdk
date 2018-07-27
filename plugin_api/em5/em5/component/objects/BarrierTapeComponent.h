// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Color4;
	class Transform;
}
namespace em5
{
	class SimpleBarrierTapeComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 barrier tape component
	*/
	class EM5_API_EXPORT BarrierTapeComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::BarrierTapeComponent" unique component ID

		enum TapeConnection
		{
			POLE_TO_POLE,
			POLE_TO_SQUAD,
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit BarrierTapeComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BarrierTapeComponent();

		//[-------------------------------------------------------]
		//[ Preview poles                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a pole, if it does exist already we will return false
		*/
		bool createPole(uint32 poleIndex);

		/**
		*  @brief
		*    Move a pole
		*
		*  @param[in/out] targetPosition
		*    Position where the pole should be. Returns a position where it _would_ be (mainly fixing height)
		*/
		void movePole(uint32 poleIndex, const glm::vec3& targetPosition);

		/**
		*  @brief
		*    Returns the current pole position or a ZERO vector if no position is available for this pole
		*/
		const glm::vec3& getPolePosition(uint32 poleIndex) const;

		/**
		*  @brief
		*    Check if a poles is on a valid place
		*/
		bool isPolePositionValid(uint32 poleIndex) const;

		/**
		*  @brief
		*    Create the finished barrier
		*/
		void createBarrier();

		/**
		*  @brief
		*    Returns "true" if this barrier exists (=blocks path finding), "false" otherwise
		*/
		bool isBarrier() const;

		/**
		*  @brief
		*    Get a pointer to a pole entity, index can be 0 or 1, null if we don't have this pole
		*/
		qsf::Entity* getPoleEntity(uint32 poleIndex) const;

		/**
		*  @brief
		*    Number of poles which are placed, previews or all (maximum 2)
		*/
		uint32 getNumPoles() const;

		/**
		*  @brief
		*    Transparent poles
		*/
		void setPoleTransparency(uint32 poleIndex, float transparency);

		/**
		*  @brief
		*    Get the transparency of the specified pole or 0.0f if the pole does not exist
		*/
		float getPoleTransparency(uint32 poleIndex) const;

		/**
		*  @brief
		*    Transparent tape
		*/
		void setTapeTransparency(float transparency);

		/**
		*  @brief
		*    Get the transparency of the tape or 0.0f if the tape does not exist
		*/
		float getTapeTransparency() const;

		/**
		*  @brief
		*    Set pole markers
		*/
		void setPoleMarker(uint32 poleIndex, const qsf::Color4& color);

		/**
		*  @brief
		*    Delete pole markers
		*/
		void deletePoleMarker(uint32 poleIndex);

		/**
		*  @brief
		*    Tape can be connected between poles or from the first pole to the squad
		*
		*  @param[in] policeEntity
		*    The policeman entity
		*/
		void createTape(TapeConnection tapeConnection, qsf::Entity* policeEntity = nullptr);

		/**
		*  @brief
		*    Do we have a valid tape?
		*/
		bool hasTape() const;

		/**
		*  @brief
		*    Returns the current tape connection type
		*/
		TapeConnection getTapeConnectionType() const;

		/**
		*  @brief
		*    Returns the entity id of the police entity the tape is currently connected to or uninitialized if the tape is not connected to a police entity
		*/
		uint64 getTapeConnectionPoliceEntityId() const;

		/**
		*  @brief
		*     Destroy the barrier tape (no fadeout, just removing it)
		*/
		void destroyTape();

		/**
		*  @brief
		*    Remember if it's finished building up a barrier
		*/
		bool isFinishedBuilding() const;
		void setFinishedBuilding(bool finished);

		/**
		*  @brief
		*    Give back the center of the tape
		*/
		glm::vec3 getBarrierCenter() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Cleanup the poles and tapes
		void clearBarrier();

		// Is this a valid place to put up a pole?
		bool isValidPolePos(uint32 poleIndex, const qsf::Transform& targetTransform) const;

		// Find the ground position for the given position
		bool dropToGround(glm::vec3& position) const;

		bool isPoleCollisionValid(qsf::Entity& poleEntity, const qsf::Transform& targetTransform) const;
		bool isPolePositionValid(const qsf::Transform& targetTransform) const;
		bool isTapePositionValid(const glm::vec3& groundPos1, const glm::vec3& groundPos2) const;

		glm::quat getBarrierRotation() const;
		float getBarrierLength() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Poles for barrier tape commands/actions

		qsf::WeakPtr<qsf::Entity> mPoleEntities[2];
		qsf::WeakPtr<SimpleBarrierTapeComponent> mConnectedBarrierTape;
		qsf::WeakPtr<qsf::Entity> mClickEntity;
		float mPoleTransparency[2];

		TapeConnection mTapeConnection;
		uint64 mTapeConnectionPoliceEntityId;
		float mTapeTransparency;

		bool mFinishedBuilding;				///< State information for PlaceBarrierTapeCommandMode
		bool mIsBarrier;					///< State information for multiplayer mode


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BarrierTapeComponent)
