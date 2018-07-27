// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/action/Action.h"
#include "qsf/reflection/type/CampQsfTime.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PropertyDictionary;
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
	*    Wait action class
	*/
	class QSF_API_EXPORT WaitAction : public Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const NamedIdentifier ACTION_ID;		///< "qsf::WaitAction" unique action type identifier
		static const NamedIdentifier TIME_PROPERTY;	///< "time" property identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		WaitAction();

		/**
		*  @brief
		*    Generic constructor from user defined parameters
		*/
		WaitAction(PropertyDictionary* parameters);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WaitAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const Time& time);

		/**
		*  @brief
		*    Update remaining wait time after initialization
		*/
		void setWaitTime(const Time& time);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Serialize or deserialize the action using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Update the action
		*
		*  @param clock
		*    The clock used for timing the action updates; holds e.g. time passed since last update
		*  @return
		*    The result of the action update
		*/
		virtual action::Result updateAction(const Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Time mTimeRemaining;	///< Remaining time to wait


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::WaitAction)
