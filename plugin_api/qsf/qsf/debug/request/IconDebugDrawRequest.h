// Copyright (C) 2012-2018 Promotion Software GmbH


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
	class QSF_API_EXPORT IconDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<IconDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] topLeftPosition
		*    Top left position of the icon
		*  @param[in] iconName
		*    Icon local asset name
		*  @param[in] tintColor
		*    Color multiplied with the icon color map
		*  @param[in] width
		*    Icon width, null by default; if width <= 0, original icon size used
		*  @param[in] height
		*    Icon height, null by default; if height <= 0, original icon size used
		*/
		inline IconDebugDrawRequest(const glm::vec3& topLeftPosition, const std::string& iconName, const Color4& tintColor = Color4::WHITE, float width = 1.0f, float height = 1.0f);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~IconDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3	mPosition;
		std::string	mIconName;
		Color4		mTintColor;	///< Color to tint the graphics, white by default
		float		mWidth;
		float		mHeight;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/IconDebugDrawRequest-inl.h"
