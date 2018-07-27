// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/manager/Manager.h>

#include <QtCore/qobject.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
	class DebugDrawProxy;
	class ParameterGroup;
	class ParticlesComponent;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Icon manager class
		*
		*  @todo
		*    - TODO(mk) Currently we don't support entities with multiple icons, because operations received only for the parent entities. We have to get through all child entities and do:
		*    - TODO(mk) Transform/ ParticleAsset changing / component deleting
		*    - TODO(co) On big maps, the performance is horrible with icons enabled, so icons are disabled by default at the moment
		*/
		class IconManager : public QObject, public Manager
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			IconManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~IconManager();

			void rescanMapForComponentsWithIcons();
			void disableAllIcons();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void createDrawProxyForParticleComponent(ParticlesComponent& particleComponent);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::vector<std::pair<Component*, DebugDrawProxy*>> ComponentProxyList;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ComponentProxyList	mComponentProxyList;	///< List of components there proxy that are interesting for drawing icons


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
