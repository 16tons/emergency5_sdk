// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/component/ComponentOperation.h>

#include "qsf_editor/operation/terrain/TerrainDataMap.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TerrainComponent;
	namespace editor
	{
		class TerrainEditHelper;
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
		*    Abstract terrain component operation base class
		*/
		class TerrainOperation : public base::ComponentOperation
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to "qsf::editor::base::TerrainOperation::mOperationValues" in order to connect it as CAMP array


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::base::TerrainOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			TerrainOperation();

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] heightMapValues
			*    New height map values, this operation takes over the control of the given memory and will free it later on
			*/
			TerrainOperation(uint64 entityId, TerrainDataType dataType, std::vector<TerrainDataMapValue>& heightMapValues);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainOperation();

			/**
			*  @brief
			*    Return the entity terrain component this operation is performed on
			*
			*  @return
			*    The entity terrain component this operation is performed on, can be a null pointer, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			TerrainComponent* getTerrainComponent() const;


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getId() const override;
			virtual bool isLocal() const override;
			virtual size_t getMemoryConsumption() const override;
			virtual void sealImpl() override;
			virtual void undoImpl() override;
			virtual void redoImpl() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void undoRedo(bool undo);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<TerrainDataMapValue> mOperationValues;
			TerrainDataType					 mDataType;
			// Internal
			std::shared_ptr<TerrainEditHelper> mTerrainEditHelper;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainOperation);
