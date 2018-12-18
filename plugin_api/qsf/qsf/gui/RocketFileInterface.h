// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Rocket/Core/FileInterface.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FileCache;
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
	*    QSF file interface for libRocket
	*/
	class RocketFileInterface : public Rocket::Core::FileInterface
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RocketFileInterface();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RocketFileInterface();

		/**
		*  @brief
		*    Get the file cache instance
		*/
		inline FileCache& getFileCache() const  { return mFileCache; }


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Core::FileInterface methods    ]
	//[-------------------------------------------------------]
	public:
		virtual Rocket::Core::FileHandle Open(const Rocket::Core::String& path) override;
		virtual void Close(Rocket::Core::FileHandle file) override;
		virtual size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file) override;
		virtual bool Seek(Rocket::Core::FileHandle file, long offset, int origin) override;
		virtual size_t Tell(Rocket::Core::FileHandle file) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FileCache& mFileCache;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
