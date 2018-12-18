// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>
#include <qsf/math/Color3.h>
#include <qsf/math/BitFlagSet.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Image;
	class Texture;
	template<typename T> class SharedPtr;
	typedef SharedPtr<Texture> TexturePtr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    The warbler feed window was introduced in EMERGENCY 2017 and hence is only available in this version and above
	*/
	class EM5_API_EXPORT WarblerFeedWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum Flag
		{
			SHOW_NEW_TWEETS_AUTOMATICALLY			= 1<<0,	///< Show new tweets automatically
			SHOW_WARBLER_FEED_BUTTON_ON_NEW_TWEET	= 1<<1,	///< Show the warbler feed button inside the menu bar as soon as a new tweet arrives
			PLAY_AUDIO_ON_NEW_TWEET					= 1<<2,	///< Play an audio effect as soon as a new tweet arrives
			PAUSE_GAME_ON_SHOW_WINDOW				= 1<<3,	///< Pause the game as soon as the warbler feed window gets shown
			SHOW_COMMENT_ICON						= 1<<4,	///< Show comment icon
			SHOW_SHARE_ICON							= 1<<5,	///< Show share icon
			SHOW_LIKE_ICON							= 1<<6,	///< Show like icon
			BLOCK_INPUT								= 1<<7,	///< Block input if the warbler feed is visible
			EDITABLE_TEXT							= 1<<8,	///< The feed text is editable (no special display formating when active)
		};
		typedef qsf::BitFlagSet<uint16, Flag> Flags;

		struct WarblerTweet
		{
			std::string				text;
			uint64					cameraEntityId;
			glm::vec3				cameraPosition;	///< Store the camera position at the point in time of capturing the screenshot, the camera might move later on
			glm::quat				cameraRotation;	///< Store the camera rotation at the point in time of capturing the screenshot, the camera might move later on
			Rocket::Core::Element*	element;		///< Holds the close event objective window element
			float					height;			///< Holds height of the element
			Ogre::TexturePtr*		ogreTexturePtr;
		};
		typedef std::vector<WarblerTweet> WarblerTweets;


	//[-------------------------------------------------------]
	//[ Public boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when an editable tweet text has changed.
		*
		*  @note
		*    - Recommended slot signature: void onWarblerTextChanged(const std::string& text, int warblerId)
		*/
		boost::signals2::signal<void(const std::string&, int)> WarblerTextChanged;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool shouldWarblerFeedWindowBeEnabled();
		static bool isWarblerFeedWindowEnabled();
		static void clearWarblerFeed();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit WarblerFeedWindow(qsf::GuiContext& guiContext);
		virtual ~WarblerFeedWindow();

		/**
		*  @brief
		*    Return whether or not the warbler feed window is enabled
		*/
		inline bool isEnabled() const;

		/**
		*  @brief
		*    Enable/disable warbler feed window; e.g. inside medieval events it doesn't make much sense
		*/
		void setEnabled(bool enabled);

		/**
		*  @brief
		*    Return the flags
		*/
		inline Flags getFlags() const;

		/**
		*  @brief
		*    Set the flags
		*/
		inline void setFlags(Flags flags);

		/**
		*  @brief
		*    Add warbler tweet to the warbler feed
		*
		*  @param[in] text
		*    Warbler tweet text
		*  @param[in] cameraEntityId
		*    ID of the camera entity to use for screenshot generation
		*  @param[in] rmlTitleAsString
		*    Optional RML title as string, if empty string it's generated automatically
		*/
		void addWarblerTweet(const std::string& text, uint64 cameraEntityId, const std::string& rmlTitleAsString = "");

		/**
		*  @brief
		*    Add warbler tweet with a fixed image to the warbler feed
		*
		*  @param[in] text
		*    Warbler tweet text
		*  @param[in] imageLocalFilename
		*    Local file name of the image to use, e.g. "em5/gui/warbler/arsonist.dds"
		*  @param[in] rmlTitleAsString
		*    Optional RML title as string, if empty string it's generated automatically
		*
		*  @return
		*    Index of the adder warbler tweet
		*/
		int addFixedImageWarblerTweet(const std::string& text, const std::string& imageLocalFilename, const std::string& rmlTitleAsString = "");

		/**
		*  @brief
		*    Add external warbler tweet to the warbler feed
		*
		*  @param[in] text
		*    Warbler tweet text
		*  @param[in] ogreImage
		*    OGRE image containing the warbler tweet screenshot
		*  @param[in] rmlTitleAsString
		*    Optional RML title as string, if empty string it's generated automatically
		*
		*  @return
		*    Index of the adder warbler tweet
		*/
		int addExternalWarblerTweet(const std::string& text, Ogre::Image& ogreImage, const std::string& rmlTitleAsString = "");

		/**
		*  @brief
		*    Remove all warbler tweets
		*/
		void clear();

		/**
		*  @brief
		*    Return all warbler tweets
		*/
		inline const WarblerTweets& getWarblerTweets() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void onReloadGui();

		/**
		*  @brief
		*    Process the animation
		*/
		void onAnimationUpdate(const qsf::JobArguments& jobArguments);

		void onWarblerShotRendering(uint64 camearEntityId, Ogre::Image* ogreImage = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			mEnabled;
		Flags			mFlags;
		qsf::JobProxy	mAnimationJobProxy;	///< Regular animation update job proxy; for updates once a frame; insert new warbler tweet animation
		qsf::JobProxy	mMakeShotJobProxy;	///< Delayed update job proxy for making a warbler shot
		WarblerTweets	mWarblerTweets;
		WarblerTweets	mNewWarblerTweets;	///< Hold a list of the new warbler tweet which have to insert in the warbler feed window
		bool			mCurrentInputBlockedState;
		std::string		mNextWarblerText;
		std::string		mNextWarblerRmlTitleAsString;
		uint32			mTextureCounter;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/hud/WarblerFeedWindow-inl.h"
