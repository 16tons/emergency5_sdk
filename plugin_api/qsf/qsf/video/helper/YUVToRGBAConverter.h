// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


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
	*    YUV to RGBA encoder
	*/
	class QSF_API_EXPORT YUVToRGBAConverter
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct YUVBufferDescription
		{
			const uint8* YBuffer;
			const uint8* UBuffer;
			const uint8* VBuffer;
			uint32 imageWidth;
			uint32 imageHeight;
			uint32 uvWidth;
			uint32 uvHeight;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Converts the given YUV buffer into a pre-allocated RGBA buffer (which must be the same size as the Y plane)
		*/
		static void convert(const YUVBufferDescription& yuvBuffer, uint32* outRGBABuffer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		YUVToRGBAConverter() {}
		~YUVToRGBAConverter() {}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Static lookup tables needed for YUV conversion
		// TODO(oi) Check if arithmetic instead of LUT will be quicker due to cache pollution
		static bool gConversionTablesInitialized;
		static int32 gYTable[256];
		static int32 gRVTable[256];
		static int32 gGUTable[256];
		static int32 gGVTable[256];
		static int32 gBUTable[256];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
