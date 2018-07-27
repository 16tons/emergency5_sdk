// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest.h"
#include "qsf/math/Color4.h"

#include <glm/glm.hpp>

#include <string>


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
	*    Icon debug draw request implementation
	*/
	class QSF_API_EXPORT ImageDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<ImageDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] topLeftPosition
		*    Top left position of the image, in pixels
		*  @param[in] size
		*    Size of the image to draw in pixels
		*  @param[in] textureLocalAssetName
		*    Local asset name of the texture to draw as image
		*  @param[in] tintColor
		*    Color multiplied with the color map
		*/
		inline ImageDebugDrawRequest(const glm::vec2& topLeftPosition, const glm::vec2& size, const std::string& textureLocalAssetName, const Color4& tintColor = Color4::WHITE);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] worldPosition
		*    Position of the image in world space
		*  @param[in] size
		*    Size of the image to draw in pixels
		*  @param[in] textureLocalAssetName
		*    Local asset name of the texture to draw as image
		*  @param[in] tintColor
		*    Color multiplied with the color map
		*
		*  @note
		*    Don't forget to call DebugDrawProxy::setDrawInClipSpace(true); before
		*/
		inline ImageDebugDrawRequest(const glm::vec3& windowPosition, const glm::vec2& size, const std::string& textureLocalAssetName, const Color4& tintColor = Color4::WHITE);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ImageDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		std::string getOrCreateMaterial() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec2	mPosition;				///< Position in clip space
		glm::vec3	mWorldPosition;			///< Position in world space
		glm::vec2	mSize;					///< Image size in window percentage
		bool		mUseWorldSpace;			///< Use world space coordinates?
		Color4		mTintColor;				///< Color to tint the graphics, white by default
		std::string	mTextureLocalAssetName;	///< Name of the image


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/ImageDebugDrawRequest-inl.h"
