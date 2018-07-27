// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/gui/GuiDocument.h"
#include "qsf/time/Time.h"

#include <OGRE/OgreFrameListener.h>

#include <boost/noncopyable.hpp>


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
		class ElementText;
	}
}
namespace qsf
{
	class GridSceneManager;
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
	*    Debug overlay class
	*/
	class DebugOverlay : public GuiDocument, public Ogre::FrameListener, public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] ogreRenderWindow
		*    OGRE render window to use, the instance must stay alive as long as this debug overlay exists
		*/
		explicit DebugOverlay(GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DebugOverlay();

		bool getShowFramerateStability() const;
		void setShowFramerateStability(bool showFramerateStability);

		// TODO(fw): This is only a workaround for Mac rendering errors
		void setRenderInvisible(bool invisible);


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::FrameListener methods            ]
	//[-------------------------------------------------------]
	public:
		virtual bool frameEnded(const Ogre::FrameEvent& ogreFrameEvent) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateFramerateStabilityVisualization(GridSceneManager& gridSceneManager);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool					   mShowFramerateStability;
		Rocket::Core::ElementText* mFpsElementText;						///< FPS text element, always valid, do not destroy the instance
		Rocket::Core::ElementText* mGeometryElementText;				///< Geometry text element, always valid, do not destroy the instance
		Ogre::ManualObject*		   mOgreManualObject;
		Ogre::SceneNode*		   mOgreSceneNode;
		bool					   mRenderInvisible;
		Time					   mPipelineStateCompilerWorkingTime;	///< We show the "compiling" information a little bit longer to always be able to see that compiling was done (visual hiccup debugging)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
