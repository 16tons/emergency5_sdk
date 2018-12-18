// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>

#include <QtCore/qobject.h>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}
namespace qsf
{
	namespace logic
	{
		class SequenceTrack;
		class SequenceTrackKey;

		// This is a really unsafe forward declaration which should match SequenceTrack::SequenceTrackKeyWithIndex. But there is a static assert to detect mismatches!
		// TODO(oi) Maybe move the pair definition into the qsf namespace instead of using it as a nested type.
		typedef std::pair<SequenceTrackKey*, uint32> SequenceTrackKeyWithIndex;
	}
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class SequenceEditorState;
		class SequenceEditorWrapper;
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
		*    Wrapper class to ease the editing of individual sequence tracks
		*
		*  @remarks
		*    The wrapping allows catching of modifications and creates a relatively thin layer of abstraction between the
		*    editor and the edited data. Each track publishes a number of sub-tracks, which are finally editable. The editor
		*    works on a number of specified sub track types which gets converted to the implementation needed track data.
		*/
		class SequenceEditorTrackWrapper : public QObject
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static base::Operation* getAddKeyOperation(SequenceEditorState* editorState, uint32 trackId, uint32 newFrameNumber, const boost::property_tree::ptree* keyData);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SequenceEditorTrackWrapper(SequenceEditorWrapper* sew, uint32 trackId);
			virtual ~SequenceEditorTrackWrapper() {}

			class SubTrack
			{
			public:
				explicit SubTrack(SequenceEditorTrackWrapper& parentTrack);

				virtual ~SubTrack() {}

				enum SubTrackType
				{
					// TODO(co) Why is there commented code?
					STT_FLOAT,
					/*STT_INT,
					STT_BOOL,
					STT_STRING,*/
				};

				virtual SubTrackType getTrackType() const = 0;

			protected:
				SequenceEditorTrackWrapper& mTrack;
			};

			class FloatSubTrack : public SubTrack
			{
			public:
				explicit FloatSubTrack(SequenceEditorTrackWrapper& parentTrack);

				virtual SubTrackType getTrackType() const override { return STT_FLOAT; }
				virtual uint32 getKeyCount() const { return 0; }

				virtual uint32 getKeyFrameNumberAt(uint32 index) const { return 0.0f; }

				virtual void setKeyAt(uint32 index, float newValue) {}
				virtual float getKeyAt(uint32 index) const { return 0.0f; }

				virtual bool getSplitTangentsFlagAt(uint32 index) const { return false; }
				virtual void setSplitTangentsFlagAt(uint32 index, bool split) { }

				virtual float getOutgoingControlPointAt(uint32 index) const { return 0.0f; }
				virtual float getOutgoingControlPointTimeAt(uint32 index) const { return 0.0f; }

				virtual float getIncomingControlPointAt(uint32 index) const { return 0.0f; }
				virtual float getIncomingControlPointTimeAt(uint32 index) const { return 0.0f; }

				virtual void setOutgoingControlPointAt(uint32 index, float newValue, float newTime) {}
				virtual void setIncomingControlPointAt(uint32 index, float newValue, float newTime) {}

				virtual bool supportsBezierInterpolation() const { return false; }

				void flattenTangentsAt(uint32 index);
				void linearizeTangentsAt(uint32 index);
			};

			virtual uint32 getNumberOfSubTracks() const = 0;
			virtual SubTrack& getSubTrack(uint32 n) = 0;
			uint32 getTrackId() const { return mTrackId; }
			SequenceEditorWrapper& getSequence() const { return *mSequence; }
			logic::SequenceTrack* getTrackReference() const;

			bool isEntityComponentPropertyTrack() const;

			/**
			*  @brief
			*    Generate an operation for moving a key on the time axis
			*
			*  @note
			*   - The key value parameter is non-const because it is needed to be mutable in the method itself
			*/
			base::Operation* getMoveKeyAtOperation(uint32 index, uint32 newFrameNumber, const boost::property_tree::ptree* keyValue, bool handleSelection = true) const;
			base::Operation* getAddKeyOperation(uint32 newFrameNumber, const boost::property_tree::ptree* keyData) const;
			base::Operation* getRemoveKeyOperation(uint32 newFrameNumber, bool handleSelection = true) const;

			void saveKeyToPtree(uint32 keyIndex, boost::property_tree::ptree& targetPtree) const;
			void loadKeyFromPtree(uint32 keyIndex, const boost::property_tree::ptree& sourcePtree);

			/**
			*  @brief
			*    Create a key in the track at the given time
			*/
			logic::SequenceTrackKeyWithIndex createKeyAtFrame(uint32 frameNumber, const boost::property_tree::ptree* initialValue);

			/**
			*  @brief
			*    Create a key with no initial value if it is not known
			*/
			logic::SequenceTrackKeyWithIndex createKeyAtFrame(uint32 frameNumber);

			/**
			*  @brief
			*    High-level helper: Find a key at a given frame number; if no key is found, the returned key reference will be null pointer
			*/
			logic::SequenceTrackKeyWithIndex findKeyAtFrame(uint32 frameNumber) const;

			/**
			*  @brief
			*    Check if a key exists at a given time
			*/
			bool hasKeyAtFrame(uint32 frameNumber) const;

			/**
			*  @brief
			*    Remove a key in the track
			*/
			bool removeKeyByIndex(uint32 keyIndex);

			bool removeKeyByFrameNumber(uint32 frameNumber);


		protected:
			SequenceEditorWrapper* mSequence;
			uint32				   mTrackId;


		};

		class SequenceEditorTrackWrapperGeneric : public SequenceEditorTrackWrapper
		{


		public:
			SequenceEditorTrackWrapperGeneric(SequenceEditorWrapper* sew, uint32 trackId);
			~SequenceEditorTrackWrapperGeneric() {}
			virtual uint32 getNumberOfSubTracks() const override;

			virtual SubTrack& getSubTrack(uint32 n) override;


		};

		class SequenceEditorTrackWrapperVec3 : public SequenceEditorTrackWrapper
		{


		public:
			SequenceEditorTrackWrapperVec3(SequenceEditorWrapper* sew, uint32 trackId);
			~SequenceEditorTrackWrapperVec3();
			virtual uint32 getNumberOfSubTracks() const override;

			virtual SubTrack& getSubTrack(uint32 n) override;

			FloatSubTrack* mTrackX;
			FloatSubTrack* mTrackY;
			FloatSubTrack* mTrackZ;


		};

		class SequenceEditorTrackWrapperSingleFloat : public SequenceEditorTrackWrapper
		{


		public:
			SequenceEditorTrackWrapperSingleFloat(SequenceEditorWrapper* sew, uint32 trackId);
			~SequenceEditorTrackWrapperSingleFloat();
			virtual uint32 getNumberOfSubTracks() const override;

			virtual SubTrack& getSubTrack(uint32 n) override;

			FloatSubTrack* mTrack;


		};

		class SequenceEditorTrackWrapperColor3 : public SequenceEditorTrackWrapper
		{


		public:
			SequenceEditorTrackWrapperColor3(SequenceEditorWrapper* sew, uint32 trackId);
			~SequenceEditorTrackWrapperColor3();
			virtual uint32 getNumberOfSubTracks() const override;

			virtual SubTrack& getSubTrack(uint32 n) override;

			FloatSubTrack* mTrackR;
			FloatSubTrack* mTrackG;
			FloatSubTrack* mTrackB;


		};

		class SequenceEditorTrackWrapperColor4 : public SequenceEditorTrackWrapper
		{


		public:
			SequenceEditorTrackWrapperColor4(SequenceEditorWrapper* sew, uint32 trackId);
			~SequenceEditorTrackWrapperColor4();
			virtual uint32 getNumberOfSubTracks() const override;

			virtual SubTrack& getSubTrack(uint32 n) override;

			FloatSubTrack* mTrackR;
			FloatSubTrack* mTrackG;
			FloatSubTrack* mTrackB;
			FloatSubTrack* mTrackA;


		};

		class SequenceEditorTrackWrapperEulerAngles : public SequenceEditorTrackWrapper
		{


		public:
			SequenceEditorTrackWrapperEulerAngles(SequenceEditorWrapper* sew, uint32 trackId);
			~SequenceEditorTrackWrapperEulerAngles();
			virtual uint32 getNumberOfSubTracks() const override;

			virtual SubTrack& getSubTrack(uint32 n) override;

			FloatSubTrack* mTrackX;
			FloatSubTrack* mTrackY;
			FloatSubTrack* mTrackZ;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
