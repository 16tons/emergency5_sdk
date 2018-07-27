// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/pluginable/Pluginable.h"
#include "qsf/prototype/finalizer/PrototypeFinalizerManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BasePrototypeManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	struct PrototypeFinalizerSettings
	{
		bool mIsOnServer;	///< "true" if the processing is performed on the server, as hidden changes that don't get transmitted to the clients as operations
		bool mIsMap;		///< "true" if the prototype manager is a map and contains entities

		PrototypeFinalizerSettings() : mIsOnServer(false), mIsMap(false) {}
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Prototype finalizer base class
	*/
	class QSF_API_EXPORT PrototypeFinalizer : public Pluginable<PrototypeFinalizerManager>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototypeFinalizerManager
		*    The manager this prototype finalizer instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
		*/
		explicit PrototypeFinalizer(PrototypeFinalizerManager* prototypeFinalizerManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrototypeFinalizer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::PrototypeFinalizer methods        ]
	//[-------------------------------------------------------]
	public:
		virtual void processPrototypes(BasePrototypeManager& prototypeManager, const PrototypeFinalizerSettings& settings) = 0;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PrototypeFinalizer)
