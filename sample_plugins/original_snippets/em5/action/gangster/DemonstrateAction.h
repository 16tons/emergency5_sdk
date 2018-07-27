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
	*    EMERGENCY 5 demonstrate action; plays demonstration animations
	*/
	class EM5_API_EXPORT DemonstrateAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::DemonstrateAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DemonstrateAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DemonstrateAction();

		/**
		*  @brief
		*    Initialize action with values
		*
		*  @param[in] eventId
		*    ID of the event where the person is demonstrating
		*  @param[in] demonstrationCenter
		*    Center position of the demonstration
		*  @param[in] demonstrationRadius
		*    Radius of the demonstration
		*  @param[in] equipment
		*    Named Identifier of an equipment (from "em5/base/EquipmentAssets.h")
		*/
		void init(uint64 eventId, const glm::vec3& demonstrationCenter, float demonstrationRadius, const qsf::NamedIdentifier& equipment = qsf::NamedIdentifier());

		/*
		*  @brief
		*    Get back the event ID of this demonstration
		*/
		uint64 getEventId() const;


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
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64					mEventId;
		qsf::NamedIdentifier	mEquipment;
		glm::vec3				mDemonstrationCenter;
		float					mDemonstrationRadius;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DemonstrateAction)
