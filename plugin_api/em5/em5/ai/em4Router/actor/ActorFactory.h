// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/navigation/em4Router/wrapper/actor/ActorFactory.h>


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
	*    Emergency 5 actor factory implementation
	*
	*  @remarks
	*    An actor factory constructs EActor or subclass instances from QSF entities.
	*    The exact subclass is determined by the entity's components.
	*/
	class ActorFactory : public qsf::ai::ActorFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ActorFactory();
		~ActorFactory();

		virtual qsf::ai::EActor* constructActor(qsf::Entity& entity) override;
		virtual void destructActor(qsf::ai::EActor* actor) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		struct Internal;
		Internal& mInternal;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
