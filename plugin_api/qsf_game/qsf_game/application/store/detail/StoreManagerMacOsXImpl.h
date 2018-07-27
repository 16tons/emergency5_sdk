// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ C++ declarations and definitions                      ]
//[-------------------------------------------------------]
extern "C++"
{


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	namespace qsf
	{
		namespace game
		{
			class StoreManagerMacOsX;

			namespace macosx
			{

				void setStoreManagerInstance(StoreManagerMacOsX* storeManager);
				void validateReceipt();
				bool initializeStore(const std::vector<std::string>& productIds);
				bool restoreBoughtProducts();
				bool startPurchase(const char* productId);

			}
		}
	}
}
