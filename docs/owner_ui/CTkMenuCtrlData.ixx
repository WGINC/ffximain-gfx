module;

#include <format>
#include <cstdint>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkMenuCtrlData;

import KKXI.PrettyFormatter;

import KKXI.GameStructs.Tk.CTkObject;
import KKXI.GameStructs.Tk.CTkMenuData;
import KKXI.GameStructs.Tk.CTkDrawCtrlButton;
import KKXI.GameStructs.Tk.CTkMenuPrimitive;
import KKXI.GameStructs.Tk.KaScrollBar;
import KKXI.GameStructs.Tk.TKRECT;
import KKXI.GameStructs.Tk.CTkObList;

export struct CTkMenuCtrlData
{
	CTkObject m_CTkObject;
	CTkMenuData* m_pMenuData;
	CTkDrawCtrlButton* m_pCurDrawCtrlButton;
	CTkMenuPrimitive* m_pMenuPrimitive;
	uint32_t m_MenuCtrlID; //0x0010 ENUM_MENUCTRL_ID
	CTkObList m_DrawShapes; //0x0014
	char pad_0030[10]; //0x0030
	TKRECT m_WinRect; //0x003A
	TKRECT m_OrgWinRect; //0x0042
	int16_t m_StartCursolNum; //0x004A
	int16_t m_CursolNum; //0x004C
	int16_t m_CursolOption; //0x004E
	int16_t m_DelayTime; //0x0050
	int16_t m_LocX; //0x0052
	int16_t m_LocY; //0x0054
	int16_t m_InputDelay; //0x0056
	int16_t m_ButtonSuu; //0x0058
	int16_t m_OnKey_AtDisableMove; //0x005A
	int8_t m_MenuCtrlStatus; //0x005C
	int8_t m_IsWaiting; //0x005D
	int16_t m_ActiveButtonNeckTime; //0x005E
	int8_t m_Priority; //0x0060
	int8_t m_EnableCursolMove; //0x0061
	int8_t m_EnableCursolDraw; //0x0062
	int8_t m_EnableButtonDrawSelect; //0x0063
	int16_t m_CloseWindowNeckTime; //0x0064
	int16_t m_CloseWaitNeckTime; //0x0066
	int8_t m_EnableFrameAnim; //0x0068
	int8_t m_EnableWindowDraw; //0x0069
	int8_t m_EnablePrimitiveDraw; //0x006A
	int8_t m_IsBlink; //0x006B
	int8_t m_IsMostClearTitle; //0x006C
	int8_t m_IsMostClearHelp; //0x006D
	int8_t m_RequestDestroy; //0x006E
	int8_t m_TargetLockIncCnt; //0x006F
	int8_t m_WishDelete; //0x0070
	uint8_t m_ActiveRequestNo; //0x0071
	int8_t m_IsLockCurMCD; //0x0072
	int8_t m_IsPopUp; //0x0073
	int8_t m_IsDrawScrollBar; //0x0074
	int8_t m_IsExecluseAuto; //0x0075
	int8_t m_IsReposiionMCD; //0x0076
	int8_t m_HaveMouseCtrl; //0x0077
	char pad_0078[16]; //0x0078
	KaScrollBar* m_pKaScrollBar; //0x0088
	int16_t m_ReqX; //0x008C
	int16_t m_ReqY; //0x008E
	int16_t m_ReqW; //0x0090
	int16_t m_ReqH; //0x0092
	int32_t m_LastResizeNeck; //0x0094 unsure about the req and this
	int8_t m_IsCarriageActive; //0x0098
	int8_t m_IsZoom; //0x0099
	int8_t m_AutoCursorWidth; //0x009A
	int8_t m_IsSizeWindow; //0x009B this and below are somehow related to resize windows, seems to use m_SizeWinRect and assocaited variables when this is 1
	TKRECT m_SizeWinRect; //0x009C
	int32_t m_SizeL; //0x00A4
	int32_t m_SizeT; //0x00A8
	int32_t m_SizeW; //0x00AC
	int32_t m_SizeH; //0x00B0
	int32_t m_SizeOrgH; //0x00B4
	int32_t m_SizeDrawY; //0x00B8
	TKRECT m_AreaRect; //0x00BC
	int8_t m_IsNoFrame; //0x00C4

	REFLECT(
		REFLECT_MEMBER(CTkMenuCtrlData, m_CTkObject),
		REFLECT_MEMBER(CTkMenuCtrlData, m_pMenuData),
		REFLECT_MEMBER(CTkMenuCtrlData, m_pCurDrawCtrlButton),
		REFLECT_MEMBER(CTkMenuCtrlData, m_pMenuPrimitive),
		REFLECT_MEMBER(CTkMenuCtrlData, m_MenuCtrlID),
		REFLECT_MEMBER(CTkMenuCtrlData, m_DrawShapes),
		REFLECT_MEMBER(CTkMenuCtrlData, m_WinRect),
		REFLECT_MEMBER(CTkMenuCtrlData, m_OrgWinRect),
		REFLECT_MEMBER(CTkMenuCtrlData, m_StartCursolNum),
		REFLECT_MEMBER(CTkMenuCtrlData, m_CursolNum),
		REFLECT_MEMBER(CTkMenuCtrlData, m_CursolOption),
		REFLECT_MEMBER(CTkMenuCtrlData, m_DelayTime),
		REFLECT_MEMBER(CTkMenuCtrlData, m_LocX),
		REFLECT_MEMBER(CTkMenuCtrlData, m_LocY),
		REFLECT_MEMBER(CTkMenuCtrlData, m_InputDelay),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ButtonSuu),
		REFLECT_MEMBER(CTkMenuCtrlData, m_OnKey_AtDisableMove),
		REFLECT_MEMBER(CTkMenuCtrlData, m_MenuCtrlStatus),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsWaiting),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ActiveButtonNeckTime),
		REFLECT_MEMBER(CTkMenuCtrlData, m_Priority),
		REFLECT_MEMBER(CTkMenuCtrlData, m_EnableCursolMove),
		REFLECT_MEMBER(CTkMenuCtrlData, m_EnableCursolDraw),
		REFLECT_MEMBER(CTkMenuCtrlData, m_EnableButtonDrawSelect),
		REFLECT_MEMBER(CTkMenuCtrlData, m_CloseWindowNeckTime),
		REFLECT_MEMBER(CTkMenuCtrlData, m_CloseWaitNeckTime),
		REFLECT_MEMBER(CTkMenuCtrlData, m_EnableFrameAnim),
		REFLECT_MEMBER(CTkMenuCtrlData, m_EnableWindowDraw),
		REFLECT_MEMBER(CTkMenuCtrlData, m_EnablePrimitiveDraw),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsBlink),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsMostClearTitle),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsMostClearHelp),
		REFLECT_MEMBER(CTkMenuCtrlData, m_RequestDestroy),
		REFLECT_MEMBER(CTkMenuCtrlData, m_TargetLockIncCnt),
		REFLECT_MEMBER(CTkMenuCtrlData, m_WishDelete),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ActiveRequestNo),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsLockCurMCD),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsPopUp),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsDrawScrollBar),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsExecluseAuto),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsReposiionMCD),
		REFLECT_MEMBER(CTkMenuCtrlData, m_HaveMouseCtrl),
		REFLECT_MEMBER(CTkMenuCtrlData, m_pKaScrollBar),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ReqX),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ReqY),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ReqW),
		REFLECT_MEMBER(CTkMenuCtrlData, m_ReqH),
		REFLECT_MEMBER(CTkMenuCtrlData, m_LastResizeNeck),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsCarriageActive),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsZoom),
		REFLECT_MEMBER(CTkMenuCtrlData, m_AutoCursorWidth),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsSizeWindow),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeWinRect),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeL),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeT),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeW),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeH),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeOrgH),
		REFLECT_MEMBER(CTkMenuCtrlData, m_SizeDrawY),
		REFLECT_MEMBER(CTkMenuCtrlData, m_AreaRect),
		REFLECT_MEMBER(CTkMenuCtrlData, m_IsNoFrame)
	)
};

template<>
struct std::formatter<CTkMenuCtrlData> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuCtrlData& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuCtrlData>(obj, ctx, "CTkMenuCtrlData");
	}
};