// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/reflection/type/CampGlmQuat.h"
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"


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
	*    Linear rotation component class
	*/
	class QSF_API_EXPORT LinearRotationComponent : public Component, public JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::LinearRotationComponent" unique component ID
		static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
		static const uint32 ROTATION;		///< "Rotation" unique class property ID inside the class
		static const uint32 MULTIPLIER;		///< "Multiplier" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline LinearRotationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LinearRotationComponent();

		/**
		*  @brief
		*    Get the rotation
		*
		*  @return
		*    Returns the rotation on each step
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::quat& getRotation() const;

		/**
		*  @brief
		*    Set the rotation for each step
		*
		*  @param[in] rotation
		*    Rotation quaternion
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Get the rotation multiplier
		*
		*  @return
		*    Returns the multiplier from the rotation on each step
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMultiplier() const;

		/**
		*  @brief
		*    Set the multiplier for each step
		*
		*  @param[in] multiplier
		*    Rotation multiplier
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setMultiplier(float multiplier);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onSetActive(bool active) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Component* getMovableObjectRenderingListenerComponent() override;
		virtual void registerMovableObjectRenderingListener() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void backupOriginalTransform();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		glm::quat	  mRotation;				///< Rotation quaternion
		float		  mMultiplier;				///< Multiplier of the rotation
		// Internal
		glm::quat	  mOriginalRotation;		///< Original rotation
		glm::quat	  mCurrentRotationOffset;	///< Current rotation offset quaternion
		bool		  mListening;				///< Listening to component property changes?


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
#include "qsf/component/rotation/LinearRotationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LinearRotationComponent)
