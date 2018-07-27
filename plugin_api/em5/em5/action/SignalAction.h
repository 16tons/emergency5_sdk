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
#include <qsf/message/MessageParameters.h>


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
	*    EMERGENCY 5 signal action
	*/
	class EM5_API_EXPORT SignalAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::SignalAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SignalAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SignalAction();

		/**
		*  @brief
		*    Initialize with a message emitted on action update and optionally on action abort
		*
		*  @remarks
		*    The message as defined by message configuration and message parameters is emitted when this action is first updated.
		*    The same message is optionally also emitted when this action is aborted (i.e. destroyed before being updated once).
		*/
		void init(const qsf::MessageConfiguration& message, const qsf::MessageParameters& messageParameters = qsf::MessageParameters(), bool emitMessageOnAbort = true);

		/**
		*  @brief
		*    Initialize with different messages to be emitted on action update and on action abort
		*/
		void init(const qsf::MessageConfiguration& messageAtUpdate, const qsf::MessageConfiguration& messageAtAbort,
				  const qsf::MessageParameters& messageParametersAtUpdate = qsf::MessageParameters(), const qsf::MessageParameters& messageParametersAtAbort = qsf::MessageParameters());


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;
		virtual void onAbort() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						mSignalOnAbort;
		qsf::MessageConfiguration	mMessageAtUpdate;
		qsf::MessageParameters		mMessageParametersAtUpdate;
		qsf::MessageConfiguration	mMessageAtAbort;
		qsf::MessageParameters		mMessageParametersAtAbort;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SignalAction)
