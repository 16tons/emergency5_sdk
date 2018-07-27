// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/handler/InputHandler.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class TransformComponent;
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
	*    Entity input handler base class
	*/
	class QSF_API_EXPORT EntityInputHandler : public InputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] id
		*    Input handler ID
		*/
		inline explicit EntityInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~EntityInputHandler();

		/**
		*  @brief
		*    Set target entity that will be directly controlled by this input handler
		*/
		void setEntityById(uint64 entityId);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::EntityInputHandler methods     ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void startupForEntity(Entity& entity);
		inline virtual void shutdownForEntity(Entity& entity);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return target entity, or a null pointer
		*/
		Entity* getEntity() const;

		/**
		*  @brief
		*    Return target entity's transform component; can be a null pointer if the entity does not exist or has no transform component
		*/
		TransformComponent* getTransformComponent() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64 mEntityId;	///< ID of the currently controlled entity; may be uninitialized


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/handler/EntityInputHandler-inl.h"
