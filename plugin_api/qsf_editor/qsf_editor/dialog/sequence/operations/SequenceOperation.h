// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/Operation.h>

// TODO(co) Check if this include can be removed
#include <qsf/reflection/type/CampUint64.h>	// The 64 bit unsigned integer ID of the entity operation class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class SequenceEditorState;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Abstract sequence operation base class
		*
		*  @note
		*    TODO(oi) write some notes about the operation
		*/
		class SequenceOperation : public base::Operation
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SequenceOperation();

			/**
			*  @brief
			*    Set editor state (do before sealing)
			*/
			void setSequenceEditorState(SequenceEditorState* sequenceEditorState);


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual size_t getMemoryConsumption() const override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			SequenceOperation();


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			SequenceEditorState* mSequenceEditorState;	///< Editor state reference is kept so operations can be executed upon it. The editor state also remembers the sequence reference.


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::SequenceOperation)
