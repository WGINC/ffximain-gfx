module;

#include <format>
#include <cstdint>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkObList;

import KKXI.PrettyFormatter;

import KKXI.GameStructs.Tk.CTkNode;

export struct CTkObList
{
	CTkNode* m_pTopNode;
	CTkNode* m_pLastNode;
	CTkNode* m_pTopFreeNode;
	CTkNode* m_pLastFreeNode;
	int32_t m_Num;
	int32_t m_NodeNum;
	int32_t m_FreeNodeNum;

	REFLECT(
		REFLECT_MEMBER(CTkObList, m_pTopNode),
		REFLECT_MEMBER(CTkObList, m_pLastNode),
		REFLECT_MEMBER(CTkObList, m_pTopFreeNode),
		REFLECT_MEMBER(CTkObList, m_pLastFreeNode),
		REFLECT_MEMBER(CTkObList, m_Num),
		REFLECT_MEMBER(CTkObList, m_NodeNum),
		REFLECT_MEMBER(CTkObList, m_FreeNodeNum)
	)
};

template<>
struct std::formatter<CTkObList> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkObList& obj, FmtContext& fmtCtx) const
	{
		return PrettyFormatter::format<FmtContext, CTkObList>(obj, fmtCtx, "CTkObList");
	}
};