// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/action/gangster/EscapeAction.h"


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
	*    EMERGENCY 5 escape car short action
	*/
	class EscapeCarShortAction : public EscapeAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::EscapeCarShortAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		EscapeCarShortAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EscapeCarShortAction();

		/**
		*  @brief
		*    Initialization
		*/
		void init(const std::string& escapeTargetTag);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint32 getMovementMode(uint32 movementModeId) const;
		bool checkAllowUpdate() const;
		bool checkPositionStuck();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Internal
		bool mIsCivilPerson;

		qsf::Time mStuckTime;
		glm::vec3 mTargetPosition;
		glm::vec3 mOldPosition;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EscapeCarShortAction)