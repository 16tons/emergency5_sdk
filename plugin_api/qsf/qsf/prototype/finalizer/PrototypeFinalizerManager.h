// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/pluginable/PluginableManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BasePrototypeManager;
	class PrototypeFinalizer;
	struct PrototypeFinalizerSettings;
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
	*    Prototype finalizer pluginable manager
	*
	*  @remarks
	*    Collects prototype finalizer class instances used to correct inconcistencies or similar in maps and prototype managers
	*/
	class QSF_API_EXPORT PrototypeFinalizerManager : public PluginableManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PrototypeFinalizerManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PrototypeFinalizerManager();

		/**
		*  @brief
		*    Process all prototypes
		*/
		void processPrototypes(BasePrototypeManager& prototypeManager, const PrototypeFinalizerSettings& settings) const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PluginableManager methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void classAdded(const camp::Class& added) override;
		virtual void createPluginableInstance(const camp::Class& added, camp::UserObject& campUserObject) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PrototypeFinalizerManager)
