// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline GridComponent::GridComponent(Prototype* prototype) :
			Component(prototype),
			mOgreSceneNode(nullptr),
			mOgreObject(nullptr),
			mGridExtents(100.0f),
			mGridColor(0.7f, 0.7f, 0.7f, 0.4f)
		{
			mLineWidths[0] = 0.02f;
			mLineWidths[1] = 0.06f;
			mLineWidths[2] = 0.12f;
			mLineSteps[0] = 1.0f;
			mLineSteps[1] = 10.0f;
			mLineSteps[2] = 50.0f;
		}

		inline GridComponent::~GridComponent()
		{
			// Nothing to do in here
		}

		inline float GridComponent::getGridExtents() const
		{
			return mGridExtents;
		}

		inline void GridComponent::setGridExtents(float value)
		{
			// Something changed?
			if (mGridExtents != value)
			{
				mGridExtents = value;

				// Update render geometry
				rebuildManualObject();
			}
		}

		inline float GridComponent::getPrimaryLineWidth() const
		{
			return mLineWidths[0];
		}

		inline void GridComponent::setPrimaryLineWidth(float value)
		{
			setLineWidthByIndex(0, value);
		}

		inline float GridComponent::getSecondaryLineWidth() const
		{
			return mLineWidths[1];
		}

		inline void GridComponent::setSecondaryLineWidth(float value)
		{
			setLineWidthByIndex(1, value);
		}

		inline float GridComponent::getTertiaryLineWidth() const
		{
			return mLineWidths[2];
		}

		inline void GridComponent::setTertiaryLineWidth(float value)
		{
			setLineWidthByIndex(2, value);
		}

		inline float GridComponent::getPrimaryLineStep() const
		{
			return mLineSteps[0];
		}

		inline void GridComponent::setPrimaryLineStep(float value)
		{
			setLineStepByIndex(0, value);
		}

		inline float GridComponent::getSecondaryLineStep() const
		{
			return mLineSteps[1];
		}

		inline void GridComponent::setSecondaryLineStep(float value)
		{
			setLineStepByIndex(1, value);
		}

		inline float GridComponent::getTertiaryLineStep() const
		{
			return mLineSteps[2];
		}

		inline void GridComponent::setTertiaryLineStep(float value)
		{
			setLineStepByIndex(2, value);
		}

		inline const Color4& GridComponent::getGridColor() const
		{
			return mGridColor;
		}

		inline void GridComponent::setGridColor(const Color4& value)
		{
			// Something changed?
			if (mGridColor != value)
			{
				mGridColor = value;

				// Update render geometry
				rebuildManualObject();
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		inline void GridComponent::setLineWidthByIndex(int index, float value)
		{
			QSF_ASSERT(index >= 0 && index < 3, "Invalid index", QSF_REACT_THROW);

			// Something changed?
			if (mLineWidths[index] != value)
			{
				mLineWidths[index] = value;

				// Update render geometry
				rebuildManualObject();
			}
		}

		inline void GridComponent::setLineStepByIndex(int index, float value)
		{
			QSF_ASSERT(index >= 0 && index < 3, "Invalid index", QSF_REACT_THROW);

			// Something changed?
			if (mLineSteps[index] != value)
			{
				mLineSteps[index] = value;

				// Update render geometry
				rebuildManualObject();
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
