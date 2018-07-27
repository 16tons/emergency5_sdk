// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <string>

#include <boost/utility/value_init.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Forward declarations                                  ]
	//[-------------------------------------------------------]
	class Script;


	//[-------------------------------------------------------]
	//[ Types                                                 ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Type that is not used
	*/
	class NullType
	{
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 5 parameters and a return value
	*/
	template <typename R, typename T0 = NullType, typename T1 = NullType, typename T2 = NullType, typename T3 = NullType, typename T4 = NullType>
	class ScriptFunction
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		R operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, true, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->pushArgument(t2);
				mScript->pushArgument(t3);
				mScript->pushArgument(t4);
				if (mScript->endCall())
				{
					boost::value_initialized<R> r;
					return (R)mScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return boost::value_initialized<R>();
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "MyNamespace", "MyNamespace.MyOtherNamespace" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 5 parameters without a return value
	*/
	template <typename T0, typename T1, typename T2, typename T3, typename T4>
	class ScriptFunction<void, T0, T1, T2, T3, T4>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		void operator ()(T0 t0, T1 t1, T2 t2, T3 t3, T4 t4)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, false, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->pushArgument(t2);
				mScript->pushArgument(t3);
				mScript->pushArgument(t4);
				mScript->endCall();
			}
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 4 parameters and a return value
	*/
	template <typename R, typename T0, typename T1, typename T2, typename T3>
	class ScriptFunction<R, T0, T1, T2, T3>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		R operator ()(T0 t0, T1 t1, T2 t2, T3 t3)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, true, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->pushArgument(t2);
				mScript->pushArgument(t3);
				if (mScript->endCall())
				{
					boost::value_initialized<R> r;
					return (R)mScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return boost::value_initialized<R>();
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 4 parameters without a return value
	*/
	template <typename T0, typename T1, typename T2, typename T3>
	class ScriptFunction<void, T0, T1, T2, T3>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		void operator ()(T0 t0, T1 t1, T2 t2, T3 t3)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, false, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->pushArgument(t2);
				mScript->pushArgument(t3);
				mScript->endCall();
			}
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 3 parameters and a return value
	*/
	template <typename R, typename T0, typename T1, typename T2>
	class ScriptFunction<R, T0, T1, T2>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		R operator ()(T0 t0, T1 t1, T2 t2)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, true, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->pushArgument(t2);
				if (mScript->endCall())
				{
					boost::value_initialized<R> r;
					return (R)mScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return boost::value_initialized<R>();
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 3 parameters without a return value
	*/
	template <typename T0, typename T1, typename T2>
	class ScriptFunction<void, T0, T1, T2>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		void operator ()(T0 t0, T1 t1, T2 t2)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, false, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->pushArgument(t2);
				mScript->endCall();
			}
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 2 parameters and a return value
	*/
	template <typename R, typename T0, typename T1>
	class ScriptFunction<R, T0, T1>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		R operator ()(T0 t0, T1 t1)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, true, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				if (mScript->endCall())
				{
					boost::value_initialized<R> r;
					return (R)mScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return boost::value_initialized<R>();
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 2 parameters without a return value
	*/
	template <typename T0, typename T1>
	class ScriptFunction<void, T0, T1>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		void operator ()(T0 t0, T1 t1)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, false, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->pushArgument(t1);
				mScript->endCall();
			}
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 1 parameters and a return value
	*/
	template <typename R, typename T0>
	class ScriptFunction<R, T0>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		R operator ()(T0 t0)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, true, mNamespaceName))
			{
				mScript->pushArgument(t0);
				if (mScript->endCall())
				{
					boost::value_initialized<R> r;
					return (R)mScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return boost::value_initialized<R>();
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 1 parameters without a return value
	*/
	template <typename T0>
	class ScriptFunction<void, T0>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		void operator ()(T0 t0)
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, false, mNamespaceName))
			{
				mScript->pushArgument(t0);
				mScript->endCall();
			}
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 0 parameters and a return value
	*/
	template <typename R>
	class ScriptFunction<R>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		R operator ()()
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, true, mNamespaceName))
			{
				if (mScript->endCall())
				{
					boost::value_initialized<R> r;
					return (R)mScript->getReturn(r);	// C-style cast to be as flexible as possible in here
				}
			}
			return boost::value_initialized<R>();
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};

	/**
	*  @brief
	*    Script function pointer
	*
	*  @remarks
	*    Implementation for 0 parameters without a return value
	*/
	template <>
	class ScriptFunction<void>
	{
	public:
		ScriptFunction(Script* script, const std::string& functionName) :
			mScript(script),
			mFunctionName(functionName)
		{
			// Nothing to do in here
		}

		ScriptFunction(Script* script, const std::string& functionName, const std::string& namespaceName) :
			mScript(script),
			mFunctionName(functionName),
			mNamespaceName(namespaceName)
		{
			// Nothing to do in here
		}

		void operator ()()
		{
			if (nullptr != mScript && mScript->beginCall(mFunctionName, false, mNamespaceName))
			{
				mScript->endCall();
			}
		}

	private:
		Script*		mScript;		///< Script instance the function is in, can be a null pointer, do not destroy the instance
		std::string mFunctionName;	///< Name of the script function to use
		std::string mNamespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
