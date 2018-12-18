// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/menu/asset/AssetMenu.h"

#include <memory>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class AssetIssue;
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
		*    Asset issue menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class AssetIssueMenu : public AssetMenu
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Qt signals (MOC)                                      ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			void appliedFix(std::shared_ptr<base::AssetIssue> assetIssue, uint32 fixIndex, bool isSuccess);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetIssue
			*    The asset issue this menu should operate on
			*/
			explicit AssetIssueMenu(std::shared_ptr<base::AssetIssue> assetIssue);

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] assetIssues
			*    The asset issues this menu should operate on (make sure they all have the same type or the "resolve by"-actions will not be shown)
			*/
			explicit AssetIssueMenu(const std::vector<std::shared_ptr<base::AssetIssue>>& assetIssues);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetIssueMenu();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::AssetMenu methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual std::string getAbsoluteCachedAssetFilename(size_t index) const override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredResolveBy();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void buildMenu();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<std::shared_ptr<base::AssetIssue>> mAssetIssues;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
