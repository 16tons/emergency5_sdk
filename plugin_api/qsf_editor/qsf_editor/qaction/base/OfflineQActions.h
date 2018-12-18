// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
		*    Offline QActions category
		*/
		class OfflineQActions : public QActions
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;		///< "qsf::editor::OfflineQActions" unique pluginable QActions ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QtActions manager this QtActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit OfflineQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OfflineQActions();

			/**
			*  @brief
			*    Avoid the unwanted loss of map data by involving the user if required
			*
			*  @return
			*    "true" if all went fine and map data can now be manipulated without data loss, else "false"
			*
			*  @note
			*    - In case we're currently in online mode, this method will always return "true"
			*/
			bool avoidMapDataLoss();

			//[-------------------------------------------------------]
			//[ QAction                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the new map Qt action
			*
			*  @return
			*    The new map Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getNewMapQAction() const;

			/**
			*  @brief
			*    Return the load map Qt action
			*
			*  @return
			*    The load map Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getLoadMapQAction() const;

			/**
			*  @brief
			*    Return the save map Qt action
			*
			*  @return
			*    The save map Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSaveMapQAction() const;

			/**
			*  @brief
			*    Add recent file Qt actions to the given Qt menu
			*
			*  @param[in] qMenu
			*    Qt menu to add the recent file Qt actions to
			*/
			void addRecentFileQActionsToQMenu(QMenu& qMenu) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		private:
			static const unsigned int MAXIMUM_NUMBER_OF_RECENT_FILES = 10;	///< Maximum number of recent map files


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredNewMap();
			void onTriggeredLoadMap();
			void onTriggeredSaveMap();
			void onTriggeredRecentFile();
			void onCleanChanged(bool clean);
			void onPostMapRebuild();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Load a QSF map
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the QSF map to load
			*  @param[in] newMapMode
			*    New map mode? (emit pre/post map loading signals?)
			*/
			void loadMap(GlobalAssetId globalAssetId, bool newMapMode = false);

			/**
			*  @brief
			*    Save a QSF map
			*
			*  @param[in] filename
			*    Filename to save the current QSF map under
			*/
			void saveMap(const QString& filename);

			/**
			*  @brief
			*    Finalize new map import
			*
			*  @param[in] newAsset
			*    Information about the new asset
			*/
			void finalizeNewMapImport(const Asset& newAsset);

			/**
			*  @brief
			*    Set the recent map file
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the recent QSF map
			*/
			void setRecentFile(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Update the recent map files Qt actions
			*/
			void updateRecentFileActions();

			/**
			*  @brief
			*    Get a consistent file list
			*
			*  @param[in] fileList
			*    File list to get conistent
			*
			*  @return
			*    Consistent file list
			*/
			QStringList getConsistentFileList(const QStringList& fileList) const;

			/**
			*  @brief
			*    Updates the enabled-state of the save map action
			*/
			void updateSaveMapActionState();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map&	 mMap;													///< Map instance to operate on, do not destroy the instance
			QAction* mNewMapQAction;										///< New map Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mLoadMapQAction;										///< Load map Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSaveMapQAction;										///< Save map Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mQActionRecentFiles[MAXIMUM_NUMBER_OF_RECENT_FILES];	///< Recent map files Qt actions, always valid, Qt will destroy the instances automatically


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/base/OfflineQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::OfflineQActions)
