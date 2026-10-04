module;

#include <format>
#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkMenuFrame;

import KKXI.GameStructs.Tk.FRSOURCEREC;
import KKXI.GameStructs.Tk.CTkResourceId;

import KKXI.PrettyFormatter;

export class CTkMenuFrame
{
public:
	FRSOURCEREC* m_pRR; //0x0000
	CTkResourceId* m_pResources; //0x0004
	char* m_pHelpMsg; //0x0008
	char* m_pTitleMsg; //0x000C

	REFLECT(
		REFLECT_MEMBER(CTkMenuFrame, m_pRR),
		REFLECT_MEMBER(CTkMenuFrame, m_pResources),
		REFLECT_MEMBER(CTkMenuFrame, m_pHelpMsg),
		REFLECT_MEMBER(CTkMenuFrame, m_pTitleMsg)
	)
}; //Size: 0x0010

template<>
struct std::formatter<CTkMenuFrame> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuFrame& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuFrame>(obj, ctx, "CTkMenuFrame");
	}
};