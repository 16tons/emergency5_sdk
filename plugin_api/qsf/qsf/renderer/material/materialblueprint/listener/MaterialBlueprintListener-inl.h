// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MaterialBlueprintListener::~MaterialBlueprintListener()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private virtual qsf::IMaterialBlueprintListener methods ]
	//[-------------------------------------------------------]
	inline void MaterialBlueprintListener::beginFillUnknown()
	{
		// Nothing here
	}

	inline bool MaterialBlueprintListener::fillUnknownValue(uint32, uint8*, uint32)
	{
		// Nothing here

		// Value not filled
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
