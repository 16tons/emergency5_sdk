// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionHelpers.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		#define CLONEABLE_REACTION(ClassName) \
			virtual Reaction* clone() const {return new ClassName(*this);}\
			virtual const char* getRegistrationName() const {return #ClassName ;}


		/** Base class for AI reactions.
		* Although the reaction behaviour can be implemented in the reaction-rule itself
		* it is sometimes desirable to have it encapsulated into this dedicated reaction-class,
		* especially if the reaction is one out of many to be chosen from a ReactionPool.
		*/
		class QSF_AI_API_EXPORT Reaction
		{
			friend class ReactionSystem;

			// Tag string IDs used by the JSON serializer.
			//@{
			static const char* PROBABILITY_TAG;
			//@}
		public:
			Reaction();
			virtual ~Reaction();
			virtual Reaction* clone() const=0;

			// Return the name by which this reaction shall be registered inside the ReactionSystem.
			virtual const char* getRegistrationName() const=0;

			// Read-only access to the Reaction's id.
			//@{
			const std::string& getName() const;
			uint32 getId() const; // string hash of the ReactionRule's name.
			//@}

			// get / set the probability at which this reaction is going to be chosen
			// if stored inside a ReactionPool.
			//@{
			float getProbability() const;
			void setProbability(float probability);
			//@}

			// Implement the actual reaction code inside this method.
			virtual bool handle(const EvaluationParameters& parameters,const PerceptionEvent* event)=0;

			virtual void serialize(BinarySerializer& serializer);
			virtual void deserialize(const boost::property_tree::ptree& tree);
			virtual void serialize(boost::property_tree::ptree& tree) const;

			// Used for sorting the reactions by probability if stored inside a ReactionPool.
			static bool compareByProbability(const Reaction* a,const Reaction* b);

		private:
			// only accessable to the ReactionSystem, which is a friend of this class for that purpose.
			void setName(const char* name);

		private:
			NamedIdentifier mReactionId;
		protected:
			float mProbability;
		};

		/** Reaction variant which adds support for automatic
		* delayed actions. Especially useful for for example weather-reactions
		* when you don't want all entities to react at the very same moment.
		*/
		class DelayedReaction : public Reaction
		{
		public:
			DelayedReaction();
			virtual void deserialize(const boost::property_tree::ptree& tree);
			virtual void serialize(boost::property_tree::ptree& tree) const;
		protected:
			MinMaxDelay mDelay;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/Reaction-inl.h"
