// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DebugTextComponent::DebugTextComponent(Prototype* prototype) :
		Component(prototype),
		mActive(true),
		mText("<----Text---->"),
		mHeightOffset(0.0f)
	{
		// Set alignment on center
		mTextParameters.setHorizontalAlignment(DebugTextParameters::H_ALIGNMENT_CENTER);
		mTextParameters.setVerticalAlignment(DebugTextParameters::V_ALIGNMENT_CENTER);
	}

	inline DebugTextComponent::~DebugTextComponent()
	{
		// Nothing to do in here
	}

	inline DebugTextComponent::VerticalAlignment DebugTextComponent::getVerticalAlignment() const
	{
		return mTextParameters.getVerticalAlignment();
	}

	inline void DebugTextComponent::setVerticalAlignment(VerticalAlignment verticalAlignment)
	{
		mTextParameters.setVerticalAlignment(verticalAlignment);
		onPropertyChange();
	}

	inline DebugTextComponent::HorizontalAlignment DebugTextComponent::getHorizontalAlignment() const
	{
		return mTextParameters.getHorizontalAlignment();
	}

	inline void DebugTextComponent::setHorizontalAlignment(HorizontalAlignment horizontalAlignment)
	{
		mTextParameters.setHorizontalAlignment(horizontalAlignment);
		onPropertyChange();
	}

	inline float DebugTextComponent::getHorizontalOffset() const
	{
		return mTextParameters.getHorizontalOffset();
	}

	inline void DebugTextComponent::setHorizontalOffset(float horizontalOffset)
	{
		mTextParameters.setHorizontalOffset(horizontalOffset);
		onPropertyChange();
	}

	inline float DebugTextComponent::getVerticalOffset() const
	{
		return mTextParameters.getVerticalOffset();
	}

	inline void DebugTextComponent::setVerticalOffset(float verticalOffset)
	{
		mTextParameters.setVerticalOffset(verticalOffset);
		onPropertyChange();
	}

	inline float DebugTextComponent::getHeightOffset()
	{
		return mHeightOffset;
	}

	inline void DebugTextComponent::setHeightOffset(float heightOffset)
	{
		mHeightOffset = heightOffset;
		onPropertyChange();
	}

	inline Color4 DebugTextComponent::getTextColor() const
	{
		return mTextParameters.getTextColor();
	}

	inline void DebugTextComponent::setTextColor(Color4 textColor)
	{
		mTextParameters.setTextColor(textColor);
		onPropertyChange();
	}

	inline float DebugTextComponent::getFontSize() const
	{
		return mTextParameters.getFontSize();
	}

	inline void DebugTextComponent::setFontSize(float fontSize)
	{
		mTextParameters.setFontSize(fontSize);
		onPropertyChange();
	}

	inline void DebugTextComponent::setActive(bool active)
	{
		mActive = active;
		onPropertyChange();
	}

	inline bool DebugTextComponent::getActive()
	{
		return mActive;
	}

	inline void DebugTextComponent::setText(const std::string& text)
	{
		mText = text;
		onPropertyChange();
	}

	inline const std::string& DebugTextComponent::getText() const
	{
		return mText;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
