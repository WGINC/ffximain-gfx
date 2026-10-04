module;

#include <format>
#include <cstdint>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkMenuPrimitive;

import KKXI.PrettyFormatter;

class CTkMenuCtrlData;

export class CTkMenuPrimitive
{
public:
	uint32_t m_vftable; //0x0000
	int16_t m_RecKind; //0x0004 part of : CTkObject
	int16_t m_RecSub1; //0x0006 part of : CTkObject
	CTkMenuCtrlData* m_pParentMCD; //0x0008
	int8_t m_InputEnable; //0x000C
	int8_t m_SaveCursol; //0x000D unsure
	int16_t m_Reposition; //0x000E unsure

	REFLECT(
		REFLECT_MEMBER(CTkMenuPrimitive, m_vftable),
		REFLECT_MEMBER(CTkMenuPrimitive, m_RecKind),
		REFLECT_MEMBER(CTkMenuPrimitive, m_RecSub1),
		REFLECT_MEMBER(CTkMenuPrimitive, m_pParentMCD),
		REFLECT_MEMBER(CTkMenuPrimitive, m_InputEnable),
		REFLECT_MEMBER(CTkMenuPrimitive, m_SaveCursol),
		REFLECT_MEMBER(CTkMenuPrimitive, m_Reposition)
	)
}; //Size: 0x0010

template<>
struct std::formatter<CTkMenuPrimitive> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuPrimitive& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuPrimitive>(obj, ctx, "CTkMenuPrimitive");
	}
};