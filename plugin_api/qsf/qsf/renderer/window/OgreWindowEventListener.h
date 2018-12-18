// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/OgreWindowEventUtilities.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RenderWindow;
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
	*    OGRE window event listener
	*/
	class OgreWindowEventListener : public Ogre::WindowEventListener, public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderWindow
		*    Owner render window
		*/
		explicit OgreWindowEventListener(RenderWindow& renderWindow);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreWindowEventListener();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::WindowEventListener methods      ]
	//[-------------------------------------------------------]
	public:
		virtual void windowResized(Ogre::RenderWindow* renderWindow) override;
		virtual void windowFocusChange(Ogre::RenderWindow *renderWindow) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RenderWindow* mRenderWindow;	///< Owner render window, always valid, don't destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
