// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
	*    Compression helper class for binary serialization
	*/
	class QSF_API_EXPORT BinarySerializerCompress
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		BinarySerializerCompress(bool compress, BinarySerializer& inputSerializer, bool writeUncompressedSize, size_t uncompressedBufferMinSize = 0, int compressionLevel = 7);
		~BinarySerializerCompress();

		BinarySerializer& getUncompressedDataSerializer();
		void flush();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BinarySerializer& mInputSerializer;			///< Binary serializer we got in the constructor, will be filled with compressed data
		bool			  mWriteUncompressedSize;
		int				  mCompressionLevel;		///< Between 1 (fastest) and 9 (best compression)

		struct Uncompressed;
		Uncompressed* mUncompressed;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
