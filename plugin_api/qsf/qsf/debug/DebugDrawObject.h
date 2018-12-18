// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Color4.h"
#include "qsf/math/Transform.h"
#include "qsf/time/Time.h"
#include "qsf/job/JobProxy.h"
#include "qsf/debug/DebugTextParameters.h"

#include <glm/fwd.hpp>

#include <boost/optional.hpp>
#include <boost/signals2.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	class ManualObject;
}
namespace Rocket
{
	namespace Core
	{
		class Element;
	}
}
namespace qsf
{
	class CameraComponent;
	class DebugDrawManager;
	class DebugDrawProxy;
	class DebugDrawRequest;
	class DebugDrawLifetimeData;
	class DebugDrawOwnerData;
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
	*    Debug draw object class
	*/
	class QSF_API_EXPORT DebugDrawObject : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get a name for a debug color ogre material but do not create one
		*/
		static std::string getOgreMaterialNameByColor(const DebugTextParameters::ColorTexture& debugColorTexture, bool depthInteraction, bool drawInClipSpace);

		/**
		*  @brief
		*    Get the name of a debug color ogre material and create a material if there is none
		*/
		static std::string getOrCreateOgreMaterialByColor(const DebugTextParameters::ColorTexture& debugColorTexture, bool depthInteraction, bool drawInClipSpace);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] manager
		*    Owner debug draw manager
		*  @param[in] id
		*    Debug draw request ID
		*  @param[in] proxy
		*    Debug draw proxy pointer; may be a null pointer
		*/
		inline DebugDrawObject(DebugDrawManager& manager, uint32 id, DebugDrawProxy* proxy);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DebugDrawObject();

		/**
		*  @brief
		*    Return the owner debug draw manager
		*/
		inline DebugDrawManager& getDebugDrawManager() const;

		/**
		*  @brief
		*    Return this debug draw object's ID
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return this debug draw object's proxy, or null pointer if there is none
		*/
		inline DebugDrawProxy* getProxy() const;

		/**
		*  @brief
		*    Clear all debug drawing
		*/
		void clear();

		/**
		*  @brief
		*    Detach the debug draw proxy, leaving this debug draw object independent
		*/
		void detachProxy();

		/**
		*  @brief
		*    Evaluate given lifetime data, i.e. prepare removal of itself when lifetime is over
		*/
		void evaluateLifetimeData(const DebugDrawLifetimeData& lifetimeData);

		/**
		*  @brief
		*    Evaluate given owner data, i.e. link internal OGRE manual object to the owner
		*/
		void evaluateOwnerData(const DebugDrawOwnerData& ownerData);

		/**
		*  @brief
		*    Set the transform of the debug draw object
		*
		*  @param[in] transform
		*    New transform of the debug draw object
		*/
		void setTransform(const Transform& transform);

		/**
		*  @brief
		*    Return whether or not drawing of the object in view space (in world space otherwise) is enabled
		*
		*  @return
		*    If true, draw object in clip space with coordinates in [-1, 1]; if false, draw it in world space
		*/
		inline bool getDrawInClipSpace() const;

		/**
		*  @brief
		*    Enable or disable drawing of the object in view space (in world space otherwise)
		*
		*  @param[in] drawInClipSpace
		*    If true, draw object in clip space with coordinates in [-1, 1]; if false, draw it in world space
		*/
		void setDrawInClipSpace(bool drawInClipSpace);

		/**
		*  @brief
		*    Return the debug draw object's internal OGRE manual object - if necessary, create one
		*/
		Ogre::ManualObject& getOrCreateOgreManualObject();

		/**
		*  @brief
		*    Create and return a new libRocket element of the given type
		*/
		Rocket::Core::Element* createRocketElement(const std::string& type);

		/**
		*  @brief
		*    Create and return a new libRocket element of the given type with a parent
		*/
		Rocket::Core::Element* createRocketElementWithParent(Rocket::Core::Element* parentElement, const std::string& type);

		/**
		*  @brief
		*    Create and return a new libRocket element of the given type
		*/
		Rocket::Core::Element* createRocketElementWithWorldPosition(const std::string& type, const glm::vec3& worldPosition, const glm::vec2& windowSpaceOffset);

		/**
		*  @brief
		*    Set flag for rendering the debug draw object as billboard, means it rotated to the camera
		*/
		void setRequestDrawAsBillboard(bool drawAsBillboard);

		//[-------------------------------------------------------]
		//[ Visibility                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get visible state of this debug draw object
		*/
		inline bool isVisible() const;

		/**
		*  @brief
		*    Show or hide debug drawing
		*/
		void setVisible(bool visible);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);

		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);
		void setOgreManualObjectDrawInClipSpace(bool drawInClipSpace);

		Rocket::Core::Element* createRocketElementInternal(const std::string& type, Rocket::Core::Element* parentElement, const glm::vec3* worldPosition = nullptr, const glm::vec2* windowSpaceOffset = nullptr);
		void setRocketElementPosition(Rocket::Core::Element& rocketElement, const glm::vec3& worldPosition, Ogre::Camera& ogreCamera, Ogre::Viewport& ogreViewport, const glm::vec2& windowSpaceOffset = glm::vec2());


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct RocketElement
		{
			Rocket::Core::Element* mElement;
			bool mUsesWorldPosition;
			glm::vec3 mWorldPosition;
			glm::vec2 mWindowSpaceOffset;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DebugDrawManager*			mManager;								///< Pointer to manager, always valid, must not be deleted
		uint32						mId;									///< ID associated with this debug draw object
		DebugDrawProxy*				mProxy;									///< Debug draw proxy used to create this object; may be a null pointer in case there is no proxy
		Ogre::SceneNode*			mOgreSceneNode;							///< OGRE scene node instance containing "mOgreManualObject"
		Ogre::ManualObject*			mOgreManualObject;						///< OGRE manual object instance holding the render geometry data
		std::vector<RocketElement>	mRocketElements;
		boost::signals2::connection	mPreCompositorWorkspaceUpdateConnection;
		JobProxy					mRemovalJobProxy;						///< Job proxy for removing this debug draw object after a certain time
		bool						mIsVisible;								///< If "false", the associated debug draw requests are hidden
		bool						mDrawInClipSpace;						///< Draw in clip space instead of world space
		bool						mDrawAsBillboard;						///< Turn object every frame to camera
		boost::optional<Transform>	mTransformToSet;						///< Transform to set for the OGRE scene node as soon as it is created


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawObject-inl.h"
