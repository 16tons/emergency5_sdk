// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace ai
	{
		class EActor;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		 *   @brief
		 *     Actor factory interface definition
		 *
		 *   @remarks
		 *     An actor factory constructs EActor or subclass instances from QSF entities.
		 *     The exact subclass is determined by the entity's components.
		 */
		class ActorFactory
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			ActorFactory() {}
			virtual ~ActorFactory() {}

			virtual EActor* constructActor(Entity& entity) = 0;
			virtual void destructActor(EActor* actor) = 0;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
