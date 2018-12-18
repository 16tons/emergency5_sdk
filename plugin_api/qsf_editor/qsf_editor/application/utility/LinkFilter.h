// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qobject.h>


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
		*    This class filters the "QEvent::WhatsThisClicked" event and opens an external website with the clicked hyberlink
		*
		*  @remarks
		*    The "What's This"-tooltip needs a href as information, Qt displays this in his way in catches the click as event.
		*    Use <a href'www.yourspeciallink.com'>Click here</a> to insert tags with html format.
		*/
		class LinkFilter : public QObject
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit LinkFilter(QObject* parent = nullptr);
			virtual ~LinkFilter();


		//[-------------------------------------------------------]
		//[ Public virtual QObject methods                        ]
		//[-------------------------------------------------------]
		public:
			virtual bool eventFilter(QObject* qObject, QEvent* qEvent) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
