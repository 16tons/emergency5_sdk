// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/selection/SelectionMarkerManager.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DebugDrawProxy;
	namespace editor
	{
		class NodeSelectionManager;
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
		*    QSF editor node selection marker manager class that cares for activation and deactivation of markers
		*/
		class NodeSelectionMarkerManager : public QObject, public SelectionMarkerManager
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
			*    Constructor
			*
			*  @param[in] nodeSelectionManager
			*    QSF editor node selection manager to spy on, must stay valid as long as this selection marker manager instance exists
			*/
			explicit NodeSelectionMarkerManager(NodeSelectionManager& nodeSelectionManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NodeSelectionMarkerManager();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void createAllMarkers();
			void destroyAllMarkers();
			void onSelected(unsigned int id);
			void onDeselected(unsigned int id);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef boost::container::flat_map<uint32, DebugDrawProxy*> DebugDrawProxyMap;	///< Key = node index, value = debug draw proxy instance


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NodeSelectionManager& mNodeSelectionManager;	///< QSF editor node selection manager to spy on, do not destroy the instance
			DebugDrawProxyMap	  mDebugDrawProxyMap;		///< Node debug draw proxies, key = node index, value = debug draw proxy instance (we're responsible for destroying the instances)


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
