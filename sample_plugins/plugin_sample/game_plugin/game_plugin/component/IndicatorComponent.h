// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/Export.h"

#include <qsf/component/Component.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Indicator component
	*
	*  @remarks
	*    This is a sample component that can be added to entities inside the editor.
	*    It creates a colorful box around the entity.
	*/
	class GAMEPLUGIN_API_EXPORT IndicatorComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "user::IndicatorComponent" unique component ID

		// Predefined colors
		enum Color
		{
			WHITE  = 0xffffff,
			RED    = 0xffff00,
			YELLOW = 0xff0000,
			GREEN  = 0x00ff00,
			BLUE   = 0x0000ff
		};


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
		IndicatorComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		~IndicatorComponent();

		// Getter and setter for "Color" property
		inline Color getColor() const;
		void setColor(Color color);

		void setRandomColor();

		// Getter and setter for "IsAnimated" property
		inline bool isAnimated() const;
		void setIsAnimated(bool animated);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments& jobArguments);
		void updateVisualization(bool onlyPosition = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Color				mColor;				///< Selected color of this indicator
		bool				mIsAnimated;		///< If "true", the indicator is animated
		qsf::Time			mAnimationTime;		///< Current time for animation

		qsf::JobProxy		mUpdateJobProxy;	///< Update job proxy, needed to get regular update calls
		qsf::DebugDrawProxy mDebugDrawProxy;	///< Debug draw proxy, needed for the visual representation


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "game_plugin/component/IndicatorComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(user::IndicatorComponent::Color)
QSF_CAMP_TYPE_NONCOPYABLE(user::IndicatorComponent)
