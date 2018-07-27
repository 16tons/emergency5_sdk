// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/*! \class BLSingleton
		 *	\brief Template implementation of the Singleton pattern
		 */
		template <typename T> class BLSingleton
		{
			friend class BLSingleton<T>;
			private:
				static T *pInstance;
				static bool mDestroying;
			public:
				virtual ~BLSingleton()
				{
					if (!mDestroying)
						Destroy();
				};
				static T *Instance ();
				static bool IsValid() { return pInstance ? true : false; }
				static void Create();
				static void Destroy();
				T* operator->() { QSF_ASSERT(pInstance, "BLSingleton<T>: No instance", QSF_REACT_NONE); return pInstance; /* return Instance();*/ }
				static bool IsInstantiated() { return pInstance != nullptr; }
		};

		template <typename T> bool BLSingleton<T>::mDestroying = false;
		template <typename T> T* BLSingleton<T>::pInstance = nullptr;

		template <typename T> T* BLSingleton<T>::Instance()
		{
			return pInstance = pInstance ? pInstance : new T();
		}

		template <typename T> void BLSingleton<T>::Create()
		{
			QSF_ASSERT(!pInstance, "BLSingleton<T>::Create(): Has already an instance", QSF_REACT_NONE);
			pInstance = pInstance ? pInstance : new T();
		}

		template <typename T> void BLSingleton<T>::Destroy()
		{
			QSF_ASSERT(pInstance, " BLSingleton<T>::Destroy(): Has no instance", QSF_REACT_NONE);
			mDestroying = true;
			delete pInstance;
			pInstance = nullptr;
		}

		#define DECLARESINGLETON(class__) friend class BLSingleton<class__>


		template <typename T> class BLAutoSingleton
		{
			friend class BLAutoSingleton<T>;
			private:
				static T *pInstance;
				static bool mDestroying;
			public:
				virtual ~BLAutoSingleton()
				{
					if (!mDestroying)
						Destroy();
				};
				static T *Instance ();
				static bool IsValid() { return pInstance ? true : false; }
				static void Create();
				static void Destroy();
				T* operator->() { return Instance(); }
				static bool IsInstantiated() { return pInstance != nullptr; }
		};

		template <typename T> bool BLAutoSingleton<T>::mDestroying = false;
		template <typename T> T* BLAutoSingleton<T>::pInstance = nullptr;

		template <typename T> T* BLAutoSingleton<T>::Instance()
		{
			return pInstance = pInstance ? pInstance : new T();
		}

		template <typename T> void BLAutoSingleton<T>::Create()
		{
			pInstance = pInstance ? pInstance : new T();
		}

		template <typename T> void BLAutoSingleton<T>::Destroy()
		{
			mDestroying = true;
			delete pInstance;
			pInstance = nullptr;
		}

		#define DECLAREAUTOSINGLETON(class__) friend class BLAutoSingleton<class__>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
