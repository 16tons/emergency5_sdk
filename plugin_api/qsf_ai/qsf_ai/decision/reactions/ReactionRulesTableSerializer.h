// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/Serializer.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		class ReactionRulesTable;

		/** Class to (de)serialize reaction tables used by the ReactionComponent.
		*/
		class ReactionRulesTableSerializer : public Serializer
		{
		public:
			static void deserialize(ReactionRulesTable& reactionTable,const std::string& filename,bool clearTableBeforeDeserialize);
			static void serialize(const ReactionRulesTable& reactionTable,const std::string& filename);
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
