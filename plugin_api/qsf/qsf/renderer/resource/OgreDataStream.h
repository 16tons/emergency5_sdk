// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/OgreDataStream.h>


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
	*    Data stream implementation for OGRE
	*
	*  @note
	*    - "qsf::OgreDataStream" is an OGRE data stream, meaning exceptions will be thrown in case of errors
	*/
	class OgreDataStream : public Ogre::DataStream
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] filename
		*    Name of the file to open
		*/
		OgreDataStream(const Ogre::String& filename);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] filename
		*    Name of the file to open
		*  @param[in] name
		*    The name (e.g. resource name) that can be used to identify the source for his data (optional)
		*/
		OgreDataStream(const Ogre::String& filename, const Ogre::String& name);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreDataStream();


	//[-------------------------------------------------------]
	//[ Public virtual "Ogre::DataStream" methods             ]
	//[-------------------------------------------------------]
	public:
		virtual size_t read(void* buf, size_t count) override;
		virtual void skip(long count) override;
		virtual void seek(size_t pos) override;
		virtual size_t tell() const override;
		virtual bool eof() const override;
		virtual void close() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		void* mPhysicsFsFile;	///< PhysicsFS file handle, can be a null pointer, close it if you no longer need it (we can't forward declare "PHYSFS_File")


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
