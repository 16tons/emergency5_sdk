// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class DroneTransportLogic;
	class PersonComponent;
}


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
	*    EMERGENCY 5 SEK drone search action
	*/
	class EM5_API_EXPORT SEKDroneSearchAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;			///< "em5::SEKDroneSearchAction" unique action identifier
		static const qsf::Time TIME_BETWEEN_CLICKS;				///< Time between click sound effect
		static const qsf::Time TIME_DRONE_FLY_WITHOUT_TARGET;	///< Time the drone is searching for new targets till go back to transporter

		enum State
		{
			STATE_INIT,
			STATE_WAIT_FOR_START,
			STATE_SEARCH_GANGSTER,
			STATE_GOTO_GANGSTER,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SEKDroneSearchAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SEKDroneSearchAction();

		void init(qsf::Entity* droneTransporter);

		void searchNewTarget();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void showHint() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* getSearchTarget() const;
		bool isValidDroneTarget(PersonComponent* personComponent) const;
		void addListWithPriority(std::multimap<int, qsf::Entity*>& result, std::vector<qsf::Entity*>& list, int priority) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State						mState;
		qsf::WeakPtr<qsf::Entity>	mDroneTransporter;	///< Back pointer to the transporter of the drone, do not delete it
		DroneTransportLogic*		mDroneTransportLogic;
		qsf::WeakPtr<qsf::Entity>	mSearchTarget;
		qsf::Time					mSecondsPassedSinceClick;
		qsf::Time					mTimeWithoutTarget;
		bool						mNewTarget;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SEKDroneSearchAction)
