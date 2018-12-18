// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/Operation.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class OperationData;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Definitions                                           ]
			//[-------------------------------------------------------]
			typedef std::vector<Operation*> OperationArray;
			typedef std::vector<OperationData*> OperationDataArray;


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Compound operation class, used to collect several operations
			*/
			class QSF_EDITOR_BASE_API_EXPORT CompoundOperation : public Operation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::CompoundOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				CompoundOperation();

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~CompoundOperation();

				/**
				*  @brief
				*    Clear the list of operations, also deleting them
				*/
				void clear();

				/**
				*  @brief
				*    Add an operation at the end of the list of operations
				*
				*  @param[in] operation
				*    The operation to be added; null pointer is allowed, but pointless
				*
				*  @note
				*    - Only add operations that are dynamically allocated; CompoundOperation will become their new owner and care about their destruction
				*/
				void pushBackOperation(Operation* operation);

				/**
				*  @brief
				*    Add an operation data instance to the operation data list
				*
				*  @param[in] operationData
				*    The operation data instance to be added; null pointer is allowed, but pointless
				*
				*  @note
				*    - Only add operation data instances that are dynamically allocated; CompoundOperation will become their new owner and care about their destruction
				*/
				void addOperationData(OperationData* operationData);

				/**
				*  @brief
				*    Return an added operation data instance by its ID
				*
				*  @param[in] operationDataId
				*    The operattion data's ID (which is the internal index in CompoundOperation's operation data list)
				*/
				OperationData* getOperationDataById(uint32 operationDataId) const;

				const OperationArray& getOperations() const;
				void setOperations(const OperationArray& operations);

				const OperationDataArray& getOperationDatas() const;
				void setOperationDatas(const OperationDataArray& operationDatas);


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual size_t getMemoryConsumption() const override;
				virtual void sealImpl() override;
				virtual void undoImpl() override;
				virtual void redoImpl() override;


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				OperationArray	   mOperations;		///< List of operations this compound operation consists of
				OperationDataArray mOperationData;	///< List of operation data instances this compound operation consists of


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::CompoundOperation)
