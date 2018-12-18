// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Context.h"
#include "qsf/input/InputListener.h"

#include <glm/glm.hpp>

#include <boost/signals2.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class Matrix4;
	class SceneManager;
	class MovableObject;
}
namespace Rocket
{
	namespace Core
	{
		class Context;
	}
}
namespace qsf
{
	class RenderWindow;
	class KeyboardEvent;
	class GuiContextInputListener;
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
	*    Ingame Graphical User Interface (GUI) context class
	*/
	class QSF_API_EXPORT GuiContext : public Context, public InputListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class GuiSystem;				// Creates GUI context instances
		friend class RenderWindow;			// Shows ingame GUIs
		friend class OgreCompositorPassGui;	// Sets the mouse cursor position


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (int width, int height)> ViewportDimensionChanged;	///< This Boost signal is emitted when viewport dimensions changed


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<RenderWindow*> RenderWindows;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void ()> ReloadGui;	///< This Boost signal is emitted when the GUI should be reloaded from RML files (hot-reloading to instantly see made changes)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~GuiContext();

		/**
		*  @brief
		*    Return whether or not the GUI context is enabled
		*
		*  @return
		*    "true" if the GUI context is enabled, else "false"
		*/
		inline bool isEnabled() const;

		/**
		*  @brief
		*    Set whether or not the GUI context is enabled
		*
		*  @param[in] enabled
		*    "true" if the GUI context is enabled, else "false"
		*/
		inline void setEnabled(bool enabled);

		/**
		*  @brief
		*    Set whether or not the GUI input is enabled
		*
		*  @param[in] enabled
		*    "true" if the GUI input is enabled, else "false"
		*/
		void setInputEnabled(bool enabled);

		/**
		*  @brief
		*    Return the GUI context overall visibility
		*
		*  @return
		*    The GUI context overall visibility
		*/
		inline float getVisibility() const;

		/**
		*  @brief
		*    Set the GUI context overall visibility
		*
		*  @param[in] visibility
		*    The GUI context overall visibility
		*/
		inline void setVisibility(float visibility);

		/**
		*  @brief
		*    Return the fixed GUI context dimension
		*
		*  @return
		*    The fixed GUI dimension, if component is not uninitialized, this fixed value is used instead of the dynamic viewport setting
		*/
		inline const glm::ivec2& getFixedDimension() const;

		/**
		*  @brief
		*    Set the fixed GUI context dimension
		*
		*  @param[in] fixedDimension
		*    The fixed GUI context dimension, if component is not uninitialized, this fixed value is used instead of the dynamic viewport setting
		*/
		inline void setFixedDimension(const glm::ivec2& fixedDimension);

		/**
		*  @brief
		*    Return the minimum GUI context dimension
		*
		*  @return
		*    The minimum GUI context dimension under which the GUI layout still works, dimensions below this will result in scaled down elements
		*/
		inline const glm::ivec2& getMinimumDimension() const;

		/**
		*  @brief
		*    Set the minimum GUI context dimension
		*
		*  @param[in] minimumDimension
		*    The minimum GUI context dimension under which the GUI layout still works, dimensions below this will result in scaled down elements
		*/
		inline void setMinimumDimension(const glm::ivec2& minimumDimension);

		/**
		*  @brief
		*    Return whether or not aspect ratio correction is enabled
		*
		*  @return
		*    "true" if the aspect ratio correction is enabled, else "false"
		*
		*  @remarks
		*    Usually, GUIs are designed for a resolution of 1920x1080 and the resolution aspect ratio. When using a resolution like
		*    1280x960 the GUI will look distorted without an GUI aspect ratio correction. Aspect ratio correction is only performed
		*    when running in fullscreen mode.
		*/
		inline bool isAspectRatioCorrectionEnabled() const;

		/**
		*  @brief
		*    Set whether or not aspect ratio correction is enabled
		*
		*  @param[in] enabled
		*    "true" if aspect ratio correction is enabled, else "false"
		*
		*  @see
		*    - "qsf::GuiContext::isAspectRatioCorrectionEnabled()"
		*/
		inline void setAspectRatioCorrectionEnabled(bool enabled);

		/**
		*  @brief
		*    Return the libRocket context
		*
		*  @return
		*    The libRocket context
		*/
		inline Rocket::Core::Context& getRocketCoreContext() const;

		/**
		*  @brief
		*    Return the render windows this GUI context is currently assigned to
		*
		*  @return
		*    The render windows this GUI context is currently assigned to
		*/
		inline const RenderWindows& getRenderWindows() const;

		//[-------------------------------------------------------]
		//[ Mouse                                                 ]
		//[-------------------------------------------------------]
		inline bool getFixedMouseCursorPosition() const;
		inline const glm::vec2& getMouseCursorPosition() const;
		glm::vec2 getNormalizedMouseCursorPosition() const;
		void setMouseCursorPosition(const glm::vec2& mouseCursorPosition, bool fixed);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Enable/disable default input
		*
		*  @note
		*    - Press the control-shift-g key to emit the "qsf::GuiContext::ReloadGui"-signal
		*/
		void setDefaultInputEnabled(bool enabled);

		/**
		*  @brief
		*    Return the encapsulated OGRE scene manager instance
		*
		*  @return
		*    The encapsulated OGRE scene manager instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the encapsulated OGRE scene manager instance if you don't have to
		*/
		inline Ogre::SceneManager* getOgreSceneManager() const;

		/**
		*  @brief
		*    Return the encapsulated OGRE movable object instance
		*
		*  @return
		*    The encapsulated OGRE movable object instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the encapsulated OGRE movable object instance if you don't have to
		*/
		inline Ogre::MovableObject* getGuiContextMoveable() const;

		/**
		*  @brief
		*    Return the OGRE camera instance
		*
		*  @return
		*    The OGRE camera instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the OGRE camera instance if you don't have to
		*/
		inline Ogre::Camera* getOgreCamera() const;

		/**
		*  @brief
		*    Return GUI space to clip space matrix
		*
		*  @param[out] ogreMatrix4
		*    Will receive the GUI space to clip space matrix
		*
		*  @note
		*    - In case of virtual reality (VR) applications using e.g. the Oculus Rift or HTC Vive, we need to be able to transform the GUI by using e.g. the headtracker
		*/
		void getGuiSpaceToClipSpaceMatrix(Ogre::Matrix4& ogreMatrix4) const;

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the GUI scissor rectangle usage is enabled
		*
		*  @return
		*    "true" if the GUI scissor rectangle usage is enabled, else "false"
		*
		*  @see
		*    - For GUI layout tests it's useful to disable the scissor rectangle usage from time to time in order to detect GUI layout problems
		*/
		inline bool isScissorRectangleUsageEnabled() const;

		/**
		*  @brief
		*    Set whether or not the GUI scissor rectangle usage is enabled
		*
		*  @param[in] enabled
		*    "true" if the GUI scissor rectangle usage is enabled, else "false"
		*
		*  @see
		*    - "qsf::GuiContext::isScissorRectangleUsageEnabled()"
		*/
		inline void setScissorRectangleUsageEnabled(bool enabled);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		GuiContext();

		/**
		*  @brief
		*    Called when the viewport dimension has been changed
		*
		*  @param[in] width
		*    The new viewport width
		*  @param[in] height
		*    The new viewport height
		*/
		void onViewportDimensionChanged(int width, int height);

		void processKeyboardEvent(const KeyboardEvent& keyboardEvent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						mEnabled;						///< "true" if the GUI context is enabled, else "false"
		float						mVisibility;					///< The GUI context overall visibility
		glm::ivec2					mFixedDimension;				///< If component is not uninitialized, this fixed value is used instead of the dynamic viewport setting
		glm::ivec2					mMinimumDimension;				///< Minimum dimension under which the GUI layout still works, dimensions below this will result in scaled down elements
		bool						mAspectRatioCorrectionEnabled;	///< "true" if the aspect ratio correction is enabled, else "false"
		Rocket::Core::Context*		mRocketCoreContext;				///< libRocket context, always valid, don't forget to remove the reference in case we no longer need it
		bool						mInputEnabled;
		GuiContextInputListener*	mGuiContextInputListener;		///< GUI context input listener instance, always valid, we're responsible for destroying the instance in case we no longer need it
		Ogre::SceneManager*			mOgreSceneManager;				///< OGRE scene manager, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		Ogre::MovableObject*		mGuiContextMoveable;			///< GUI context moveable, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		Ogre::Camera*				mOgreCamera;					///< OGRE camera instance, can be a null pointer
		RenderWindows				mRenderWindows;					///< Render windows this GUI context is assigned to, do not destroy the instances
		bool						mEnableDefaultInput;
		bool						mScissorRectangleUsageEnabled;
		glm::vec2					mMouseCursorPosition;
		bool						mFixedMouseCursorPosition;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/gui/GuiContext-inl.h"
