// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/selection/SelectionMarkerManager.h>

#include <QtCore/qobject.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
	class ParameterGroup;
	class DebugDrawManager;
	class GeneralDebugGroup;
	namespace editor
	{
		class RenderingSettingsGroup;
		class EntitySelectionManager;
		namespace base
		{
			class Operation;
			class UserManager;
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
		*    QSF editor entity selection marker manager class that cares for activation and deactivation of markers
		*/
		class EntitySelectionMarkerManager : public QObject, public SelectionMarkerManager
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entitySelectionManager
			*    QSF editor entity selection manager to spy on, must stay valid as long as this selection marker manager instance exists
			*/
			explicit EntitySelectionMarkerManager(EntitySelectionManager& entitySelectionManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EntitySelectionMarkerManager();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// qsf::editor::EntitySelectionManager
			void onHighlightedEntityChanged(Entity* entity);
			void onEntityLocked(uint64 entityId, uint32 userId);
			void onEntityUnlocked(uint64 entityId, uint32 userId);
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);
			void onCoupledToServer();
			void onDecoupledFromServer();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onSelected(uint64 id);
			void onDeselected(uint64 id);

			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef boost::container::flat_map<uint64, unsigned int> LockedEntitiesMap;	///< Key = entity ID, value = debug draw request ID


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			base::UserManager&			  mUserManager;				///< User manager instance for fast access, do not destroy the instance
			EntitySelectionManager&		  mEntitySelectionManager;	///< QSF editor entity selection manager to spy on, always valid, do not destroy the instance
			DebugDrawManager&			  mDebugDrawManager;		///< Debug draw manager instance, do not destroy the instance
			Map&						  mMap;						///< Map instance the entity selection marker manager is working on
			const RenderingSettingsGroup& mRenderingSettingsGroup;	///< Rendering settings group instance (cached instance for more efficient access)
			const GeneralDebugGroup&	  mGeneralDebugGroup;		///< General debug group instance (cached instance for more efficient access)
			unsigned int				  mDebugDrawRequestId;		///< Debug draw request ID
			LockedEntitiesMap			  mLockedEntitiesMap;		///< Locked debug draw request IDs, key = entity ID, value = debug draw request ID


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
