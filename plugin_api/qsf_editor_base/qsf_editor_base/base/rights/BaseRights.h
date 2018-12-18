// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/base/rights/Right.h>
#include <qsf/base/rights/Rights.h>


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
			*    Editor base rights class
			*/
			class QSF_EDITOR_BASE_API_EXPORT BaseRights : public Rights
			{


			//[-------------------------------------------------------]
			//[ Qt definitions (MOC)                                  ]
			//[-------------------------------------------------------]
			// Empty fake Q_OBJECT macro to silence the lupdate Qt tool ("Class '<x>' lacks Q_OBJECT macro" warnings)
			#ifndef Q_OBJECT
				#define Q_OBJECT
				#define QSF_FAKE_Q_OBJECT
			#endif
			Q_OBJECT
			#ifdef QSF_FAKE_Q_OBJECT
				#undef Q_OBJECT
				#undef QSF_FAKE_Q_OBJECT
			#endif


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 ASSET_CREATE;								///< "asset.create" unique right ID
				static const uint32 ASSET_DELETE;								///< "asset.delete" unique right ID
				static const uint32 ASSET_EDIT;									///< "asset.edit" unique right ID
				static const uint32 ASSET_MAINTENANCE;							///< "asset.maintenance" unique right ID
				static const uint32 EDITOR_OPERATION;							///< "editor.operation" unique right ID
				static const uint32 EDITOR_PROJECT_CREATE;						///< "editor.project.create" unique right ID
				static const uint32 EDITOR_PROJECT_DESTROY;						///< "editor.project.destroy" unique right ID
				static const uint32 EDITOR_PROJECT_DORELEASE;					///< "editor.project.dorelease" unique right ID
				static const uint32 EDITOR_PROJECT_EDIT;						///< "editor.project.edit" unique right ID
				static const uint32 EDITOR_PROJECT_ASSETPACKAGE_CREATE;			///< "editor.project.assetpackage.create" unique right ID
				static const uint32 EDITOR_PROJECT_ASSETPACKAGE_DESTROY;		///< "editor.project.assetpackage.destroy" unique right ID
				static const uint32 EDITOR_PROJECT_ASSETPACKAGE_EDIT;			///< "editor.project.assetpackage.edit" unique right ID
				static const uint32 EDITOR_PROJECT_ASSETPACKAGE_ASSET_ADD;		///< "editor.project.assetpackage.asset.add" unique right ID
				static const uint32 EDITOR_PROJECT_ASSETPACKAGE_ASSET_REMOVE;	///< "editor.project.assetpackage.asset.remove" unique right ID
				static const uint32 SERVER_MAINTENANCE;							///< "server.maintenance" unique right ID
				static const uint32 SERVER_MANAGEMENT;							///< "server.management" unique right ID
				static const uint32 SERVER_USERS;								///< "server.users" unique right ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] rightsManager
				*    The rights manager this rights instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
				*/
				explicit BaseRights(RightsManager* rightsManager);

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~BaseRights();


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				Right mAssetCreateRight;						///< Asset create right
				Right mAssetDeleteRight;						///< Asset delete right
				Right mAssetEditRight;							///< Asset edit right
				Right mAssetMaintenanceRight;					///< Asset maintenance right
				Right mEditorOperationRight;					///< Editor operation right
				Right mEditorProjectCreate;						///< Editor project create right
				Right mEditorProjectDestroy;					///< Editor project destroy right
				Right mEditorProjectDoRelease;					///< Editor project do release right
				Right mEditorProjectEdit;						///< Editor project edit right
				Right mEditorProjectAssetPackageCreate;			///< Editor project asset package create right
				Right mEditorProjectAssetPackageDestroy;		///< Editor project asset package destroy right
				Right mEditorProjectAssetPackageEdit;			///< Editor project asset package edit right
				Right mEditorProjectAssetPackageAssetAdd;		///< Editor project asset package asset add right
				Right mEditorProjectAssetPackageAssetRemove;	///< Editor project asset package asset remove right
				Right mServerMaintenanceRight;					///< Server maintenance right
				Right mServerManagementRight;					///< Server management right
				Right mServerUsersRight;						///< Server users right


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/base/rights/BaseRights-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::BaseRights)
