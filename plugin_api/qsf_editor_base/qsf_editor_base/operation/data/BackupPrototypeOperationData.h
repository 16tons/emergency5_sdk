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

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
	class Component;
	class Prototype;
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
			*    Backup prototype operation data class
			*/
			class QSF_EDITOR_BASE_API_EXPORT BackupPrototypeOperationData : public OperationData
			{


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
				friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to "qsf::editor::base::BackupPrototypeOperationData::mPrototypeBackup" in order to connect it as CAMP array


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_DATA_ID;	///< "qsf::editor::base::BackupPrototypeOperationData" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				inline BackupPrototypeOperationData();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] prototype
				*    The prototype instance to backup
				*/
				inline explicit BackupPrototypeOperationData(const Prototype& prototype);

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~BackupPrototypeOperationData();

				/**
				*  @brief
				*    Restore the given entity
				*
				*  @param[in] entityId
				*    The unique entity identifier of the entity to create
				*
				*  @return
				*    The entity instance created, or a null pointer in case restore failed
				*/
				Entity* restoreEntity(Map& map, uint64 entityId);

				/**
				*  @brief
				*    Return the name the restored entity will have
				*
				*  @param[out] name
				*    Will receive the UTF-8 name the restored entity will have on success, not touched on failure
				*
				*  @return
				*    "true" if all went fine, else "false"
				*/
				bool getEntityName(std::string& name) const;

				/**
				*  @brief
				*    Return the ID of the layer the restored entity will reside in
				*
				*  @param[out] layerId
				*    Will receive the ID of the layer the restored entity will reside in, not touched on failure
				*
				*  @return
				*    "true" if all went fine, else "false"
				*/
				bool getEntityLayerId(uint32& layerId) const;

				/**
				*  @brief
				*    Return the prototype backup
				*
				*  @return
				*    The prototype backup
				*/
				inline const boost::property_tree::ptree& getPrototypeBackup() const;


			//[-------------------------------------------------------]
			//[ Public qsf::editor::OperationData methods             ]
			//[-------------------------------------------------------]
			public:
				inline virtual uint32 getId() const override;
				inline virtual size_t getMemoryConsumption() const override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				/**
				*  @brief
				*    Backup the given prototype
				*
				*  @param[in] prototype
				*    Prototype to backup
				*
				*  @note
				*    - We ignore prototype data to ensure redo/undo is consistent, even in case the prototype was changed in the meanwhile
				*/
				void backupPrototype(const Prototype& prototype);

				/**
				*  @brief
				*    Backup the given component
				*
				*  @param[in]  component
				*    Component to backup
				*  @param[out] componentBackup
				*    Receives the component
				*
				*  @note
				*    - We ignore prototype component data to ensure redo/undo is consistent, even in case the prototype was changed in the meanwhile
				*/
				void backupComponent(Component& component, boost::property_tree::ptree& componentBackup);

				/**
				*  @brief
				*    Restore the given component
				*
				*  @param[in] entity
				*    Entity to add the component to
				*  @param[in] componentId
				*    The component's ID
				*  @param[in] componentBackup
				*    The backup of the component to restore
				*
				*  @note
				*    - The created component instance is not started automatically
				*/
				void restoreComponent(Entity& entity, uint32 componentId, const boost::property_tree::ptree& componentBackup);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				boost::property_tree::ptree mPrototypeBackup;	///< Prototype backup


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::BackupPrototypeOperationData)


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/data/BackupPrototypeOperationData-inl.h"
