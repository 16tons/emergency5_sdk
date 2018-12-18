// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <qsf/base/NamedIdentifier.h>


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
		*    Static QSF logic messages collection class
		*/
		class QSF_LOGIC_API_EXPORT Messages
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    "SequenceStart" message, emitted just before a sequence will start
			*
			*  @remarks
			*    - Filters:
			*        (1): The global asset ID of the sequence, a uint64
			*    - Parameters:
			*        "sequenceId": The global asset ID of the sequence, a uint64
			*    - No Response
			*/
			static const NamedIdentifier QSF_LOGIC_SEQUENCE_START;

			/**
			*  @brief
			*    "SequenceEnd" message, emitted after a sequence is finished
			*
			*  @remarks
			*    - Filters:
			*        (1): The global asset ID of the sequence, a uint64
			*    - Parameters:
			*        "sequenceId": The global asset ID of the sequence, a uint64
			*		 "ownerComponent": Optional pointer to owning "qsf::logic::PlayerSequenceComponent" instance
			*    - No Response
			*/
			static const NamedIdentifier QSF_LOGIC_SEQUENCE_END;

			/**
			*  @brief
			*    "SequenceSetCamera" message, emitted if there's a current camera change inside the played sequence
			*
			*  @remarks
			*    - Filters:
			*        (1): The global asset ID of the sequence, a uint64
			*    - Parameters:
			*        "sequenceId": The global asset ID of the sequence, a uint64
			*        "entityId": ID of the entity the camera component is in
			*    - No Response
			*/
			static const NamedIdentifier QSF_LOGIC_SEQUENCE_SET_CAMERA;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			Messages();

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~Messages();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
