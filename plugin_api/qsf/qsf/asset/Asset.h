// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/BaseAsset.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Project;
	class AssetPackage;
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
	*    Asset class
	*/
	class QSF_API_EXPORT Asset : public BaseAsset
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] globalAssetId
		*    The global asset identifier
		*/
		explicit Asset(GlobalAssetId globalAssetId);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Asset();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] source
		*    Source to copy from
		*
		*  @return
		*    Reference to this base asset
		*/
		inline Asset& operator =(const Asset& source);

		/**
		*  @brief
		*    Copy method excluding GlobalAssetId
		*
		*  @param[in] source
		*    Source to copy from
		*
		*  @return
		*    Reference to this base asset
		*/
		inline Asset& copyPropertiesFrom(const Asset& source);

		/**
		*  @brief
		*    Return the asset priority
		*
		*  @return
		*    The asset priority
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getPriority() const;

		/**
		*  @brief
		*    Set the asset priority
		*
		*  @param[in] priority
		*    The new asset priority
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setPriority(float priority);

		/**
		*  @brief
		*    Construct the local asset name inside a given asset package
		*
		*  @param[in] assetPackage
		*    The asset package the asset is considered to be in
		*
		*  @return
		*    The local UTF-8 asset name inside a given asset package
		*
		*  @note
		*    - Example: "sample/mesh/house/house01", "<project name>/<asset type>/<asset category>/<asset name>"
		*/
		std::string getLocalAssetNameInAssetPackage(const AssetPackage& assetPackage) const;

		/**
		*  @brief
		*    Generate an relative filename inside the given asset package optionally using the provided file extension
		*
		*  @param[in] assetPackage
		*    Asset package the asset is considered to be in
		*  @param[in] fileExtension
		*    File extension that should be appended to the relative filename. (optional; A dot (".") will automatically prepended if the provided file extension does not begin with one)
		*
		*  @return
		*    The generated relative UTF-8 filename inside the given asset package, empty string if the asset package does not have its filename set yet (was never saved/loaded)
		*/
		std::string generateRelativeFilename(const AssetPackage& assetPackage, const std::string& fileExtension = "") const;


	//[-------------------------------------------------------]
	//[ Public virtual "qsf::BaseAsset" methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer, uint32 version) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float mPriority;	///< The asset priority (for queue ordering on client)


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/asset/Asset-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Asset)
