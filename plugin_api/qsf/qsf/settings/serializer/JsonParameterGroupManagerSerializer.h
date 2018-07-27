// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/settings/serializer/ParameterGroupManagerSerializer.h"
#include "qsf/platform/PlatformTypes.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}


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
	*    JSON parameter group manager serializer
	*/
	class JsonParameterGroupManagerSerializer : public ParameterGroupManagerSerializer
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
		*
		*  @param[in] formatTypeOverride
		*    If not a null pointer, this format type string is overriding the default format type required in the JSON
		*/
		inline explicit JsonParameterGroupManagerSerializer(const std::string* formatTypeOverride = nullptr);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~JsonParameterGroupManagerSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ParameterGroupManagerSerializer methods ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(ParameterGroupManager& parameterGroupManager, std::istream& istream) const override;
		virtual void serialize(ParameterGroupManager& parameterGroupManager, std::ostream& ostream, bool differenceToDefault = true) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load a JSON parameter group manager
		*
		*  @param[in] parameterGroupManager
		*    Parameter group manager instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadJson(ParameterGroupManager& parameterGroupManager, boost::property_tree::ptree& rootPTree) const;

		/**
		*  @brief
		*    Load a parameter group manager
		*
		*  @param[in] parameterGroupManager
		*    Parameter group manager instance to fill
		*  @param[in] rootPTree
		*    Boost root ptree
		*
		*  @exception
		*    Throws "std::exception" in case on an error
		*/
		void loadParameterGroupManager(ParameterGroupManager& parameterGroupManager, boost::property_tree::ptree& rootPTree) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mRequiredFormatType;	///< Format type string required in the JSON


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/settings/serializer/JsonParameterGroupManagerSerializer-inl.h"
