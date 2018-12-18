// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/application/store/StoreManager.h"


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
		*    Store manager implementation for Mac App Store
		*/
		class StoreManagerMacOsX : public StoreManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			StoreManagerMacOsX();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~StoreManagerMacOsX();

			// The following methods are meant to be called from the "StoreManagerMacOsXImpl.mm"
			void onTransactionPurchased(const char* productName);
			void onTransactionRestored(const char* productName);
			void onTransactionFailed(const char* productName, bool userCanceled);
			void unlockProduct(const char* productName);
			void onRestoreSuccess();
			void onRestoreFailed();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::StoreManager methods        ]
		//[-------------------------------------------------------]
		public:
			virtual void onStartup() override;
			virtual void onShutdown() override;

			virtual bool arePurchasesAllowed() override;

			virtual void registerProduct(const NamedIdentifier& productId) override;

			virtual bool restoreBoughtProducts() override;

			virtual bool startPurchase(ProductId productId) override;

			virtual bool isProductAvailable(ProductId productId) override;
			virtual bool isProductBought(ProductId productId) override;

//			virtual std::string getProductPriceFormatted(ProductId productId) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct ProductInfo
			{
				std::string mName;		///< Technical identifier of product, in iTunesConnect "Product ID"
			};


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mPurchasesAllowed;
			std::map<ProductId, ProductInfo> mKnownProducts;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
