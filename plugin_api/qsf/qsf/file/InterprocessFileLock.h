// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <string>


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
	*    Replacement for boost::interprocess::file_lock on Windows, since the boost-implementation does not support Unicode filenames on Windows
	*
	* // TODO(ca) Implement as a pure wrapper for boost::interprocess::file_lock on all other platforms than Windows
	*/
	class QSF_API_EXPORT InterprocessFileLock
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		InterprocessFileLock();

		explicit InterprocessFileLock(const std::string& absoluteFilename);

		~InterprocessFileLock();

		bool open(const std::string& absoluteFilename);

		void close();

		bool isOpen() const;

		bool lock();

		void unlock();

		bool isLocked() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mAbsoluteFilename;
		#ifdef WIN32
			void* mFileHandle;
		#else
			int mFileDescriptor;
		#endif
		bool mIsLocked;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
