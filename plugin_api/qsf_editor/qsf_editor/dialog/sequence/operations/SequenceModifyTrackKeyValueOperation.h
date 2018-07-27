// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/sequence/operations/SequenceTrackOperation.h"

#include <boost/property_tree/ptree.hpp>


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
		*    Modify sequence track key value operation class
		*
		*  @note
		*    TODO(oi) write some notes about the operation
		*/
		class SequenceModifyTrackKeyValueOperation : public SequenceTrackOperation
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceModifyTrackKeyValueOperation(uint32 trackId, uint32 frameNumber);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SequenceModifyTrackKeyValueOperation();

			void setNewKeyValue(const boost::property_tree::ptree& newValue);
			void setOldKeyValue(const boost::property_tree::ptree& oldValue);
			void modifyKeyValue(const boost::property_tree::ptree& sourceValue);


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
			float						mFrameNumber;
			bool						mWasOldValueSupplied;
			boost::property_tree::ptree	mSavedKeyValue;
			boost::property_tree::ptree	mNewKeyValue;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::SequenceModifyTrackKeyValueOperation)
