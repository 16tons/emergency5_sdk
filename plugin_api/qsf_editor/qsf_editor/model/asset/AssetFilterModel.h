// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qsortfilterproxymodel.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QRegExp;
QT_END_NAMESPACE
namespace qsf
{
	class Asset;
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
		*    Asset Qt filter model
		*/
		class AssetFilterModel : public QSortFilterProxyModel
		{


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] parent
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*/
			explicit AssetFilterModel(QObject* parent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetFilterModel();

			/**
			*  @brief
			*    Check whether or not the given filter accepts the given asset
			*
			*  @param[in] filter
			*    Filter to use
			*  @param[in] asset
			*    Asset to check
			*
			*  @return
			*    "true" if the asset is accepted by the filter, else "false"
			*/
			bool filterAcceptsAsset(const QRegExp& filter, const Asset& asset) const;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
