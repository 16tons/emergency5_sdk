// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"
#include "qsf_editor/selection/entity/EntitySelectionMarkerManager.h"

#include <qsf/selection/EntitySelectionManager.h>

#include <QtCore/qobject.h>

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	template <typename T>
		class ClassBuilder;
}
namespace qsf
{
	class Map;
	class Entity;
	namespace editor
	{
		class AddEntitiesToSelectionOperation;
		class RemoveEntitiesFromSelectionOperation;
		class SetCurrentEntityOperation;
		class SelectEntityOperation;
		namespace base
		{
			class User;
			class Operation;
			class CompoundOperation;
			class SelectEntityHiddenOperation;
			class DeselectEntityHiddenOperation;
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


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF editor entity selection manager class
		*
		*  @note
		*    - The currently selected entities can only be changed by using the "qsf::editor::SelectEntityOperation",
		*      "qsf::editor::AddEntitiesToSelectionOperation" and "qsf::editor::RemoveEntitiesFromSelectionOperation" operations
		*    - We need to public inherit from qsf::EntitySelectionManager otherwise compiler error: qsf/plugin/pluginable/PluginableManager-inl.h:38:61: error: ‘qsf::PluginableBase’ is an inaccessible base of ‘qsf::editor::EntitySelectionManager’
		*    - Due private inheritance the information of that base class gets "lost" for the outer world
		*/
		class QSF_EDITOR_API_EXPORT EntitySelectionManager : public QObject, public qsf::EntitySelectionManager
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			template <typename T>
				friend class camp::ClassBuilder;
			friend class SelectEntityOperation;
			friend class SetCurrentEntityOperation;
			friend class EntitySelectionMarkerManager;
			friend class AddEntitiesToSelectionOperation;
			friend class RemoveEntitiesFromSelectionOperation;
			friend class LockEntityOperation;
			friend class UnlockEntityOperation;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;		///< "qsf::editor::EntitySelectionManager" unique pluginable selection manager ID

			typedef boost::container::flat_set<uint64> IdSet;				///< Entity ID set
			typedef boost::container::flat_set<uint64> LockedEntitiesSet;	///< Set of locked entity IDs


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Calculate the world space center position of the given entities
			*
			*  @param[in]  idSet
			*    IDs of the entities to use in order to calculate the world space center position
			*  @param[out] center
			*    On success, will receive the calculated world space center position of the given entities, undefined content on failure
			*/
			static bool calculateIdSetCenterPosition(const IdSet& idSet, glm::vec3& center);


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after an selection change has occurred
			*
			*  @param[in] entity
			*    The new selected entity, can be a null pointer, do not destroy the instance
			*/
			void change(Entity* entity);

			/**
			*  @brief
			*    Qt signal emitted after an highlighted entity change has occurred
			*
			*  @param[in] entity
			*    The new highlighted entity change, can be a null pointer, do not destroy the instance
			*/
			void highlightedEntityChanged(Entity* entity);

			/**
			*  @brief
			*    Qt signal emitted after an entity got locked
			*
			*  @param[in] entityId
			*    ID of the locked entity
			*  @param[in] userId
			*    The ID of the user issuing the operation resulting in the lock
			*/
			void entityLocked(uint64 entityId, uint32 userId);

			/**
			*  @brief
			*    Qt signal emitted after an entity got unlocked
			*
			*  @param[in] entityId
			*    ID of the unlocked entity
			*  @param[in] userId
			*    The ID of the user issuing the operation resulting in the unlock
			*/
			void entityUnlocked(uint64 entityId, uint32 userId);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] selectionSystem
			*    The QSF selection system this selection manager instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit EntitySelectionManager(qsf::SelectionSystem* selectionSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EntitySelectionManager();

			/**
			*  @brief
			*    Return the current entity ("qsf::EntitySelectionManager::getCurrentId()")
			*
			*  @return
			*    The current entity, can be a null pointer, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			Entity* getCurrentEntity() const;

			//[-------------------------------------------------------]
			//[ Highlighting                                          ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the highlighted entity
			*
			*  @return
			*    The highlighted entity, can be a null pointer, do not destroy the instance
			*/
			Entity* getHighlightedEntity() const;

			/**
			*  @brief
			*    Return the ID of the highlighted entity
			*
			*  @return
			*    The ID of the highlighted entity, "qsf::getUninitialized<uint64><>()" if nothing is highlighted
			*/
			inline uint64 getHighlightedEntityId() const;

			/**
			*  @brief
			*    Set the highlighted entity
			*
			*  @param[in] entityId
			*    The ID of the entity to highlight, "qsf::getUninitialized<uint64><>()" to highlight nothing
			*/
			void setHighlightedEntityById(uint64 entityId);

			//[-------------------------------------------------------]
			//[ Locking                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Check whether or not the given entity ID is locked
			*
			*  @param[in] entityId
			*    Entity ID to check for lock
			*
			*  @return
			*    "true" if the given entity ID is locked, else "false"
			*
			*  @note
			*    - In case the entity is locked, the user will be informed about this via a status bar message
			*/
			bool isEntityLocked(uint64 entityId) const;

			/**
			*  @brief
			*    Return the set of locked entity IDs
			*
			*  @return
			*    Set of locked entity IDs, do not destroy the instance
			*/
			inline const LockedEntitiesSet& getLockedEntitiesSet() const;

			//[-------------------------------------------------------]
			//[ Operation                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Create an clear entities selection operation instance
			*
			*  @return
			*    Created clear entities selection operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			SelectEntityOperation* createClearEntitySelectionOperation() const;

			/**
			*  @brief
			*    Create an select entity operation instance
			*
			*  @param[in] entityId
			*    The unique entity identifier of the new selected entity
			*
			*  @return
			*    Created select entity operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			SelectEntityOperation* createSelectEntityOperation(uint64 entityId) const;

			/**
			*  @brief
			*    Create an select entities operation instance
			*
			*  @param[in] entityIdSet
			*    The unique entity identifiers to select
			*
			*  @return
			*    Created select entities operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			base::CompoundOperation* createSelectEntitiesOperation(const IdSet& entityIdSet) const;

			/**
			*  @brief
			*    Create an add entities to selection operation instance
			*
			*  @param[in] entityIdSet
			*    The unique entity identifiers to add to the selection
			*  @param[in] allEntitiesRequired
			*    By default, entities which are currently locked by another editor user are not going to be selected; if this parameter is
			*    "true" it means all or nothing, resulting in a null pointer being returned in case one of the requested entities is currently
			*    locked by another editor user
			*
			*  @return
			*    Created add entities to selection operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			AddEntitiesToSelectionOperation* createAddEntitiesToSelectionOperation(const IdSet& entityIdSet, bool allEntitiesRequired = false) const;

			/**
			*  @brief
			*    Create an remove entities to selection operation instance
			*
			*  @param[in] entityIdSet
			*    The unique entity identifiers to remove from the selection
			*
			*  @return
			*    Created remove entities to selection operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			RemoveEntitiesFromSelectionOperation* createRemoveEntitiesFromSelectionOperation(const IdSet& entityIdSet) const;

			/**
			*  @brief
			*    Create an set current entity (the focus of attention) operation instance
			*
			*  @param[in] entityId
			*    The unique entity identifier to set as current ID
			*
			*  @return
			*    Created set current entity operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			SetCurrentEntityOperation* createSetCurrentEntityOperation(uint64 entityId) const;

			/**
			*  @brief
			*    Create an select entity hidden operation instance
			*
			*  @param[in] entityId
			*    The unique entity identifier to select via hidden operation
			*
			*  @return
			*    Created select entity hidden operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			base::SelectEntityHiddenOperation* createSelectEntityHiddenOperation(uint64 entityId) const;

			/**
			*  @brief
			*    Create an select entity hidden operation instance
			*
			*  @param[in] entityIdSet
			*    The unique entity identifiers to select via hidden operation
			*
			*  @return
			*    Created select entity hidden operation instance, can be a null pointer, destroy the instance if you no longer need it
			*
			*  @note
			*    - This method will reject the whole selection attempt (return a nullptr) if any one of the given entity identifiers could not be locked
			*/
			base::SelectEntityHiddenOperation* createSelectEntityHiddenOperation(const IdSet& entityIdSet) const;

			/**
			*  @brief
			*    Create an deselect entity hidden operation instance
			*
			*  @param[in] entityId
			*    The unique entity identifier to deselect via hidden operation
			*
			*  @return
			*    Created deselect entity hidden operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			base::DeselectEntityHiddenOperation* createDeselectEntityHiddenOperation(uint64 entityId) const;

			/**
			*  @brief
			*    Create an deselect entity hidden operation instance
			*
			*  @param[in] entityIdSet
			*    The unique entity identifiers to deselect via hidden operation
			*
			*  @return
			*    Created deselect entity hidden operation instance, can be a null pointer, destroy the instance if you no longer need it
			*/
			base::DeselectEntityHiddenOperation* createDeselectEntityHiddenOperation(const IdSet& entityIdSet) const;

			//[-------------------------------------------------------]
			//[ Mirrored qsf::SelectionManagerTemplate getter methods ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the first selected ID
			*
			*  @return
			*    The selected ID, in case of multiple selected IDs the first one will be returned
			*/
			inline uint64 getSelectedId() const;

			/**
			*  @brief
			*    Return whole selection as set of IDs
			*
			*  @return
			*    Whole selection as set of IDs, do not destroy the instance
			*/
			inline const IdSet& getSelectedIdSet() const;

			/**
			*  @brief
			*    Return the number of currently selected IDs
			*
			*  @return
			*    The number of currently selected IDs
			*/
			inline size_t getSelectionSize() const;

			/**
			*  @brief
			*    Check whether or not the given ID is currently selected
			*
			*  @return
			*    "true" in case the given ID is currently selected, else "false"
			*/
			inline bool isIdSelected(uint64 id) const;

			/**
			*  @brief
			*    Return the current ID
			*
			*  @return
			*    The current ID, "qsf::getUninitialized<uint64>()" is there's no current ID
			*
			*  @remarks
			*    Usually the current ID is equal to the last selected ID. It's possible to explicitly set this current
			*    ID, although it's recommended to not do so if it can be avoided. Do also don't use the information about
			*    the current ID if you don't have to and use the general selected ID set whenever possible.
			*/
			inline uint64 getCurrentId() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::SelectionManagerTemplate methods ]
		//[-------------------------------------------------------]
		protected:
			inline virtual bool isSelectionValid(uint64 id) const override;
			virtual void onCurrentChange(uint64 id) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Entity deleted
			*
			*  @param[in] id
			*    The ID of the deleted entity
			*  @param[in] userId
			*    The ID of the user issuing the operation
			*/
			void entityDeleted(uint64 id, uint32 userId);

			/**
			*  @brief
			*    Entity created
			*
			*  @param[in] id
			*    The ID of the created entity
			*  @param[in] userId
			*    The ID of the user issuing the operation
			*/
			void entityCreated(uint64 id, uint32 userId);

			/**
			*  @brief
			*    Called as soon as an entity gets locked
			*
			*  @param[in] entityId
			*    ID of the locked entity
			*  @param[in] userId
			*    The ID of the user issuing the operation resulting in the lock
			*/
			void onEntityLocked(uint64 entityId, uint32 userId);

			/**
			*  @brief
			*    Called as soon as an entity gets unlocked
			*
			*  @param[in] entityId
			*    ID of the unlocked entity
			*  @param[in] userId
			*    The ID of the user issuing the operation resulting in the unlock
			*/
			void onEntityUnlocked(uint64 entityId, uint32 userId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef boost::container::flat_map<uint64, uint32> LockedEntitiesUserMap;	///< Key = entity ID, value = user ID


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EntitySelectionMarkerManager mEntitySelectionMarkerManager;	///< Entity selection marker manager instance
			const Map&					 mMap;							///< Map instance the entity selection manager is working on (cached instance for more efficient access)
			uint64						 mHighlightedEntity;			///< Highlighted entity, "qsf::getUninitialized<uint64><>()" if invalid
			LockedEntitiesSet			 mLockedEntitiesSet;			///< Set of currently locked entity IDs
			LockedEntitiesUserMap		 mLockedEntitiesUserMap;		///< Locked user IDs, key = entity ID, value = user ID


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::EntitySelectionManager)


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager-inl.h"
