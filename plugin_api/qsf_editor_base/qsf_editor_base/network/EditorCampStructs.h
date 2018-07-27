// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include "qsf_editor_base/network/NetworkTypes.h"

#include <qsf/asset/project/BaseProject.h>
#include <qsf/asset/project/BaseAssetPackage.h>
#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/reflection/type/CampUint64.h> // The 64 bit unsigned integer ID of the prototype class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type


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
			//[ Structs                                               ]
			//[-------------------------------------------------------]
			struct QSF_EDITOR_BASE_API_EXPORT EditorMapInfo
			{
				virtual inline ~EditorMapInfo() {};

				std::string   name;
				GlobalAssetId globalAssetId;

				QSF_CAMP_RTTI();
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorMapProjectInfo
			{
				virtual inline ~EditorMapProjectInfo() {};

				std::string name;
				ProjectId   projectId;
				std::vector<EditorMapInfo> maps;

				QSF_CAMP_RTTI();
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorMapProjectList
			{
				virtual inline ~EditorMapProjectList() {};

				std::vector<EditorMapProjectInfo> projects;

				QSF_CAMP_RTTI();
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorAssetPackageProperties : public BaseAssetPackage
			{
				inline EditorAssetPackageProperties()
				{
					// Nothing to do in here
				}

				inline EditorAssetPackageProperties(const BaseAssetPackage& r) :
					BaseAssetPackage(r)
				{
					// Nothing to do in here
				}

				inline EditorAssetPackageProperties& operator=(const BaseAssetPackage& r)
				{
					BaseAssetPackage::operator=(r);
					return *this;
				}

				virtual inline ~EditorAssetPackageProperties() {};

				QSF_CAMP_RTTI();
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorProjectProperties : public BaseProject
			{
				inline EditorProjectProperties()
				{
					// Nothing to do in here
				}

				inline EditorProjectProperties(const BaseProject& r) :
					BaseProject(r)
				{
					// Nothing to do in here
				}

				inline EditorProjectProperties& operator=(const BaseProject& r)
				{
					BaseProject::operator=(r);
					return *this;
				}

				virtual inline ~EditorProjectProperties() {};

				QSF_CAMP_RTTI();
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorProjectListContainer
			{
				virtual inline ~EditorProjectListContainer() {};

				std::vector<EditorProjectProperties> projectList;

				QSF_CAMP_RTTI()
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorAssetContainer
			{
				virtual inline ~EditorAssetContainer() {};

				GlobalAssetId globalAssetId;
				AssetRevision assetRevision;
				AssetRevision cachedAssetRevision;
				std::string dateCreated;

				QSF_CAMP_RTTI()
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorAssetPackageContainer : public BaseAssetPackage
			{
				inline EditorAssetPackageContainer()
				{
					// Nothing to do in here
				}

				inline EditorAssetPackageContainer(const EditorAssetPackageContainer& r) :
					BaseAssetPackage(r),
					assetList(r.assetList)
				{
					// Nothing to do in here
				}

				inline EditorAssetPackageContainer& operator=(const EditorAssetPackageContainer& r)
				{
					if (&r != this)
					{
						BaseAssetPackage::operator=(r);
						assetList = r.assetList;
					}
					return *this;
				}

				inline EditorAssetPackageContainer& operator=(const BaseAssetPackage& r)
				{
					BaseAssetPackage::operator=(r);
					return *this;
				}

				virtual inline ~EditorAssetPackageContainer() {};

				std::vector<EditorAssetContainer> assetList;

				QSF_CAMP_RTTI()
			};

			struct QSF_EDITOR_BASE_API_EXPORT EditorAssetPackageListContainer
			{
				inline EditorAssetPackageListContainer() :
					projectId(getUninitialized<ProjectId>())
				{};
				virtual inline ~EditorAssetPackageListContainer() {};

				ProjectId projectId;

				std::vector<EditorAssetPackageContainer> assetPackageList;

				QSF_CAMP_RTTI()
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
QSF_CAMP_TYPE(qsf::editor::base::EditorMapInfo);
QSF_CAMP_TYPE(qsf::editor::base::EditorMapProjectInfo);
QSF_CAMP_TYPE(qsf::editor::base::EditorMapProjectList);
QSF_CAMP_TYPE(qsf::editor::base::EditorAssetPackageProperties);
QSF_CAMP_TYPE(qsf::editor::base::EditorProjectProperties);
QSF_CAMP_TYPE(qsf::editor::base::EditorProjectListContainer);
QSF_CAMP_TYPE(qsf::editor::base::EditorAssetContainer);
QSF_CAMP_TYPE(qsf::editor::base::EditorAssetPackageContainer);
QSF_CAMP_TYPE(qsf::editor::base::EditorAssetPackageListContainer);
