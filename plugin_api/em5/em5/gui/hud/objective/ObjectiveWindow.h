// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/helper/BlinkButton.h"
#include "em5/gui/hud/objective/DefaultObjectiveWindowDataSource.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/audio/AudioProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Objective window
	*/
	class EM5_API_EXPORT ObjectiveWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ObjectiveWindow(qsf::GuiContext& guiContext);
		virtual ~ObjectiveWindow();

		/**
		*  @brief
		*    Enable an button
		*
		*  @param[in] state
		*    Indicates if the button should enabled or not
		*/
		void enableButton(bool state);

		/**
		*  @brief
		*    Let an button blink
		*
		*  @param[in] blink
		*    Indicates if the button should blink or not
		*/
		void blinkButton(bool blink);

		/**
		*  @brief
		*    Removes any shown objectives and events
		*/
		void clear();

		/**
		*  @brief
		*    Return the default objective window data source
		*
		*  @return
		*    The default objective window data source
		*/
		const DefaultObjectiveWindowDataSource& getDefaultObjectiveWindowDataSource() const;

		/**
		*  @brief
		*    Set objective window data source
		*
		*  @param[in] objectiveWindowDataSource
		*    Objective window data source to use, if null pointer the EMERGENCY 5 default implementation will be used, the instance must stay valid as long as it's referenced (the objective window doesn't take over the memory ownership)
		*/
		void setObjectiveWindowDataSource(ObjectiveWindowDataSource* objectiveWindowDataSource);

		/**
		*  @brief
		*    Trigger GUI refresh
		*/
		void triggerRefresh();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Message listener callbacks
		void onUpdateEvents(const qsf::MessageParameters& parameters);
		void onNewEvent(const qsf::MessageParameters& parameters);
		void onEventFinish(const qsf::MessageParameters& parameters);
		void onEventFinishFailed(const qsf::MessageParameters& parameters);
		void onObjectiveGivePoints(const qsf::MessageParameters& parameters);
		void onEventFinishPoints(const qsf::MessageParameters& parameters);

		// Job updates
		void onBlink(const qsf::JobArguments& jobArguments);
		void onSignalPointAdded(const qsf::JobArguments& jobArguments);
		void onSignalObjectivePointAdded(const qsf::JobArguments& jobArguments);

		void refreshObjectiveView();
		void showPointSignal(bool show);
		bool updatePointsAnimation(Rocket::Core::Element& points, Rocket::Core::Element& background, float& animationTime, float secondsElapsed);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct ClosedEventData
		{
			Rocket::Core::Element*	element;		///< Holds the close event objective window element
			float					animationTime;	///< Holds the past animation time
			bool					eventSuccess;	///< Set true if the event success
		};
		struct ObjectivePointContainer
		{
			Rocket::Core::Element*	element;		///< Holds the objective point container element
			bool					occupied;		///< true if the container occupied
			float					animationTime;	///< Holds the past animation time
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Message proxies
		qsf::MessageProxy mMessageProxy;
		qsf::MessageProxy mMessageProxyNewEvent;
		qsf::MessageProxy mMessageProxyEventSucceeded;
		qsf::MessageProxy mMessageProxyEventFailed;
		qsf::MessageProxy mMessageProxyObjectivGivePoints;
		qsf::MessageProxy mMessageProxyEventPoints;

		// Job proxies
		qsf::JobProxy mAnimationJobProxy;
		qsf::JobProxy mAnimationSignalPointsProxy;
		qsf::JobProxy mAnimationSignalObjectivePointsProxy;
		qsf::JobProxy mDelayedUpdateJobProxy;

		// Other
		DefaultObjectiveWindowDataSource	 mDefaultObjectiveWindowDataSource;	///< EMERGENCY 5 objective window data source default implementation which holds and manages the live states
		const ObjectiveWindowDataSource*	 mObjectiveWindowDataSource;		///< Objective window data source used for visual GUI representation, always valid, don't destroy the instance since we don't own it
		std::vector<ObjectivePointContainer> mObjectivePointsContainer;
		BlinkButton							 mBlinkButton;
		std::vector<ClosedEventData>		 mClosedEventList;
		std::vector<uint32>					 mAddedObjectivePoints;
		std::vector<uint32>					 mAddedEventPoints;
		float								 mAddedPointAnimationTime;
		bool								 mEventSuccess;
		qsf::AudioProxy						 mAudioProxy;
		bool								 mDontRemoveObjective;
		bool								 mIsMainEvent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
