// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OGRE/OgrePixelFormat.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Element;
		class ElementDocument;
	}
}
namespace Ogre
{
	class Camera;
	class SceneManager;
	class CompositorShadowNodeDef;
}
namespace qsf
{
	class CameraComponent;
	class ParameterGroup;
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
	*    Abstract high level shadow setup base class
	*/
	class QSF_API_EXPORT ShadowSetup : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		ShadowSetup();
		virtual ~ShadowSetup();

		/**
		*  @brief
		*    Return whether or not the shadow setup is considered to be in debug mode
		*
		*  @return
		*    "true" if the shadow setup is considered to be in debug mode, else "false"
		*/
		bool isDebug() const;

		/**
		*  @brief
		*    Set whether or not the component is considered to be in debug mode
		*
		*  @param[in] debugMode
		*    "true" if the component is considered to be in debug mode, else "false"
		*/
		void setDebug(bool debugMode);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ShadowSetup methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void setupOgreCompositorShadowNode(Ogre::CompositorShadowNodeDef& ogreCompositorShadowNodeDef) = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::ShadowSetup methods            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return the number of used shadow maps
		*
		*  @return
		*    The number of used shadow maps
		*/
		virtual uint8 getNumberOfShadowMaps() const = 0;

		virtual void recreate() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		Ogre::PixelFormat getOgrePixelFormat() const;
		void setupOgreCompositorShadowNodeSceneRendering(Ogre::CompositorShadowNodeDef& ogreCompositorShadowNodeDef, const std::string& renderTargetName, uint32 shadowMapIndex);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool				mDebugMode;			///< Requested
		bool				mInternalDebugMode;	///< What we currently have in use
		Ogre::SceneManager*	mOgreSceneManager;
		Ogre::Camera*		mOgreCamera;	// TODO(co) We have to update this pointer in case the OGRE camera gets destroyed


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setInternalDebug(bool debugMode);
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);

		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

		Rocket::Core::ElementDocument& getOrCreateRocketElementDocument();
		Rocket::Core::Element* createRocketElement(const std::string& type);
		Rocket::Core::Element* createRocketElementWithParent(Rocket::Core::Element* parentElement, const std::string& type);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Rocket::Core::ElementDocument*	mRocketElementDocument;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
