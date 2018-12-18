// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetPackage;
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
			*    Asset issue base class
			*/
			class QSF_EDITOR_BASE_API_EXPORT AssetIssue : public boost::noncopyable
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				enum Severity
				{
					NOTICE,
					WARNING,
					ERROR
				};


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				AssetIssue();

				explicit AssetIssue(const std::string& description);

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~AssetIssue();

				/**
				*  @brief
				*    Return the issue's asset global ID
				*
				*  @return
				*    The issue's asset global ID
				*/
				inline GlobalAssetId getGlobalAssetId() const;

				/**
				*  @brief
				*    Set the issue's asset global ID
				*
				*  @param[in] globalAssetId
				*    The issue's asset global ID
				*/
				void setGlobalAssetId(GlobalAssetId globalAssetId);

				/**
				*  @brief
				*    Return the issue's asset package
				*
				*  @return
				*    The issue's asset package
				*/
				inline AssetPackage* getAssetPackage() const;

				/**
				*  @brief
				*    Set the issue's asset package
				*
				*  @param[in] assetPackage
				*    The issue's asset package
				*/
				inline void setAssetPackage(AssetPackage* assetPackage);

				/**
				*  @brief
				*    Return the issue description
				*
				*  @return
				*    The issue description
				*/
				inline const std::string& getDescription() const;

				/**
				*  @brief
				*    Set the issue description
				*
				*  @param[in] description
				*    The new issue description
				*/
				inline void setDescription(const std::string& description);

				/**
				*  @brief
				*    Return the issue's related filename
				*
				*  @return
				*    The issue issue's related filename
				*/
				inline const std::string& getFilename() const;

				/**
				*  @brief
				*    Set the issue's related filename
				*
				*  @param[in] filename
				*    The issue's related filename
				*/
				inline void setFilename(const std::string& filename);

				/**
				*  @brief
				*    Return the issue's severity
				*
				*  @return
				*    The issue issue's severity
				*/
				inline Severity getSeverity() const;

				/**
				*  @brief
				*    Set the issue's severity
				*
				*  @param[in] severity
				*    The issue's severity
				*/
				inline void setSeverity(Severity severity);


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetIssue methods  ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Returns the issue identifier
				*
				*  @return
				*    The issue identifier
				*/
				virtual uint32 getIssueId() const = 0;

				/**
				*  @brief
				*    Returns the number of potential fixes available
				*
				*  @return
				*    The number of potential fixes available
				*/
				inline virtual uint32 countFixes() const;

				/**
				*  @brief
				*    Tells whether the given fix is applicable
				*
				*  @param[in] index
				*    Index of the fix to check
				*
				*  @return
				*    "true" if the fix can be applied, "false" otherwise
				*/
				inline virtual bool isFixApplicable(uint32 index) const;

				/**
				*  @brief
				*    Returns the given fix's description
				*
				*  @param[in] index
				*    Index of the fix
				*
				*  @return
				*    The given fix's description
				*/
				inline virtual const std::string& getFixDescription(uint32 index) const;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				GlobalAssetId	mGlobalAssetId;
				AssetPackage*	mAssetPackage;
				std::string		mDescription;
				std::string		mFilename;
				Severity		mSeverity;


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
#include "qsf_editor_base/asset/maintenance/AssetIssue-inl.h"
