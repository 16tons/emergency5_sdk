// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioProxy.h"
#include "qsf/base/WeakPtr.h"
#include "qsf/base/manager/FastPodAllocator.h"
#include "qsf/component/Component.h"
#include "qsf/job/JobProxy.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class SoundTrackManager;
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
	*    Dynamic music core compositor
	*/
	class QSF_API_EXPORT DynamicMusicCompositorComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SoundTrackManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::DynamicMusicCompositorComponent" unique component ID
		static const uint32 ACTIVE;			///< "qsf::Active" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline DynamicMusicCompositorComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DynamicMusicCompositorComponent();

		/**
		*  @brief
		*    Access the stock music list
		*/
		inline const std::vector<std::string>& getMusicFiles() const;

		/**
		*  @brief
		*    Set a new stock music list
		*/
		void setMusicFiles(const std::vector<std::string>& musicFilenames);

		/**
		*  @brief
		*    Set a new stock music list
		*/
		void setMusicAssetProxies(const std::vector<AssetProxy>& musicAssetProxies);

		/**
		*  @brief
		*    Start a new score immediately; when it has finished, a new random track will be chosen from the stock music list
		*/
		void playMusicNow(const std::string& musicFilename);

		/**
		*  @brief
		*    Start a new score immediately; when it has finished, a new random track will be chosen from the stock music list
		*/
		void playMusicNow(const AssetProxy& musicAsset);

		void clearMusicFiles();

		void deleteMusicFiles();

		void setMaxMusicVolume(float maxVolume);

		void setSoundTrackManager(SoundTrackManager& soundTrackManager);
		void setMusicLevel(float level);
		void setSoundtrack(const std::string& name);
		void setLastSoundtrack();
		float getMusicLevel() const;
		int getCurrentLoop() const;
		void changeMusicTrack();


	//[-------------------------------------------------------]
	//[ Interface as required by camp_ext::ArrayMapper        ]
	//[-------------------------------------------------------]
	public:
		// TODO(co) Review & document
		virtual void setNode(std::size_t index, const std::string& value);
		virtual void insertNode(std::size_t before, const std::string& value);
		virtual void removeNode(std::size_t index);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Structure to hold a single music track
		*
		*  @remarks
		*    A track is actively played. Multiple tracks can play in parallel.
		*/
		struct MusicTrack : public FastPodAllocator<MusicTrack>
		{
			std::string	musicFilename;		///< Virtual filename of the audio file which is played by this track
			AudioProxy	musicPlayback;		///< Actual audio playback proxy
			Time		duration;			///< Original duration of the music asset
			Time		fadeInTimeBegin;	///< Point in time where the fade-in begins
			Time		fadeInTimeEnd;		///< Point in time where the fade-in ends
			Time		playbackPosition;	///< Current (approx.) position of the playhead (might be off-sync due to timer/audio sync problems)
			Time		fadeOutTimeBegin;	///< Point in time where the fade-out begins
			Time		fadeOutTimeEnd;		///< Point in time where the fade-out ends
			float		maxVolume;

			/**
			*  @brief
			*    Calculate the volume for the current playback time
			*/
			float calculateVolume() const;

			/**
			*  @brief
			*    Check the playback time to see if the track is currently fading in (volume is increasing)
			*/
			bool isFadingIn() const;

			/**
			*  @brief
			*    Check the playback time to see if the track is currently fading out (volume is decreasing)
			*/
			bool isFadingOut() const;


		};

		typedef std::shared_ptr<MusicTrack> MusicTrackPtr;	///< Music track instances are managed by this smart pointer
		typedef std::vector<MusicTrackPtr> MusicTrackPtrs;	///< List of music tracks


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

		/**
		*  @brief
		*    Does some data sync when the stock music assets array has been modified
		*/
		void onMusicAssetsChanged();

		/**
		*  @brief
		*    Update all playing music tracks, occasionally starting a new one if all current tracks are beginning to fade out
		*/
		void updateTracks(const Time& delta);

		/**
		*  @brief
		*    Start a new random score from the "stock music pool" of this component
		*/
		void startNextMusicRandomly();

		/**
		*  @brief
		*    Start a new score, fades out all other active ones
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool startNewMainTrack(const std::string& musicFilename);

		void performTrackChange();
		void changeLevel(float newLevel);
		int getNextTrackId();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobProxy				 mUpdateJobProxy;		///< Job proxy for audio updates
		std::vector<std::string> mMusicFilenames;		///< Stock music filenames which form a pool from which a score is randomly chosen for playback
		Time					 mDefaultMusicFadeTime;	///< Default time for the fade between two scores
		MusicTrackPtr			 mCurrentMainTrack;
		MusicTrackPtrs			 mTracks;				///< All currently active tracks
		float					 mMaxMusicVolume;		///< Maximum volume of the music

		// Intern
		qsf::WeakPtr<SoundTrackManager> mSoundTrackManager;	///< Reference of the sound track manager. Is used to get soundtracks and music lists.
		float	mCurrentLevel;		///< The level of the music which is currently playing
		float	mNextLevel;			///< The level of the music which will be playing after the current music is finished. If its the same as current the music is looping.
		int		mCurrentTrackId;	///< Number of the current music track in the music list
		int		mNextTrackId;
		int		mCurrentLoop;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
		QSF_CAMP_ARRAY_PROPERTY(MusicFiles, DynamicMusicCompositorComponent, std::string)	// Adds public member variable "MusicAssets" which is the middle part of the music assets <-> CAMP interconnection


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/component/DynamicMusicCompositorComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DynamicMusicCompositorComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::DynamicMusicCompositorComponent::MusicFilesInterface)
