// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>
#include <qsf/base/GetUninitialized.h>

#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Prototypes                                            ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class SequenceEditorState;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF sequence editor selection manager class
		*
		*  @note
		*    This is not derived from SelectionManagerTemplate<uint64> because otherwise, we
		*    get duplicated symbols during the linker stage and I am not sure how to resolve that
		*    at this time, but the templated nature of that superclass makes it hard to figure out the
		*    best DLLIMPORT way to go here.
		*    Maybe the selection manager base should not be used anyway, because we're working with
		*    a heterogeneous selection set (tracks / keys) which cannot be easily represented
		*    by a single SelectionManagerTemplate<>
		*
		*  @note
		*    Since we're mixing entities, tracks and keys as the working set of the sequencer, we have to make a priority decision
		*    as all those selections don't mix well or get down to a single nominator.
		*    Well, if viewed correctly, they DO:
		*       entity := bunch of tracks selected
		*       track := bunch of keys selected
		*       selected entity = selected tracks = selected keys
		*    But showing the correct selection state to the user and supplying all operations on this
		*    intermixed selected is very confusing if not thought out properly. So we priorise
		*    selection of these types and allow only one homogenous selection to be active
		*    at any time. This makes working with selections easier
		*/
		class SequenceEditorSelectionManager : public QObject
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Structure holding the identity of a key in a track
			*/
			struct KeySelection
			{
				uint32 trackId;
				uint32 frameNumber;

				KeySelection() : trackId(0), frameNumber(0) {}
				KeySelection(uint32 trackId, uint32 frameNumber) : trackId(trackId), frameNumber(frameNumber) {}

				bool operator == (const KeySelection& other) const
				{
					return (trackId == other.trackId && frameNumber == other.frameNumber);
				}

				bool operator < (const KeySelection& other) const
				{
					if (trackId != other.trackId)
					{
						return (trackId < other.trackId);
					}

					return (frameNumber < other.frameNumber);
				}
			};

			typedef boost::container::flat_set<KeySelection> KeySelectionSet;
			typedef boost::container::flat_set<uint32>		 TrackSelectionSet;
			typedef boost::container::flat_set<uint64>		 EntitySelectionSet;

			/**
			*  @brief
			*    Structure holding complete information about any selection state
			*/
			struct SelectionState
			{
				SelectionState() :
					currentKeyFrameNumber(getUninitialized<uint32>())
				{}

				SelectionState(const KeySelectionSet& keySet, const TrackSelectionSet& trackSet, const EntitySelectionSet& entitySet) :
					keySet(keySet),
					currentKeyFrameNumber(getUninitialized<uint32>()),
					trackSet(trackSet),
					entitySet(entitySet)
				{}

				/**
				*  @brief
				*    Some factory methods to ease creation of selection states
				*/
				static SelectionState fromKeys(const KeySelectionSet& keys) { return SelectionState(keys, TrackSelectionSet(), EntitySelectionSet()); }
				static SelectionState fromTracks(const TrackSelectionSet& tracks) { return SelectionState(KeySelectionSet(), tracks, EntitySelectionSet()); }
				static SelectionState fromTrack(uint32 trackId) { TrackSelectionSet ts; ts.insert(trackId); return fromTracks(ts); }
				static SelectionState fromEntities(const EntitySelectionSet& entities) { return SelectionState(KeySelectionSet(), TrackSelectionSet(), entities); }

				KeySelectionSet		keySet;
				uint32				currentKeyFrameNumber;	///< The current ID, "qsf::getUninitialized<IdType>()" if there's no current ID
				TrackSelectionSet	trackSet;
				EntitySelectionSet	entitySet;
			};


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			void selectionChanged();	///< Selection set has been changed (keys and/or tracks)


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit SequenceEditorSelectionManager(SequenceEditorState* parent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SequenceEditorSelectionManager();

			/**
			*  @brief
			*    Check if the current selection set contains keys
			*/
			bool areKeysSelected() const;

			/**
			*  @brief
			*    Check if the current selection set contains tracks
			*/
			bool areTracksSelected() const;

			/**
			*  @brief
			*    Check if the current selection set contains entities
			*/
			bool areEntitiesSelected() const;

			bool isKeySelected(const KeySelection& keySelection) const;
			bool isKeySelected(uint32 trackId, uint32 keyFrameNumber) const;
			bool isTrackSelected(uint32 trackId) const;
			bool isEntitySelected(uint64 entityId) const;
			uint32 getCurrentSelectedKeyFrameNumber() const;

			const SelectionState& getSelectionState() const;

			const TrackSelectionSet& getTrackSelectionSet() const;
			const KeySelectionSet& getKeySelectionSet() const;
			const EntitySelectionSet& getEntitySelectionSet() const;

			/**
			*  @brief
			*    Change the selection sets directly
			*
			*  @note
			*    - Only one of the sets can be populated! In a ambigous case, the track set takes precedence
			*/
			void changeSelectionDirectly(const SelectionState& newState);

			//[-------------------------------------------------------]
			//[ Operation                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Clear selected set
			*/
			base::Operation* createClearSelectionOperation();

			/**
			*  @brief
			*    Replace the current selection with a new track selection
			*/
			base::Operation* createSetTrackSelectionOperation(const TrackSelectionSet& newTrackSet);

			/**
			*  @brief
			*    Add a track to the selection
			*/
			base::Operation* createAddTrackToSelectionOperation(uint32 trackId) const;

			/**
			*  @brief
			*    Remove a track from the selection
			*/
			base::Operation* createRemoveTrackFromSelectionOperation(uint32 trackId) const;

			/**
			*  @brief
			*    Replace the current selection with a new key selection
			*/
			base::Operation* createSetKeySelectionOperation(const KeySelectionSet& newKeySet);

			/**
			*  @brief
			*    Add a key to the selection
			*/
			base::Operation* createAddKeyToSelectionOperation(uint32 trackId, uint32 frameNumber);

			/**
			*  @brief
			*    Remove a key from the selection
			*/
			base::Operation* createRemoveKeyFromSelectionOperation(uint32 trackId, uint32 frameNumber);

			/**
			*  @brief
			*    Replace the current selection with a new entity selection
			*/
			base::Operation* createSetEntitySelectionOperation(const EntitySelectionSet& newEntitySet);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			base::Operation* makeSelectionOperationFromCurrentState(const KeySelectionSet& newKeySelection, const TrackSelectionSet& newTrackSelection, const EntitySelectionSet& newEntitySelection) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SelectionState		 mSelectionState;
			SequenceEditorState* mParentState;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
