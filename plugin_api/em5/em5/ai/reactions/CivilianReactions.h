// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/decision/reactions/Reaction.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	class CivilianReactionStopAndGape : public qsf::ai::Reaction
	{
	public:
		CLONEABLE_REACTION(em5::CivilianReactionStopAndGape)
		CivilianReactionStopAndGape();
		virtual bool handle(const qsf::ai::EvaluationParameters& parameters, const qsf::ai::PerceptionEvent* event);
	};

	class CivilianReactionStopAndPhone : public qsf::ai::Reaction
	{
	public:
		CLONEABLE_REACTION(em5::CivilianReactionStopAndPhone)
		CivilianReactionStopAndPhone();
		virtual bool handle(const qsf::ai::EvaluationParameters& parameters, const qsf::ai::PerceptionEvent* event);
	private:
		qsf::ai::MinMaxDuration mDuration;
	};

	class CivilianReactionUmbrella : public qsf::ai::Reaction
	{
	public:
		CLONEABLE_REACTION(em5::CivilianReactionUmbrella)
		CivilianReactionUmbrella();
		virtual bool handle(const qsf::ai::EvaluationParameters& parameters, const qsf::ai::PerceptionEvent* event);
	};

	class CivilianReactionRun : public qsf::ai::Reaction
	{
	public:
		CLONEABLE_REACTION(em5::CivilianReactionRun)
		CivilianReactionRun();
		virtual bool handle(const qsf::ai::EvaluationParameters& parameters, const qsf::ai::PerceptionEvent* event);
	};

	class CivilianReactionTalkTo : public qsf::ai::Reaction
	{
	public:
		CLONEABLE_REACTION(em5::CivilianReactionTalkTo)
		CivilianReactionTalkTo();
		virtual bool handle(const qsf::ai::EvaluationParameters& parameters, const qsf::ai::PerceptionEvent* event);
		void cancel(const qsf::ai::EvaluationParameters& parameters);
	private:
		qsf::ai::MinMaxDuration mDuration;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
