// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/forward/BoostPTree.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/time/Time.h>
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
	*    Activity step base class
	*/
	class EM5_API_EXPORT ActivityStep
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Parameters
		{
			qsf::Entity& mEntity;
			bool		 mFirstUpdate;
			qsf::Time	 mTotalTime;
			qsf::Time	 mTimePassed;

			Parameters(qsf::Entity& entity) : mEntity(entity), mFirstUpdate(true) {}
		};

		struct Result
		{
			enum Type
			{
				CONTINUE,
				NEXT_STEP,
				SWITCH_STATE,
				STOP
			};

			qsf::NamedIdentifier mNextState;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		virtual ~ActivityStep()  {}
		inline const std::string& getType() const  { return mType; }

		bool loadFromBoostPTree(const std::string& type, const boost::property_tree::ptree& rootPTree)
		{
			mType = type;
			return loadFromBoostPTree(rootPTree);
		}


	//[-------------------------------------------------------]
	//[ Public virtual em5::ActivityStep methods              ]
	//[-------------------------------------------------------]
	public:
		virtual bool loadFromBoostPTree(const boost::property_tree::ptree& rootPTree) = 0;
		virtual Result::Type execute(const Parameters& parameters, Result& result) = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mType;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ActivityStep)
