// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawRequest.h"
#include "qsf/debug/DebugTextParameters.h"
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
	*    Text debug draw request implementation
	*
	*  @todo
	*    - TODO(vs) Uses a probably non portable friend declaration to hide an ugly default constructor creating an illegal state temporarily when used inside a std::vector
	*/
	class QSF_API_EXPORT TextDebugDrawRequest : public DebugDrawRequest
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// Necessary for allowing std containers to use the illegal default constructor
		friend std::allocator<TextDebugDrawRequest>;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] text
		*    The UTF-8 encoded text
		*  @param[in] windowPosition
		*    The position where to draw the text in 2D window coordinates
		*  @param[in] textColor
		*    Draw color
		*/
		inline TextDebugDrawRequest(const std::string& text, const glm::vec2& windowPosition, const Color4& textColor = Color4::WHITE);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] text
		*    The UTF-8 encoded text
		*  @param[in] worldPosition
		*    The position where to draw the text in 3D world coordinates
		*  @param[in] textColor
		*    Draw color
		*/
		inline TextDebugDrawRequest(const std::string& text, const glm::vec3& worldPosition, const Color4& textColor = Color4::WHITE);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] text
		*    The UTF-8 encoded text
		*  @param[in] windowPosition
		*    The position where to draw the text in 2D window coordinates
		*  @param[in] debugTextParameters
		*    Debug text parameters with special formatting options
		*/
		inline TextDebugDrawRequest(const std::string& text, const glm::vec2& windowPosition, const DebugTextParameters& debugTextParameters);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] text
		*    The UTF-8 encoded text
		*  @param[in] worldPosition
		*    The position where to draw the text in 3D world coordinates
		*  @param[in] debugTextParameters
		*    Debug text parameters with special formatting options
		*/
		inline TextDebugDrawRequest(const std::string& text, const glm::vec3& worldPosition, const DebugTextParameters& debugTextParameters);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TextDebugDrawRequest();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::DebugDrawRequest methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void applyToObject(DebugDrawObject& debugDrawObject) const override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::string			mText;					///< The text to draw
		bool				mUsingWorldPosition;	///< If "true", position is given in world coordinates, else 2D window coordinates
		glm::vec3			mPosition;				///< Position of the text in the 3D world space or on the screen
		DebugTextParameters mTextParameters;		///< Parameters defining how the text is formatted


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/** Default constructor creating an illegal state that is only needed for using these requests inside a std::vector */
		inline TextDebugDrawRequest();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/request/TextDebugDrawRequest-inl.h"
