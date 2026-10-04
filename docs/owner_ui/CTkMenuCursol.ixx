module;

#include <cstdint>
#include <format>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkMenuCursol;

import KKXI.PrettyFormatter;

export class CTkMenuCursol
{
public:
	char m_ShapeId1[8]; //0x0000
	char m_ShapeId2[8]; //0x0008
	int16_t m_ShapeNum; //0x0010
	int16_t m_Y; //0x0012
	int16_t m_X; //0x0014

	REFLECT(
		REFLECT_MEMBER(CTkMenuCursol, m_ShapeId1),
		REFLECT_MEMBER(CTkMenuCursol, m_ShapeId2),
		REFLECT_MEMBER(CTkMenuCursol, m_ShapeNum),
		REFLECT_MEMBER(CTkMenuCursol, m_Y),
		REFLECT_MEMBER(CTkMenuCursol, m_X)
	)
}; //Size: 0x0016

template<>
struct std::formatter<CTkMenuCursol> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuCursol& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuCursol>(obj, ctx, "CTkMenuCursol");
	}
};
