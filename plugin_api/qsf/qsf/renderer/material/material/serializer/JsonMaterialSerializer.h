// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/forward/BoostPTree.h"
#include "qsf/renderer/material/material/serializer/MaterialSerializer.h"


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
	*    JSON material serializer
	*/
	class QSF_API_EXPORT JsonMaterialSerializer : public MaterialSerializer
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load materials
		*
		*  @param[in] materialManager
		*    Material manager to use
		*  @param[in] absoluteDirectory
		*    Absolute material directory
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		static void loadMaterials(MaterialManager& materialManager, const std::string& absoluteDirectory);

		static Material* loadMaterialFromPTree(MaterialManager& materialManager, const boost::property_tree::ptree& propertyTree, const std::string& materialName);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonMaterialSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonMaterialSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MaterialSerializer methods        ]
	//[-------------------------------------------------------]
	public:
		virtual Material* deserialize(MaterialManager& materialManager, const std::string& materialName, std::istream& istream) const override;
		virtual void serialize(const MaterialManager& materialManager, const std::string& materialName, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const std::string MATERIAL_FILE_EXTENSION;
		static const uint32		 MATERIAL_FORMAT_VERSION;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/serializer/JsonMaterialSerializer-inl.h"
