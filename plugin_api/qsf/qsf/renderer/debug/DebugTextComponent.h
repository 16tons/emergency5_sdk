// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/debug/DebugTextParameters.h"
#include "qsf/reflection/type/CampQsfColor4.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
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
	*    Debug text component class
	*/
	class QSF_API_EXPORT DebugTextComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::DebugTextComponent" unique component ID

		typedef DebugTextParameters::VerticalAlignment VerticalAlignment;		// Just a shortcut and for CAMP registration
		typedef DebugTextParameters::HorizontalAlignment HorizontalAlignment;	// Just a shortcut and for CAMP registration


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit DebugTextComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DebugTextComponent();

		/**
		*  @brief
		*    Get vertical alignment
		*/
		inline VerticalAlignment getVerticalAlignment() const;

		/**
		*  @brief
		*    Set vertical alignment
		*/
		inline void setVerticalAlignment(VerticalAlignment verticalAlignment);

		/**
		*  @brief
		*    Get horizontal alignment
		*/
		inline HorizontalAlignment getHorizontalAlignment() const;

		/**
		*  @brief
		*    Set horizontal alignment
		*/
		inline void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);

		/**
		*  @brief
		*    Get horizontal offset
		*  @note
		*    A positive value moves the text to the right, a negative to the left
		*/
		inline float getHorizontalOffset() const;

		/**
		*  @brief
		*    Set horizontal offset
		*/
		inline void setHorizontalOffset(float horizontalOffset);

		/**
		*  @brief
		*    Get vertical offset
		*/
		inline float getVerticalOffset() const;

		/**
		*  @brief
		*    Set vertical offset
		*  @note
		*    A positive value moves the text upword, a negative downword
		*/
		inline void setVerticalOffset(float verticalOffset);

		/**
		*  @brief
		*    Set 3d height offset
		*  @note
		*    A positive value moves the text upword, a negative downword
		*/
		inline float getHeightOffset();

		/**
		*  @brief
		*    Set 3d height offset
		*  @note
		*    A positive value moves the text upword, a negative downword
		*/
		inline void setHeightOffset(float heightOffset);

		/**
		*  @brief
		*    Get text color
		*/
		inline Color4 getTextColor() const;

		/**
		*  @brief
		*    Set text color
		*/
		inline void setTextColor(Color4 textColor);

		/**
		*  @brief
		*    Get font size
		*/
		inline float getFontSize() const;

		/**
		*  @brief
		*    Set font size
		*/
		inline void setFontSize(float fontSize);

		/*
		*  @breif
		*    Get if the text is shown in the editor
		*/
		inline bool getActive();

		/*
		*  @breif
		*    Set if the text is shown in the editor
		*/
		inline void setActive(bool active);

		/**
		*  @brief
		*    Set the text to show
		*/
		inline void setText(const std::string& text);

		/**
		*  @brief
		*    Get the text to show in the editor
		*/
		inline const std::string& getText() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetActive(bool active) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/*
		*  @breif
		*    Add the text draw request to the debug draw manager
		*/
		void addRequest(const TransformComponent& transformComponent);

		/**
		*  @brief
		*    Clear and add a new request to the debug draw manager
		*/
		void onPropertyChange();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mActive;			// Should the text be shown in the editor or not?
		std::string			mText;				// Shown text
		DebugTextParameters	mTextParameters;	// Parameters of the shown debug text
		DebugDrawProxy		mDebugDrawProxy;	// The debug draw proxy
		float				mHeightOffset;		// 3D height offset


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
#include "qsf/renderer/debug/DebugTextComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DebugTextComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DebugTextComponent::VerticalAlignment)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DebugTextComponent::HorizontalAlignment)
