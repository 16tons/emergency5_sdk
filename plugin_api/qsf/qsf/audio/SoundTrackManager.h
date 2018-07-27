// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/NamedIdentifier.h"
#include "qsf/base/WeakPtr.h"
#include "qsf/job/JobProxy.h"

#include <boost/noncopyable.hpp>

#include <unordered_map>
#include <map>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class JobArguments;
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
	*    Sound track manager class
	*/
	class QSF_API_EXPORT SoundTrackManager : public qsf::WeakPtrTarget
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			struct MusicList
			{
				enum NextMode
				{
					STOP,
					PREVIOUS,
					MUSICLISTID
				};

				// Shortcuts
				typedef float Level;
				typedef uint32 MusicListId;
				typedef std::string AudioSource;
				typedef std::vector<AudioSource> AudioSourceList;

				AudioSourceList	mAudioSources;
				NextMode		mNextMode;
				MusicListId		mNextMusicId;
				// TODO(db): Transition
			};

			typedef MusicList::Level							Level;
			typedef MusicList::MusicListId						MusicListId;
			typedef NamedIdentifier								SoundtrackName;
			typedef std::unordered_map<MusicListId, MusicList>	Soundtrack;
			typedef std::map<SoundtrackName, Soundtrack>		SoundtrackList;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] map
			*    Music level system owner map instance, must stay valid as long as this music level system instance exists
			*  @param[in] name
			*    Name of the soundtrack asset
			*/
			SoundTrackManager(Map& map, const NamedIdentifier& name);

			/**
			*  @brief
			*    Destructor
			*/
			~SoundTrackManager();

			void setSoundtrack(const std::string& name);
			void setLastSoundtrack();

			const SoundtrackName& getCurrentSoundtrackName() const;
			MusicList* getMusicListById(int level) const;
			MusicListId findMusicListId(float level) const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void loadAssets();
			Soundtrack* findSoundtrack(const SoundtrackName& soundtrackName);
			MusicList* findMusicListById(Soundtrack& soundtrack, MusicListId id) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NamedIdentifier	mAssetId;
			SoundtrackList	mSoundtrackList;		///< List of all soundtracks

			Soundtrack*		mCurrentSoundtrack;		///< The current soundtrack which is loaded, null if no soundtrack is loaded
			SoundtrackName	mCurrentSoundtrackName;
			SoundtrackName	mLastSoundtrackName;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
