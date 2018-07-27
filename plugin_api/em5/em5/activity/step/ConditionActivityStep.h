// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/activity/step/ActivityStep.h"

#include <qsf/base/PropertyDictionary.h>


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
	*    Condition activity step class
	*/
	class EM5_API_EXPORT ConditionActivityStep : public ActivityStep
	{


	//[-------------------------------------------------------]
	//[ Public virtual em5::ActivityStep methods              ]
	//[-------------------------------------------------------]
	public:
		virtual bool loadFromBoostPTree(const boost::property_tree::ptree& rootPTree) override;
		virtual Result::Type execute(const Parameters& parameters, Result& result) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Condition
		{
			qsf::NamedIdentifier    mType;
			qsf::PropertyDictionary mParameters;
		};
		typedef std::vector<Condition> ConditionArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ConditionArray mConditions;
		std::string	   mNextStateOnTrueCondition;
		std::string	   mNextStateOnFalseCondition;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ConditionActivityStep)
