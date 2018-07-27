// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/sequence/operations/SequenceOperation.h"
#include "qsf_editor/dialog/sequence/SequenceEditorSelectionManager.h"


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
		*    Operation which changes selection state
		*
		*  @note
		*    TODO(oi) write some notes about the operation
		*/
		class SequenceChangeSelectionOperation : public SequenceOperation
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			SequenceChangeSelectionOperation();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SequenceChangeSelectionOperation();

			void setOldSelection(const SequenceEditorSelectionManager::SelectionState& oldSelectionState);
			void setNewSelection(const SequenceEditorSelectionManager::SelectionState& newSelectionState);
			void setOldSelectionFromManager(const SequenceEditorSelectionManager& selectionManager);


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getId() const override;
			virtual size_t getMemoryConsumption() const override;
			virtual void undoImpl() override;
			virtual void redoImpl() override;
			virtual void sealImpl() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceEditorSelectionManager::SelectionState mOldSelectionState;
			SequenceEditorSelectionManager::SelectionState mNewSelectionState;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::SequenceChangeSelectionOperation)
