module;

#include <format>
#include <cstdint>

#include "SimpleReflectionMacro.h"

//TODO: TkMenuMng -> CTkMenuMng
export module KKXI.GameStructs.Tk.CTkMenuMng;

import KKXI.PrettyFormatter;

import KKXI.GameStructs.Tk.CTkObList;
import KKXI.GameStructs.Tk.CTkMenuCtrlData;
import KKXI.GameStructs.Tk.CTkMenuPrimitive;
import KKXI.GameStructs.Tk.TKRECT;
import KKXI.GameStructs.Tk.YmMenuShapeFile;

#pragma pack(push, 1)
export class CTkMenuMng
{
public:
	CTkObList m_DrawDatas;
	CTkObList m_ResourceDatas; //0x001C
	CTkObList m_HelpDatas; //0x0038
	CTkMenuCtrlData* m_pCurMCD; //0x0054
	CTkMenuCtrlData* m_pLastCurMCD; //0x0058
	CTkMenuCtrlData* m_pActiveMCD; //0x005C
	int8_t m_EnableSaveCursol; //0x0060
	uint8_t m_ActiveRequestNo; //0x0061
	uint8_t m_WindowTexNo; //0x0062
	int8_t m_LastIsEvent; //0x0063
	int32_t m_LastAppPhase; //0x0064
	int32_t m_LastGameStatus; //0x0068
	int16_t m_CurMenuPhase; //0x006C maybe
	char pad_006E[10]; //0x006E
	int32_t m_HelpDrawDelay; //0x0078 maybe
	TKRECT m_WinRect; //0x007C
	char pad_0084[8]; //0x0084
	CTkMenuPrimitive** m_chasewnd; //0x008C
	char pad_0090[20]; //0x0090
	int16_t m_chase_h[3]; //0x00A4
	int8_t m_LoginZone; //0x00AA
	int8_t m_IsLocketMouse; //0x00AB
	int8_t m_Setup; //0x00AC
	int8_t m_IsAllwaysMenuHide; //0x00AD
	int8_t m_StopCreate; //0x00AE
	int16_t m_LButtonRepeat; //0x00AF
	int8_t m_unkFlag; //0x00B1
	int8_t m_unkFlag2; //0x00B2
	int8_t m_HideModeLock; //0x00B3
	int8_t m_MenuHideMode; //0x00B4
	int8_t m_pad; //0x00B5
	int8_t m_pad2; //0x00B6
	int8_t m_pad3; //0x00B7
	void* m_FrameRes; //0x00B8
	YmMenuShapeFile* m_FrameShape; //0x00BC

	REFLECT(
		REFLECT_MEMBER(CTkMenuMng, m_DrawDatas),
		REFLECT_MEMBER(CTkMenuMng, m_ResourceDatas),
		REFLECT_MEMBER(CTkMenuMng, m_HelpDatas),
		REFLECT_MEMBER(CTkMenuMng, m_pCurMCD),
		REFLECT_MEMBER(CTkMenuMng, m_pLastCurMCD),
		REFLECT_MEMBER(CTkMenuMng, m_pActiveMCD),
		REFLECT_MEMBER(CTkMenuMng, m_EnableSaveCursol),
		REFLECT_MEMBER(CTkMenuMng, m_ActiveRequestNo),
		REFLECT_MEMBER(CTkMenuMng, m_WindowTexNo),
		REFLECT_MEMBER(CTkMenuMng, m_LastIsEvent),
		REFLECT_MEMBER(CTkMenuMng, m_LastAppPhase),
		REFLECT_MEMBER(CTkMenuMng, m_LastGameStatus),
		REFLECT_MEMBER(CTkMenuMng, m_CurMenuPhase),
		REFLECT_MEMBER(CTkMenuMng, m_HelpDrawDelay),
		REFLECT_MEMBER(CTkMenuMng, m_WinRect),
		REFLECT_MEMBER(CTkMenuMng, m_chasewnd),
		REFLECT_MEMBER(CTkMenuMng, m_chase_h),
		REFLECT_MEMBER(CTkMenuMng, m_LoginZone),
		REFLECT_MEMBER(CTkMenuMng, m_IsLocketMouse),
		REFLECT_MEMBER(CTkMenuMng, m_Setup),
		REFLECT_MEMBER(CTkMenuMng, m_IsAllwaysMenuHide),
		REFLECT_MEMBER(CTkMenuMng, m_StopCreate),
		REFLECT_MEMBER(CTkMenuMng, m_LButtonRepeat),
		REFLECT_MEMBER(CTkMenuMng, m_unkFlag),
		REFLECT_MEMBER(CTkMenuMng, m_unkFlag2),
		REFLECT_MEMBER(CTkMenuMng, m_HideModeLock),
		REFLECT_MEMBER(CTkMenuMng, m_MenuHideMode),
		REFLECT_MEMBER(CTkMenuMng, m_FrameRes),
		REFLECT_MEMBER(CTkMenuMng, m_FrameShape)
	)
}; //Size: 0x00C0
#pragma pop

template<>
struct std::formatter<CTkMenuMng> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuMng& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuMng>(obj, ctx, "CTkMenuMng");
	}
};