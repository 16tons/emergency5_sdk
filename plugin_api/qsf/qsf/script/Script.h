// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/Object.h"

#include <camp/value.hpp>

#include <vector>

#include <glm/glm.hpp>	// TODO(co) Rethink this, in general, the script interface can only interact with primitive data types, no good idea to try to add all complex ones without a generic solution


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Function;
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
	*    Abstract script base class
	*
	*  @remarks
	*    The script interface interacts with the CAMP reflection system to avoid the need to write special script bindings for
	*    functionality already connected to the reflection system. Variable type IDs are identified by using "camp::Type" and
	*    variables with a dynamic types are handled via "camp::Value".
	*
	*    Each script class should have the following CAMP reflection system tags:
	*    - "ScriptLanguage": Script language (for example: "Lua" or "JavaScript")
	*    - "Formats":  Comma separated file format extensions this script can load in (for example: "lua" or "js" or "lua,LUA,js,JS")
	*
	*    Supported script features:
	*    - Global variables (with namespace support)
	*    - Global functions (with namespace support)
	*      - C++ calls script
	*      - Script calls C++
	*    - CAMP reflection system objects
	*      - Tags
	*      - Properties
	*      - Methods
	*      - Signals // TODO(co) Port my PixelLight script integration to QSF: The CAMP reflection system has no direct support for signals, this was quite handy in PixelLight
	*      - Slots   // TODO(co) Port my PixelLight script integration to QSF: The CAMP reflection system has no direct support for slots, this was quite handy in PixelLight
	*
	*    Supported primitive data types: bool, float, double, int8, int16, int32, int64, uint8, uint16, uint32, uint64
	*    Please note that not each script language/API may make such a detailed data type distinction.
	*    Because strings are fundamental within scripts, std::string is supported as well. Same is true for qsf::Object* and qsf::Object&.
	*/
	class QSF_API_EXPORT Script : public Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Script();

		/**
		*  @brief
		*    Return the name of the script
		*
		*  @return
		*    The name of the script, optional but recommended for better debugging
		*/
		std::string getName() const;

		/**
		*  @brief
		*    Set the name of the script
		*
		*  @param[in] name
		*    The name of the script, optional but recommended for better debugging
		*/
		void setName(const std::string& name);

		/**
		*  @brief
		*    Return the name of the script language the script is using
		*
		*  @return
		*    The name of the script language the script is using (for example "Lua" or "JavaScript")
		*/
		std::string getScriptLanguage() const;

		/**
		*  @brief
		*    Return a list of file extensions this script supports
		*
		*  @param[out] fileExtensions
		*    List of file extensions this script supports, e.g. ["lua", "LUA"] (the given list is not cleared before new entries are added)
		*/
		void getFileExtensions(std::vector<std::string>& fileExtensions) const;

		/**
		*  @brief
		*    Add a script binding to connect the given CAMP reflection system class instance with this script
		*
		*  @param[in] object
		*    CAMP reflection system class instance, must stay valid as long as this script exists
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @note
		*    - The added CAMP reflection system class instance methods will be available to the script as simple global functions
		*/
		void addBinding(Object& object, const std::string& namespaceName = "");

		/**
		*  @brief
		*    Add all script bindings to this script
		*
		*  @remarks
		*    Iterates over all available script binding instances and adds them to this script.
		*
		*  @note
		*    - The added CAMP reflection system class instance methods will be available to the script as simple global functions
		*/
		void addBindings();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Script methods                    ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Global functions                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether or not the given name belongs to a global function
		*
		*  @param[in] name
		*    Name of the global function (e.g. "gHealth")
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @return
		*    "true" if the given name belongs to a global function, else "false"
		*
		*  @remarks
		*    When calling a global script function, the script backend usually writes an error into the
		*    log when the given global script function wasn't found. So, when using optional global script
		*    functions, it's a good idea to check whether there's such a global script function by using "qsf::Script::isGlobalFunction()".
		*/
		virtual bool isGlobalFunction(const std::string& name, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Add a global function to the script
		*
		*  @param[in] object
		*    Object to apply the function on, the instance must stay valid as long as the global function is registered inside the script
		*  @param[in] functionName
		*    Function name used inside the script to call the global function (e.g. "myFunction")
		*  @param[in] campFunction
		*    CAMP function to be called, the instance must stay valid as long as the global function is registered inside the script
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe a script is already set?)
		*
		*  @note
		*    - If there's already a set script ("qsf::Script::setSourceCode()") this method will return an error
		*    - It's highly recommended to use "qsf::Script::ScriptBinding" instead of adding global functions manually
		*/
		virtual bool addGlobalFunction(Object& object, const std::string& functionName, const camp::Function& campFunction, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Remove all global functions from the script
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe a script is already set?)
		*
		*  @see
		*    - "qsf::Script::addGlobalFunction()"
		*/
		virtual bool removeAllGlobalFunctions() = 0;

		//[-------------------------------------------------------]
		//[ Script source code                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the script source code
		*
		*  @return
		*    The script source code
		*/
		virtual std::string getSourceCode() const = 0;

		/**
		*  @brief
		*    Set the script source code
		*
		*  @param[in] sourceCode
		*    Script source code, usually blank ASCII code, empty string to set to script at all
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool setSourceCode(const std::string& sourceCode) = 0;

		/**
		*  @brief
		*    Return a list of filenames associated with this script
		*
		*  @param[out] filenames
		*    Receives a list of filenames associated with this script (list is not cleared before adding new entries)
		*
		*  @remarks
		*    For example Lua allows to use the keyword "require" to add the content of another script. This method
		*    returns a list of the filenames of the files which are included within this script. One can use this
		*    information to e.g. reload the script when the one of the associated files was changed, beside reloading
		*    when just the original script file was changed.
		*/
		virtual void getAssociatedFilenames(std::vector<std::string>& filenames) = 0;

		/**
		*  @brief
		*    Execute a given script source code string
		*
		*  @param[in] sourceCode
		*    Script source code to execute, usually blank ASCII code
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Only works if there's already a valid script currently used
		*    - Lua example statement: "Speed=42"
		*/
		virtual bool execute(const std::string& sourceCode) = 0;

		//[-------------------------------------------------------]
		//[ Global variables                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add the names of found global variables to a given list
		*
		*  @param[out] globalVariables
		*    List to be filled with the names (without namespace) of the found global variables, the given list is not cleared before new entries are added
		*  @param[in]  namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*/
		virtual void getGlobalVariables(std::vector<std::string>& globalVariables, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Return whether or not the given name belongs to a global variable
		*
		*  @param[in] name
		*    Name of the global variable
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @return
		*    "true" if the given name belongs to a global variable, else "false"
		*/
		virtual bool isGlobalVariable(const std::string& name, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Return the CAMP type ID a global variable
		*
		*  @param[in] name
		*    Name of the global variable
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @return
		*    The CAMP type ID of the global variable (e.g. "camp::realType" for "float") or "camp::noType" on error
		*/
		virtual camp::Type getGlobalVariableCampType(const std::string& name, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Return the current value of a global variable
		*
		*  @param[in] name
		*    Name of the global variable
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @return
		*    The current value of the global variable
		*/
		virtual camp::Value getGlobalVariable(const std::string& name, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Set the current value of a global variable
		*
		*  @param[in] name
		*    Name of the global variable to set
		*  @param[in] campValue
		*    New value of the global variable
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @note
		*    - If there's no global variable with the given name, a new global variable is added to the script using the type of the CAMP value
		*    - Please note that it depends on the used script language/API which data types are really available,
		*      this means that "qsf::Script::getGlobalVariableCampType()" may return another data type as the one you specified
		*    - If a script global variable already exists, its type will not be changed in case the CAMP value has another type. Background is
		*      that there are some typed script languages like AngelScript which do not allow to change the type.
		*/
		virtual void setGlobalVariable(const std::string& name, const camp::Value& campValue, const std::string& namespaceName = "") = 0;

		//[-------------------------------------------------------]
		//[ Global function call, used by "qsf::ScriptFunction"   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Start a function call
		*
		*  @param[in] functionName
		*    Name of the function to call
		*  @param[in] functionResult
		*    "true" in case the function has a result, else "false"
		*  @param[in] namespaceName
		*    Optional namespace (e.g. "test", "test.me" and so on)
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - It's not recommended to use this method directly, use "qsf::ScriptFunction" instead
		*
		*  @see
		*    - Have a look at "qsf::Script::isGlobalFunction()" for additional information
		*/
		virtual bool beginCall(const std::string& functionName, bool functionResult, const std::string& namespaceName = "") = 0;

		/**
		*  @brief
		*    Push an argument required for the current function call
		*
		*  @param[in] value
		*    Argument value
		*
		*  @note
		*    - It's not recommended to use this method directly, use "qsf::ScriptFunction" instead
		*/
		virtual void pushArgument(bool value) = 0;
		virtual void pushArgument(float value) = 0;
		virtual void pushArgument(double value) = 0;
		virtual void pushArgument(int8 value) = 0;
		virtual void pushArgument(int16 value) = 0;
		virtual void pushArgument(int32 value) = 0;
		virtual void pushArgument(int64 value) = 0;
		virtual void pushArgument(uint8 value) = 0;
		virtual void pushArgument(uint16 value) = 0;
		virtual void pushArgument(uint32 value) = 0;
		virtual void pushArgument(uint64 value) = 0;
		virtual void pushArgument(const std::string& string) = 0;
		virtual void pushArgument(const glm::vec3& value) = 0;	// TODO(co) Rethink this, in general, the script interface can only interact with primitive data types, no good idea to try to add all complex ones without a generic solution
		virtual void pushArgument(Object* object) = 0;
		virtual void pushArgument(Object& object) = 0;

		/**
		*  @brief
		*    End a function call
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - It's not recommended to use this method directly, use "qsf::ScriptFunction" instead
		*    - This actually performs the prepared function call
		*/
		virtual bool endCall() = 0;

		/**
		*  @brief
		*    Return the result of a function call
		*
		*  @param[in] value
		*    Unused value... just there so the compiler can figure out the proper method
		*
		*  @return
		*    The result of a function call
		*
		*  @note
		*    - It's not recommended to use this method directly, use "qsf::ScriptFunction" instead
		*/
		virtual bool getReturn(bool value) = 0;
		virtual float getReturn(float value) = 0;
		virtual double getReturn(double value) = 0;
		virtual int8 getReturn(int8 value) = 0;
		virtual int16 getReturn(int16 value) = 0;
		virtual int32 getReturn(int32 value) = 0;
		virtual int64 getReturn(int64 value) = 0;
		virtual uint8 getReturn(uint8 value) = 0;
		virtual uint16 getReturn(uint16 value) = 0;
		virtual uint32 getReturn(uint32 value) = 0;
		virtual uint64 getReturn(uint64 value) = 0;
		virtual std::string getReturn(std::string value) = 0;
		virtual Object* getReturn(Object* value) = 0;
		virtual Object& getReturn(Object& value) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		Script();

		/**
		*  @brief
		*    Write a string into the log
		*
		*  @param[in] logLevel
		*    Log level
		*  @param[in] text
		*    Text which should be written into the log
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The text is written to the log only if the current
		*    log level is greater or equal to the specified value.
		*    This method is an extension of "Log::Output()"		// TODO(co) Port my PixelLight script integration to QSF: Update comment
		*    which also adds the name of the script to the given
		*    text.
		*/
		// TODO(co) Port my PixelLight script integration to QSF: Update log level type
		bool logOutput(uint8 logLevel, const std::string& text);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::string mName;	///< Name of this script, optional but recommended for better debugging


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::Script)
