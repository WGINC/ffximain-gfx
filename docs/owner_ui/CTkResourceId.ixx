module;

#include <cstdint>
#include <format>
#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkResourceId;

import KKXI.PrettyFormatter;
import KKXI.GameStructs.Tk.YkMenuShape;

export class CTkResourceId
{
public:
	int16_t m_Kind; //0x0000
	int16_t m_RscNum; //0x0002
	char m_RscId[20]; //0x0004
	YkMenuShape* m_Shapes; //0x0018

	REFLECT(
		REFLECT_MEMBER(CTkResourceId, m_Kind),
		REFLECT_MEMBER(CTkResourceId, m_RscNum),
		REFLECT_MEMBER(CTkResourceId, m_RscId)
	)
}; //Size: 0x001C

template<>
struct std::formatter<CTkResourceId> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkResourceId& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkResourceId>(obj, ctx, "CTkResourceId");
	}
};