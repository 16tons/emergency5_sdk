// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/rules/StandardRule.h"
#include "qsf_ai/decision/reactions/ReactionHelpers.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace reactionrules
		{


			/** Base reaction-rule class for random interuptions.
			* Derived classes implement a certain reaction by
			* overriding onInteruption.
			*
			* Sensor-Configuration:
			* No sensor required.
			*
			* JSON configuration tags:
			* ImpulsIn, NoImpulsFor
			*/
			class QSF_AI_API_EXPORT RandomInteruption : public StandardRule
			{
				// Tag string IDs used by the JSON serializer.
				//@{
				static const char* IMPULS_IN_TAG;
				static const char* NO_IMPULS_FOR_TAG;
				//@}
			public:
				RandomInteruption();
				virtual void serialize(BinarySerializer& serializer);
				virtual void deserialize(const boost::property_tree::ptree& tree);
				virtual void serialize(boost::property_tree::ptree& tree) const;
				virtual bool handle(const EvaluationParameters& parameters);

			protected:
				virtual bool onInteruption(const EvaluationParameters& parameters);

			protected:
				Time mLastCallTime;
				float mFireImpulsAt;
				UnsignedFloat mImpulsIn;
				UnsignedFloat mNoImpulsFor;
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf
