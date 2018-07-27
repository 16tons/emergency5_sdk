// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/data/OperationData.h"

#include <qsf/reflection/type/CampBoostPTree.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
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
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Backup component operation data class
			*/
			class QSF_EDITOR_BASE_API_EXPORT BackupComponentOperationData : public OperationData
			{


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
				friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to "qsf::editor::base::BackupComponentOperationData::mComponentBackup" in order to connect it as CAMP array


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_DATA_ID;	///< "qsf::editor::base::BackupComponentOperationData" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				BackupComponentOperationData();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] component
				*    The component instance to backup
				*/
				explicit BackupComponentOperationData(Component& component);

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] component
				*    The component instance to backup
				*  @param[in] storeOverrideStates
				*    Indicates if also the property override states should be stored
				*/
				BackupComponentOperationData(Component& component, bool storeOverrideStates);

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] componentId
				*    The component ID to backup
				*  @param[in] pTree
				*    The component data to store as backup (any property override states stored in the pTree is ignored on restore)
				*/
				BackupComponentOperationData(uint32 componentId, boost::property_tree::ptree& pTree);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~BackupComponentOperationData();

				/**
				*  @brief
				*    Return the component backup as boost ptree
				*
				*  @return
				*    The component backup as boost ptree
				*/
				const boost::property_tree::ptree& getComponentBackup() const;

				/**
				*  @brief
				*    Restore the given component
				*
				*  @param[in] entity
				*    Entity to add the component to
				*
				*  @note
				*    - The created component instance is not started automatically
				*/
				void restoreComponent(Entity& entity);

				/**
				*  @brief
				*    Restore the given component
				*
				*  @param[in] component
				*   The component for which the data should be restored
				*/
				void restoreComponent(Component& component);


			//[-------------------------------------------------------]
			//[ Public qsf::editor::OperationData methods             ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual size_t getMemoryConsumption() const override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Backup the given component
				*
				*  @param[in] component
				*    Component to backup
				*
				*  @note
				*    - We ignore prototype component data to ensure redo/undo is consistent, even in case the prototype was changed in the meanwhile
				*/
				void backupComponent(Component& component);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				uint32						mComponentId;				///< Component ID
				bool						mBackupWithOverrideState;	///< Indicates if the backup contains override states
				boost::property_tree::ptree mComponentBackup;			///< Component backup


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::BackupComponentOperationData)
