// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/NamedIdentifier.h>

#include <boost/container/flat_set.hpp>
#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class StoreManagerImpl;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Store manager class managing in-app-purchases on platforms supporting this
		*/
		class QSF_GAME_API_EXPORT StoreManager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef uint32 ProductId;
			typedef boost::container::flat_set<ProductId> ProductsBoughtSet;


		//[-------------------------------------------------------]
		//[ Public boost signals                                  ]
		//[-------------------------------------------------------]
		public:
			// Single purchase
			boost::signals2::signal<void(ProductId)> PurchaseSuccess;
			boost::signals2::signal<void(ProductId)> PurchaseRestore;
			boost::signals2::signal<void(ProductId)> PurchaseCancel;
			boost::signals2::signal<void(ProductId)> PurchaseError;

			// Restore of all purchases
			boost::signals2::signal<void()> RestoreSuccess;
			boost::signals2::signal<void()> RestoreFailed;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static StoreManager& createStoreManagerInstance();


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			StoreManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~StoreManager();

			/**
			*  @brief
			*    Return a set of product IDs already bought
			*/
			const ProductsBoughtSet& getProductsBought() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::StoreManager methods        ]
		//[-------------------------------------------------------]
		public:
			virtual void onStartup() = 0;
			virtual void onShutdown() = 0;

			virtual bool arePurchasesAllowed();

			virtual void registerProduct(const NamedIdentifier& productId);

			virtual bool restoreBoughtProducts();

			virtual bool startPurchase(ProductId productId);

			virtual bool isProductAvailable(ProductId productId);
			virtual bool isProductBought(ProductId productId);

			virtual std::string getProductPriceFormatted(ProductId productId);


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			ProductsBoughtSet mProductsBought;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
