// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/pluginable/PluginableManager.h"

#include <boost/signals2.hpp>

#include <ostream>
#include <istream>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
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
	*    Abstract parameter groups group manager class
	*/
	class QSF_API_EXPORT ParameterGroupManager : public PluginableManager
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const ParameterGroup&, uint32)> PropertyChanged;	///< This Boost signal is emitted when a property of one of the parameter groups was changed; parameter: parameter group instance, unique class property ID inside the parameter group class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ParameterGroupManager();

		/**
		*  @brief
		*    Set all parameters to their default values
		*/
		void setParametersToDefaultValues();

		/**
		*  @brief
		*    Load a parameter group by using a given absolute filename
		*
		*  @param[in] filename
		*    Absolute UTF-8 filename in platform-independent notation of the parameter group to load
		*  @param[in] formatTypeOverride
		*    If not a null pointer, this format type string is overriding the default format type required in the JSON
		*  @param[in] showMessageBoxOnParserError
		*    If "true", show a message box to the user if a parser error occurs reading the file
		*  @param[in] setToDefaults
		*    If "true", all properties are reset to their defaults before loading
		*
		*  @return
		*    "true" if all went fine, else "false" (file not found?)
		*
		*  @note
		*    - The current parameter group will get lost, so, you might want to save a manipulated parameter group before loading a new one
		*    - On success, automatically makes the given filename to the internally stored parameter group filename
		*
		*  @todo
		*    - TODO(co) For now, we only support JSON
		*/
		bool loadByAbsoluteFilename(const std::string& filename, const std::string* formatTypeOverride = nullptr, bool showMessageBoxOnParserError = false, bool setToDefaults = true);

		/**
		*  @brief
		*    Save the parameter group by using a given absolute filename
		*
		*  @param[in] filename
		*    Absolute UTF-8 filename in platform-independent notation of the parameter group to save
		*  @param[in] differenceToDefault
		*    Do not save properties which have the default value (meaning we already know its value without having to read it when loading)
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - On success, automatically makes the given filename to the internally stored parameter group filename
		*
		*  @todo
		*    - TODO(co) For now, we only support JSON
		*/
		bool saveByAbsoluteFilename(const std::string& filename, bool differenceToDefault = false);

		/**
		*  @brief
		*    Return the absolute filename of the parameter group
		*
		*  @return
		*    The absolute UTF-8 filename in platform-independent notation of the  parameter group, do no destroy the returned instance
		*
		*  @note
		*    - There's no "setFilename()"-method by intent
		*/
		const std::string& getFilename() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		ParameterGroupManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string mFilename;	///< Absolute UTF-8 filename in platform-independent notation of the parameter group, in case it was loaded from a file


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ParameterGroupManager)
