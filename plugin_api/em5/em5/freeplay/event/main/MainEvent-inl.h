// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	inline void MainEvent::onIntroStart(const qsf::MessageParameters& parameters)
	{
		// Base implementation is empty
	}

	inline void MainEvent::onIntroFinished(const qsf::MessageParameters& parameters)
	{
		// Base implementation is empty
	}

	inline void MainEvent::onPrologueFinished(const qsf::MessageParameters& parameters)
	{
		// Base implementation is empty
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline const std::string& MainEvent::getEventLayerName() const
	{
		return mEventLayerName;
	}

	inline const std::string& MainEvent::getEraseLayerName() const
	{
		return mEraseLayerName;
	}

	inline const std::string& MainEvent::getCutsceneLayerName() const
	{
		return mIntroCutsceneLayerName;
	}

	inline qsf::Entity& MainEvent::getScene() const
	{
		QSF_CHECK(nullptr != mScene, "Could not get main event scene entity", QSF_REACT_THROW);
		return *mScene;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
