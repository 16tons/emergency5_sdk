// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/project/serializer/ProjectSerializer.h"
#include "qsf/forward/BoostPTree.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    JSON project serializer
	*/
	class JsonProjectSerializer : public ProjectSerializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char	FORMAT_TYPE[];
		static const uint32 FORMAT_VERSION;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonProjectSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonProjectSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ProjectSerializer methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(Project& project, std::istream& istream) const override;
		virtual void serialize(const Project& project, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load a JSON project
		*
		*  @param[in] project
		*    Project instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadJson(Project& project, boost::property_tree::ptree& rootPTree) const;

		/**
		*  @brief
		*    Load a project
		*
		*  @param[in] project
		*    Project instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadProject(Project& project, boost::property_tree::ptree& rootPTree) const;

		/**
		*  @brief
		*    Load project dependencies
		*
		*  @param[in] project
		*    Project instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadProjectDependencies(Project& project, boost::property_tree::ptree& rootPTree) const;

		/**
		*  @brief
		*    Load project asset packages
		*
		*  @param[in] project
		*    Project instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadProjectAssetPackages(Project& project, boost::property_tree::ptree& rootPTree) const;

		void loadProjectPlugins(Project& project, boost::property_tree::ptree& rootPTree) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/project/serializer/JsonProjectSerializer-inl.h"
