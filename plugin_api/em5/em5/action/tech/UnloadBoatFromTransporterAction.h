// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    EMERGENCY 5 pickup vehicle by towcar action
	*/
	class EM5_API_EXPORT UnloadBoatFromTransporterAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::UnloadBoatFromTransporterAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_SUPPORTLEGS_EXPAND,
			STATE_CRANE_TURN_TO_TARGET,
			STATE_CRANE_LOWER_BOAT,
			STATE_CRANE_WAIT_BEFORE_FALL,
			STATE_CRANE_WAIT_AFTER_FALL,
			STATE_CRANE_TURN_BACK_TO_DEFAULT,
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
		UnloadBoatFromTransporterAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UnloadBoatFromTransporterAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity);

		/**
		*  @brief
		*    Get entity id of the drop point
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
		void linkBoatToCraneEntity();
		void unLinkBoatEntity(uint64 lakeId);
		void sendMessage(uint64 entityId) const;
		qsf::Entity* getFirstChildEntity(qsf::Entity* entity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64		mTargetEntityId;
		State		mState;
		float		mSecondsPassed;
		glm::quat	mDefaultRotation;
		glm::vec3	mDropPosition;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::UnloadBoatFromTransporterAction)
