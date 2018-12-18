// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetProxy.h>

#include <QtCore/qobject.h>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	namespace logic
	{
		class Sequence;
		class SequenceTrackEntityComponentProperty;
	}
	namespace editor
	{
		class SequenceEditorState;
		class SequenceEditorTrackWrapper;
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
		*    Editing wrapper for sequences; encapsulates some often used tasks for sequence editing
		*/
		class SequenceEditorWrapper : public QObject
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT		// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
						// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<uint64>								EntityIdSet;
			typedef boost::container::flat_map<uint32, SequenceEditorTrackWrapper*> TrackWrapperMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceEditorWrapper(QObject* parent, const AssetProxy& sequenceAsset);
			virtual ~SequenceEditorWrapper();

			logic::Sequence* getSequence() const;

			const EntityIdSet& getEnabledEntitiesInSequence() const;
			void addEntity(uint64 entityId);
			void removeEntity(uint64 entityId);

			/**
			*  @brief
			*    Push an operation which will add a track for the ECP and add a initial key based on the current value of the component property
			*/
			void pushAddTrackOperationWithInitialKey(uint64 entityId, const std::string& propertyPath);

			/**
			*  @brief
			*    Push an operation which will add a ECP key based on the current value of the component property
			*/
			void pushAddKeyOperation(uint32 trackId, uint64 entityId, const std::string& propertyPath);

			/**
			*  @brief
			*    Enable a track for a entity-component-property; does nothing if the track exists already
			*/
			void enableEntityTrack(uint64 entityId, const std::string& componentPropertyPath);

			/**
			*  @brief
			*    Same behavior as "enableEntityTrack()", but also takes the track ID which should be assigned to the track
			*
			*  @note
			*    - Neccessary for restoring tracks in a robust manner
			*/
			bool enableEntityTrackForceTrackId(uint64 entityId, const std::string& componentPropertyPath, uint32 trackId);

			/**
			*  @brief
			*    Remove an entity-component-property track from the sequence; does nothing if it does not exist in the first place
			*/
			void disableEntityTrack(uint64 entityId, const std::string& componentPropertyPath);

			/**
			*  @brief
			*    Add any track, enforces a track ID
			*/
			bool createAnyTrackForceTrackId(const std::string& trackClassName, uint32 trackId, bool notifyAboutAddition = true);

			logic::SequenceTrackEntityComponentProperty* getTrackForEntityComponentProperty(uint64 entityId, const std::string& componentPropertyPath) const;

			void notifyTrackAdded(uint32 trackId);
			void removeTrackWithId(uint32 trackId);

			SequenceEditorTrackWrapper* getWrapperForTrack(uint32 trackId);

			SequenceEditorState* getSequenceEditorState() const;
			void setSequenceEditorState(SequenceEditorState* sequenceEditorState);

			/**
			*  @brief
			*    Access to the asset of the currently edited sequence
			*/
			const AssetProxy& getSequenceAssetProxy() const;

			/**
			*  @brief
			*    Load the data of the currently edited sequence from the manually specified relative filename
			*/
			bool loadFromFilename(const std::string& filename);

			/**
			*  @brief
			*    Save the data to a manually specified relative filename
			*/
			bool saveByRelativeFilename(const std::string& relativeFilename);

			/**
			*  @brief
			*    Save the data to a manually specified absolute filename
			*/
			bool saveByAbsoluteFilename(const std::string& absoluteFilename);

			/**
			*  @brief
			*    Does a quick check if all entities and their components exist
			*
			*  @return
			*    "true" if the sequence could be played without errors in the current map, "false" if not
			*/
			bool isSequenceCompatibleToMap();

			/**
			*  @brief
			*    Delete all tracks which have reference errors in the current map
			*/
			void deleteTracksWithReferenceErrors();

			/**
			*  @brief
			*    Serves as an abstraction layer between the actual value of the component property and the value which is needed for the sequence tracks.
			*
			*  @note
			*    In some cases the original value has to be transformed because the sequence track expects another value type or a different format.
			*    This interface takes care of the conversion. The other way around is already performed by the evaluation logic of the individual sequence track implementations.
			*/
			static bool extractKeyValueFromComponentProperty(const camp::Property* componentProperty, camp::UserObject componentAsUserObject, camp::Value& outValue);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Does a re-evaluation of the sequence to figure out which unique entities are referenced over all tracks
			*/
			void updateEntitiesInSequence();

			/**
			*  @brief
			*    Destroy all track wrappers which are generated so far
			*
			*  @note
			*    - This will most surely break references if it is done when the editor wrapper is active in the editor
			*/
			void clearTrackWrappers();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map&				 mMap;						///< Map instance to work with, do not destroy the instance
			AssetProxy			 mSequenceAsset;			///< Reference to the asset the currently edited sequence is bound to
			SequenceEditorState* mSequenceEditorState;		///< Sequence editor state, can be a null pointer, do not destroy the instance
			logic::Sequence*	 mSequence;					///< Wrapped sequence, can be a null pointer, do not destroy the instance
			EntityIdSet			 mEntitiesInSequence;
			TrackWrapperMap		 mGeneratedTrackWrappers;	///< Keep track of all wrappers we created so they can be re-used. Neccessary to have a single signalling source and to keep track on memory.


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
