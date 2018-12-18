// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/handler/freemove/FreeMoveInputHandler.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CameraComponent;
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
	*    Orbiting camera input handler class
	*
	*  @remarks
	*    Rotates around a target point, which is always bound to the ground.
	*    Directly manipulates the "qsf::TransformComponent"-component without performing any kind of collision detection of physics.
	*/
	class QSF_API_EXPORT OrbitingCameraInputHandler : public FreeMoveInputHandler
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
		explicit OrbitingCameraInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OrbitingCameraInputHandler();

		/**
		*  @brief
		*    Return target entity's camera component, or a null pointer
		*/
		CameraComponent* getCameraComponent() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::EntityInputHandler methods     ]
	//[-------------------------------------------------------]
	protected:
		virtual void startupForEntity(Entity& entity) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputHandler methods           ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateInputHandler(const Time& timePassed) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputListener methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void eventReceived(const InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void dropTargetPointToGround(glm::vec3& point, float offsetAboveGround, bool terrainOnly = true) const;
		void dropTargetPointToGround(glm::vec3& point, float offsetAboveGround, uint32 groundMapId) const;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::OrbitingCameraInputHandler)
