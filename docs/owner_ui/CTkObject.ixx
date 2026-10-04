module;

#include <cstdint>
#include <format>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkObject;

import KKXI.PrettyFormatter;

export struct CTkObject
{
	int16_t m_RecKind;
	int16_t m_RecSub1;

	REFLECT(
		REFLECT_MEMBER(CTkObject, m_RecKind),
		REFLECT_MEMBER(CTkObject, m_RecSub1)
	)
};

template<>
struct std::formatter<CTkObject> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkObject& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkObject>(obj, ctx, "CTkObject");
	}
};