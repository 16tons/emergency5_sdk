// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>
#include <qsf/reflection/type/CampGlmQuat.h>


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
	*    EMERGENCY 5 load boat into transporter action
	*/
	class EM5_API_EXPORT LoadBoatToTransporterAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::LoadBoatToTransporterAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_SUPPORTLEGS_EXPAND,
			STATE_CRANE_ALIGN_ARM,
			STATE_CRANE_ARM_DOWN,
			STATE_CRANE_LOAD_CARGO,
			STATE_SUPPORTLEGS_HIDE,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		LoadBoatToTransporterAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LoadBoatToTransporterAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity);

		/**
		*  @brief
		*    Get entity ID of the boat
		*/
		uint64 getTargetId() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void linkBoatToCraneEntity(qsf::Entity& boatEntity);
		void linkBoatToVehicle(qsf::Entity& boatEntity);
		qsf::Entity* getFirstChildEntity(qsf::Entity* entity);
		void linkParticles(const qsf::Entity& targetEntity, qsf::Entity* particleEntity, float delay, float timeout);
		void unlinkParticles(qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64			mTargetEntityId;
		State			mState;
		glm::quat		mDefaultRotation;
		glm::vec3		mDropPosition;

		// Only for temporary particle effects, no weak pointer necessary
		qsf::Entity*	mWatersprayEntity;
		qsf::Entity*	mWaterdropsEntity;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::LoadBoatToTransporterAction)
