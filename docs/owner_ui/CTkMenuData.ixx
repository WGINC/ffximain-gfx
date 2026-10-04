module;

#include <format>
#include <cstdint>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkMenuData;

import KKXI.PrettyFormatter;

import KKXI.GameStructs.Tk.CTkMenuButton;
import KKXI.GameStructs.Tk.CTkMenuFrame;
import KKXI.GameStructs.Tk.CTkMenuCursol;
import KKXI.GameStructs.Tk.TKRECT;
import KKXI.GameStructs.Tk.CTkDrawShapeData;

export class CTkMenuData
{
public:
	int16_t m_RecKind; //0x0000 part of : CTkObject
	int16_t m_RecSub1; //0x0002 part of : CTkObject
	CTkMenuButton* m_pTkMenuButtons; //0x0004
	CTkMenuFrame* m_pTkMenuFrame; //0x0008
	CTkMenuCursol* m_pTkMenuCursol; //0x000C
	TKRECT m_WinRect; //0x0010
	int16_t m_ButtonNum; //0x0018
	int16_t m_LastCursolNo; //0x001A
	int16_t m_LastCursolOption; //0x001C
	CTkDrawShapeData m_DSD; //0x001E
	char m_Id[32]; //0x0046

	REFLECT(
		REFLECT_MEMBER(CTkMenuData, m_RecKind),
		REFLECT_MEMBER(CTkMenuData, m_RecSub1),
		REFLECT_MEMBER(CTkMenuData, m_pTkMenuButtons),
		REFLECT_MEMBER(CTkMenuData, m_pTkMenuFrame),
		REFLECT_MEMBER(CTkMenuData, m_pTkMenuCursol),
		REFLECT_MEMBER(CTkMenuData, m_WinRect),
		REFLECT_MEMBER(CTkMenuData, m_ButtonNum),
		REFLECT_MEMBER(CTkMenuData, m_LastCursolNo),
		REFLECT_MEMBER(CTkMenuData, m_LastCursolOption),
		REFLECT_MEMBER(CTkMenuData, m_DSD),
		REFLECT_MEMBER(CTkMenuData, m_Id)
	)
}; //Size: 0x0066

template<>
struct std::formatter<CTkMenuData> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuData& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuData>(obj, ctx, "CTkMenuData");
	}
};
