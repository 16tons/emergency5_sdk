// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/component/spatial/SpatialPartition2DQuadtree.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/job/JobArguments.h"
#include "qsf/job/JobProxy.h"
#include "qsf/audio/AudioProxy.h"
#include "qsf/base/manager/FastPodAllocator.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioSource;
	class AudioProxy;
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
	*    Audio source merging management component class
	*
	*  @remarks
	*    Current, only 2^8 (256) different ambient audio assets are supported simultaneously.
	*    When the limit has to be increased, the type of the group ID has to be enlarged and
	*    all the bit-juggling to combine group ID and source ID has to be refactored!
	*/
	class QSF_API_EXPORT AmbientSpotAudioMergingComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;						///< "qsf::AmbientSpotAudioMergingComponent" unique component ID
		static const uint32 SOURCE_HANDLE_GROUP_ID_BIT_SHIFT;	///< How much bits the group ID is shifted inside the source handle

		/**
		*  @brief
		*    Data structure which is used for each item which gets registered in the spatial partition.
		*    Also servers as the implementation of the SpatialPartition2D::ItemTraits policy
		*    @see SpatialPartition2D
		*/
		struct AmbientSpotAudioSourceItem : public FastPodAllocator<AmbientSpotAudioSourceItem>
		{
			typedef uint64 Identity; ///< ID type for every item
			struct Bounds
			{
				glm::vec2 min;
				glm::vec2 max;
			};

			struct CircleBounds
			{
				glm::vec2 center;
				float radius;
			};

			/**
			*  @brief
			*    Implementation of the ItemTraits::getBounds contract
			*/
			static Bounds getBounds(const AmbientSpotAudioSourceItem* const& ptrItem)
			{
				const AmbientSpotAudioSourceItem& item = *ptrItem;

				Bounds b;

				const glm::vec2 radius(item.radius, item.radius);

				b.min = glm::vec2(item.center.x, item.center.z) - radius;
				b.max = glm::vec2(item.center.x, item.center.z) + radius;

				return b;
			}

			static bool doBoundsOverlap(const Bounds& a, const Bounds& b)
			{
				// Do a SAT (separating axis test) for the two aligned axes
				for (uint32 n = 0; n < 2; ++n)
				{
					if (a.max[n] < b.min[n] || a.min[n] > b.max[n])
					{
						return false;
					}
				}

				return true;
			}

			static bool doBoundsOverlap(const Bounds& a, const CircleBounds& b)
			{
				float sqDist = 0.0f;

				const glm::vec2& min = a.min;
				const glm::vec2& max = a.max;

				for (int n = 0; n < 2; ++n)
				{
					float v = b.center[n];

					if (v < min[n])
					{
						sqDist += (min[n] - v) * (min[n] - v);
					}

					if (v > max[n])
					{
						sqDist += (v - max[n]) * (v - max[n]);
					}
				}

				return sqDist < (b.radius * b.radius);
			}

			static void splitBoundsQuadrant(const Bounds& bounds, uint32 quadrant, Bounds& outQuadrantBounds)
			{
				const glm::vec2 halfSize = (bounds.max - bounds.min) * 0.5f;

				glm::vec2 quadrantMin = (bounds.max + bounds.min) * 0.5f;

				quadrantMin.x += (quadrant & 1) ? 0.0f : -halfSize.x;
				quadrantMin.y += (quadrant & 2) ? 0.0f : -halfSize.y;

				outQuadrantBounds.min = quadrantMin;
				outQuadrantBounds.max = outQuadrantBounds.min + halfSize;
			}

			static bool areBoundsEncompassingBounds(const Bounds& outerBounds, const Bounds& innerBounds)
			{
				return
					outerBounds.max.x > innerBounds.max.x &&
					outerBounds.max.y > innerBounds.max.y &&

					outerBounds.min.x < innerBounds.min.x &&
					outerBounds.min.y < innerBounds.min.y;
			}

			/**
			*  @brief
			*    Implementation of the SpatialPartition2D::ItemTraits::getIdentity contract
			*/
			static inline const Identity& getIdentity(const AmbientSpotAudioSourceItem* const& item)
			{
				return item->id;
			}

			inline bool operator == (const AmbientSpotAudioSourceItem& other) const
			{
				return id == other.id;
			}

			inline bool operator != (const AmbientSpotAudioSourceItem& other) const
			{
				return id != other.id;
			}

			inline bool operator < (const AmbientSpotAudioSourceItem& other) const
			{
				return id < other.id;
			}

			uint64 id;
			glm::vec3 center;
			float radius;
			AudioProxy audioProxy;
		};

		typedef std::vector<AmbientSpotAudioSourceItem*> SpotAudioSourceList;

		/**
		*  @brief
		*    Defines a group of sound sources all playing the same individual audio asset
		*    TODO(oi) Comment!
		*/
		class AmbientSpotSourceGroup
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Data structure passed to AmbientSpotSourceGroup::update()
			*/
			struct UpdateData
			{
				float deltaTime;				///< Time passed since last invocation, in seconds
				glm::vec3 attentionZoneCenter;	///< Center of the attention zome
			};

			typedef SpatialPartition2DQuadtree<AmbientSpotAudioSourceItem*, AmbientSpotAudioSourceItem> SourceQuadtree;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			AmbientSpotSourceGroup(uint8 groupId, const AmbientSpotAudioSourceItem::Bounds& worldBounds);
			virtual ~AmbientSpotSourceGroup();
			void setup(GlobalAssetId individualAudioAssetId, GlobalAssetId ambienceAudioAssetId);
			void update(const UpdateData& data);
			void addSoundSource(uint64 handle, const glm::vec3& position, float radius);
			void removeSource(uint64 handle);

			uint8 getGroupId() const { return mGroupId; }
			GlobalAssetId getIndividualAudioGlobalAssetId() const { return mAudioGlobalAssetId; }


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint8 mGroupId;

			AudioProxy mAmbienceAudioProxy;
			SpotAudioSourceList mSources;
			GlobalAssetId mAudioGlobalAssetId;
			GlobalAssetId mAmbienceAudioGlobalAssetId;

			float mAmbientLevel;

			SourceQuadtree mLookup;
			SourceQuadtree::ItemSet mCurrentSourcesInVicinity;
			SourceQuadtree::ItemSet mPreviousSourcesInVicinity;
		};

		typedef std::vector<AmbientSpotSourceGroup*> SpotSourceGroupList;


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
		explicit AmbientSpotAudioMergingComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AmbientSpotAudioMergingComponent();

		/**
		*  @brief
		*    Add a new individual audio spot source
		*
		*  @param[in] individualSoundAsset
		*    Global asset ID of a sound asset which should be played for this source when it is hearable individually
		*  @param[in] ambienceSoundAsset
		*    Global asset ID of a sound asset which should be played when multiple individual sources are combined into a single ambience source
		*  @param[in] center
		*    Point in world space where the spot ambient sound is played
		*  @param[in] radius
		*    Range of the source in which the individual sound should be hearable
		*
		*  @return
		*    A handle to the newly created ambient spot sound source. You have to use removeSoundSource() to get rid of it.
		*
		*  @note
		*    - This will play a individual sound source or the ambience sound source depending on the
		*      number of individual sound sources near the listener
		*    - The ambienceSoundAsset must always be supplied but it is only relevant from the first call in combination with a "new"
		*      individual sound source. The main distinguishing attribute is the individual sound asset.
		*/
		uint64 addSoundSource(GlobalAssetId individualSoundAsset, GlobalAssetId ambienceSoundAsset, const glm::vec3& center, float radius);

		/**
		*  @brief
		*    Remove a spot ambient audio source created by addSoundSource()
		*/
		void removeSoundSource(uint64 uid);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		virtual void onUpdate(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Searche and create the group if neccessary for a given individual audio source
		*/
		AmbientSpotSourceGroup& getGroupForAmbientSound(GlobalAssetId audioSourceGlobalAssetId, GlobalAssetId ambienceSoundAsset);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobProxy			mUpdateJob;
		SpotSourceGroupList	mAllGroups;						///< List of all spot source groups
		uint64				mAmbientSourceHandleCounter;	///< Counts upwards to generate new run-time unique spot audio source handles


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AmbientSpotAudioMergingComponent)
