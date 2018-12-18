// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/handler/EntityInputHandler.h"

#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FreeMoveInputDevice;
	class CameraControlComponent;
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
	*    Free movement input handler class
	*
	*  @remarks
	*    Directly manipulates the "qsf::TransformComponent"-component without performing any kind of collision detection of physics.
	*
	*  @note
	*    - Primary intended for debugging and rapid prototyping
	*    - Restricted 3D rotation via Euler angles
	*/
	class QSF_API_EXPORT FreeMoveInputHandler : public EntityInputHandler
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const glm::vec3&, const glm::vec3&)> PositionChanged;	///< This Boost signal is emitted after the entity position has been changed; parameter: previous and new position
		boost::signals2::signal<void (const glm::quat&, const glm::quat&)> RotationChanged;	///< This Boost signal is emitted after the entity rotation has been changed; parameter: previous and new rotation


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
		explicit FreeMoveInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeMoveInputHandler();

		/**
		*  @brief
		*    Return the move speed
		*
		*  @return
		*    The move speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMoveSpeed() const;

		/**
		*  @brief
		*    Set the move speed
		*
		*  @param[in] moveSpeed
		*    The new move speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMoveSpeed(float moveSpeed);

		/**
		*  @brief
		*    Return the wheel move speed
		*
		*  @return
		*    The wheel move speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getWheelMoveSpeed() const;

		/**
		*  @brief
		*    Set the wheel move speed
		*
		*  @param[in] moveSpeed
		*    The new wheel move speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setWheelMoveSpeed(float moveSpeed);

		/**
		*  @brief
		*    Return the rotate speed
		*
		*  @return
		*    The rotate speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRotateSpeed() const;

		/**
		*  @brief
		*    Set the rotate speed
		*
		*  @param[in] rotateSpeed
		*    The new rotate speed
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRotateSpeed(float rotateSpeed);

		inline float getDefaultMoveSpeed() const;
		inline void setNullSpeedWithAltKeyEnabled(bool enable);

		inline const FreeMoveInputDevice& getFreeMoveInputDevice();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputHandler methods              ]
	//[-------------------------------------------------------]
	public:
		virtual void initialize() override;


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
	//[ Protected virtual qsf::FreeMoveInputHandler methods   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Gets called to setup the input feeder
		*/
		virtual void setupInputFeeder();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		CameraControlComponent* getCameraControlComponent() const;
		float getSpeedModifier() const;
		glm::vec3 getMovementDelta() const;
		glm::quat getNewRotation(float* outYaw = nullptr, float* outPitch = nullptr) const;
		float getMouseWheelDelta() const;
		void backupPositionRotation();
		void setNewPosition(const glm::vec3& newPosition);
		void setNewRotation(const glm::quat& newRotation);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to the CAMP reflection system
		float mMoveSpeed;		///< Move speed in world units per second
		float mWheelMoveSpeed;	///< Move speed for mouse wheel in world units per wheel step
		float mRotateSpeed;		///< Rotate speed in degrees per screen pixel
		// Internal
		FreeMoveInputDevice* mInputDevice;	///< Input device holding the key bindings; always valid, do not destroy
		glm::vec3			 mLastPosition;
		glm::quat			 mLastRotation;
		bool				 mNullSpeedWithAltKeyEnabled;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/handler/freemove/FreeMoveInputHandler-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::FreeMoveInputHandler)
