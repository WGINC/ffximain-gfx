export namespace TkMenuManagerFunctionDefinitions
{
    using fnCreateDrawMenu = int32_t * *(__thiscall*)(void* tkMenuManager, const char* menuName, char, char);
    using fnCloseDrawMenu = int(__thiscall*)(void* tkMenuManager, int a2, char a3, char a4, char a5, char a6);
    using fnConvertMenuId = char(__thiscall*)(void* tkMenuManager, char* outStr, char* menuStr);//todo: can probably make string args const
    using fnGetFromPrimTable = uintptr_t(__stdcall*)(const char* menuName, const char* convertedMenuName);
    using fnIsEnableCreateDrawMenu = bool(__thiscall*)(uintptr_t* TkMenuManager, char* menuName);
    using fnGetResourceMenu = uintptr_t(__thiscall*)(void* TkMenuManager, const char* menuName);
    using fnGetMenuCtrlData = uintptr_t(__thiscall*)(void* TkMenuManager, const char* menuName);
    using fnTkMenuCtrlDataRePosition = uintptr_t(__thiscall*)(void* TkMenuCtrlData, int16_t x, int16_t y);
    using fnTkMenuCtrlDataResize = char(__thiscall*)(void* TkMenuCtrlData, int16_t xPos, int16_t yPos, int16_t xExtent, int16_t yExtent, int a6, char a7, char a8);
    using fnDestroyDrawMenu = void(__thiscall*)(CTkMenuMng* tkMenuManager, const char* pId);
    using fnOnKeyDown = char* (__thiscall*)(CTkMenuMng* tkMenuManager, int32_t keyCode);//CTkMenuMng::OnKeyDown
}
