// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CollisionTypeManager;
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
	*    Physics system class
	*/
	class QSF_API_EXPORT PhysicsSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline PhysicsSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsSystem();

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the collision type manager instance
		*
		*  @return
		*    The collision type manager instance, do no destroy the returned instance
		*/
		CollisionTypeManager& getCollisionTypeManager() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CollisionTypeManager* mCollisionTypeManager;	///< Collision type manager instance, always valid if the system is running, destroy it in case you no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/PhysicsSystem-inl.h"
