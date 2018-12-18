// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Color4.h"

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
	*    Parameter class for the text debug draw request
	*/
	class QSF_API_EXPORT DebugTextParameters
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::string ColorTexture;

		/** Vertical alignment  */
		enum VerticalAlignment
		{
			V_ALIGNMENT_TOP = 0,
			V_ALIGNMENT_CENTER,
			V_ALIGNMENT_BOTTOM
		};

		/** Horizontal alignment  */
		enum HorizontalAlignment
		{
			H_ALIGNMENT_LEFT = 0,
			H_ALIGNMENT_CENTER,
			H_ALIGNMENT_RIGHT
		};


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const ColorTexture COLOR_NONE;	// Value: "" (empty string)
		static const ColorTexture COLOR_BLACK;	// Value: "qsf/texture/debug/transparent_black.dds"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DebugTextParameters();

		/**
		*  @brief
		*    Destructor
		*/
		~DebugTextParameters();

		/**
		*  @brief
		*    Get vertical alignment
		*/
		VerticalAlignment getVerticalAlignment() const;

		/**
		*  @brief
		*    Set vertical alignment
		*/
		void setVerticalAlignment(VerticalAlignment verticalAlignment);

		/**
		*  @brief
		*    Get horizontal alignment
		*/
		HorizontalAlignment getHorizontalAlignment() const;

		/**
		*  @brief
		*    Set horizontal alignment
		*/
		void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);

		/**
		*  @brief
		*    Get horizontal offset
		*
		*  @note
		*    - A positive value moves the text to the right, a negative to the left
		*/
		float getHorizontalOffset() const;

		/**
		*  @brief
		*    Set horizontal offset
		*/
		void setHorizontalOffset(float horizontalOffset);

		/**
		*  @brief
		*    Get vertical offset
		*/
		float getVerticalOffset() const;

		/**
		*  @brief
		*    Set vertical offset
		*
		*  @note
		*    - A positive value moves the text upword, a negative downword
		*/
		void setVerticalOffset(float verticalOffset);

		/**
		*  @brief
		*    Get text color
		*/
		Color4 getTextColor() const;

		/**
		*  @brief
		*    Set text color
		*/
		void setTextColor(const Color4& textColor);

		/**
		*  @brief
		*    Get font size
		*/
		float getFontSize() const;

		/**
		*  @brief
		*    Set font size
		*/
		void setFontSize(float fontSize);

		/**
		*  @brief
		*    Get panel color
		*/
		const ColorTexture& getPanelColor() const;

		/**
		*  @brief
		*    Set panel color
		*/
		void setPanelColor(const ColorTexture& panelColor);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		VerticalAlignment	mVerticalAlignment;
		HorizontalAlignment	mHorizontalAlignment;
		float				mHorizontalOffset;
		float				mVerticalOffset;
		Color4				mTextColor;
		float				mFontSize;
		ColorTexture		mPanelColor;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
