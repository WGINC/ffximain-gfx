uintptr_t __stdcall hkGetFromPrimTable(const char* menuName, const char* convertedMenuName)
{
	const auto nameStr = std::string(menuName, 16);

	const auto gameFunctionManager = DiContainer::Instance().Resolve<GameFunctionManager>();
	const auto result = gameFunctionManager->Call<GameFunctions::TkMenuManagerGetFromPrimTable,
		TkMenuManagerFunctionDefinitions::fnGetFromPrimTable, uintptr_t>(
		menuName, convertedMenuName);

	const auto gameUiManager = DiContainer::Instance().Resolve<GameUiManager>();
	if(gameUiManager)
	{
		gameUiManager->AddNewPrimMenuName(nameStr);
	}

	return result;
}

bool __fastcall hkIsEnableCreateDrawMenu(uintptr_t* TkMenuMngr, void* EDX, char* menuName)
{
	const auto gameFunctionManager = DiContainer::Instance().Resolve<GameFunctionManager>();
	const auto result = gameFunctionManager->Call<GameFunctions::TkMenuManagerIsEnableCreateDrawMenu,
		TkMenuManagerFunctionDefinitions::fnIsEnableCreateDrawMenu, bool>(
		TkMenuMngr, menuName);

	return result;
}

uintptr_t __fastcall hkGetResourceMenu(void* TkMenuMngr, void* EDX, const char* menuName)
{
	if(!gTkMenuMngr)
	{
		gTkMenuMngr = TkMenuMngr;
	}

	const auto gameUiManager = DiContainer::Instance().Resolve<GameUiManager>();
	if(gameUiManager)
	{
		gameUiManager->AddNewResourceMenuName(std::string(menuName, 16));
	}

	const auto gameFunctionManager = DiContainer::Instance().Resolve<GameFunctionManager>();
	const auto result = gameFunctionManager->Call<GameFunctions::TkMenuManagerGetResourceMenu,
		TkMenuManagerFunctionDefinitions::fnGetResourceMenu, uintptr_t>(
		TkMenuMngr, menuName);

	return result;
}

uintptr_t __fastcall hkGetMenuCtrlData(void* TkMenuMngr, void* EDX, const char* menuName)
{
	const auto gameFunctionManager = DiContainer::Instance().Resolve<GameFunctionManager>();
	const auto result = gameFunctionManager->Call<GameFunctions::TkMenuManagerGetMenuControlData,
		TkMenuManagerFunctionDefinitions::fnGetMenuCtrlData, uintptr_t>(
		TkMenuMngr, menuName);
	
	const auto gameUiManager = DiContainer::Instance().Resolve<GameUiManager>();
	if(gameUiManager)
	{
		gameUiManager->AddNewMenuControlDataName(std::string(menuName, 16));
	}

	return result;
}

uintptr_t __fastcall hkReposition(void* menuCtrlData, void* EDX, int16_t x, int16_t y)
{
	const auto settingsManager = DiContainer::Instance().Resolve<SettingsManager>();
	const auto gameUiOverrideSettings = settingsManager->GetSettings<GameUiOverrideSettings>(SettingsType::GameUiOverrides);
	const auto gameUiManager = DiContainer::Instance().Resolve<GameUiManager>();
	const auto gameFunctionManager = DiContainer::Instance().Resolve<GameFunctionManager>();

	if(const auto menuName = gameUiManager->IsOverrideMenuCtrlDataPointer(gTkMenuMngr, reinterpret_cast<uintptr_t>(menuCtrlData)); !menuName.empty())
	{
		for(const auto& menuOverrideSettings : gameUiOverrideSettings.m_MenuOverrides)
		{
			if(menuName == menuOverrideSettings.m_MenuName)
			{
				return gameFunctionManager->Call<GameFunctions::TkMenuManagerTkMenuCtrlDataReposition,
					TkMenuManagerFunctionDefinitions::fnTkMenuCtrlDataRePosition, uintptr_t>(
					menuCtrlData, menuOverrideSettings.m_OverridePosition[0], menuOverrideSettings.m_OverridePosition[1]);
			}
		}
	}

	return gameFunctionManager->Call<GameFunctions::TkMenuManagerTkMenuCtrlDataReposition,
		TkMenuManagerFunctionDefinitions::fnTkMenuCtrlDataRePosition, uintptr_t>(
		menuCtrlData, x, y);
}