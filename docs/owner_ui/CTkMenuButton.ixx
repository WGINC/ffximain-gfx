module;

#include <format>

#include "SimpleReflectionMacro.h"

export module KKXI.GameStructs.Tk.CTkMenuButton;

import KKXI.PrettyFormatter;

import KKXI.GameStructs.Tk.BRSOURCEREC;
import KKXI.GameStructs.Tk.CTkResourceId;

export class CTkMenuButton
{
public:
	BRSOURCEREC* m_pRR; //0x0000
	CTkResourceId* m_pResources; //0x0004
	char* m_pHelpMsg; //0x0008
	char* m_pTitleMsg; //0x000C
	char* m_Command; //0x0010

	REFLECT(
		REFLECT_MEMBER(CTkMenuButton, m_pRR),
		REFLECT_MEMBER(CTkMenuButton, m_pResources),
		REFLECT_MEMBER(CTkMenuButton, m_pHelpMsg),
		REFLECT_MEMBER(CTkMenuButton, m_pTitleMsg),
		REFLECT_MEMBER(CTkMenuButton, m_Command)
	)
}; //Size: 0x0014

template<>
struct std::formatter<CTkMenuButton> : PrettyFormatter
{
	template <class FmtContext>
	typename FmtContext::iterator format(const CTkMenuButton& obj, FmtContext& ctx) const
	{
		return PrettyFormatter::format<FmtContext, CTkMenuButton>(obj, ctx, "CTkMenuButton");
	}
};