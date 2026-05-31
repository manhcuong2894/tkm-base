#include "stdafx.h"
#include "ScriptManager.h"

void EFFECT_ENTRY::parser(CMemScript* script, EFFECT_ENTRY& info)
{
	info.KeyIndex = script->GetNumber();
	info.EfectType = script->GetAsNumber();
	info.BoneIndex = script->GetAsNumber();

	script->GetEmpty();

	info.EffectColor = script->GetAsCLRREFF();
	info.EffectSize = script->GetAsFloatNumber();
	info.EffectAngle = script->GetAsFloatNumber();
	info.EffectSubType = script->GetAsNumber();
}

void EFFECT_ENTRY::parserItem(CMemScript* script, EFFECT_ENTRY& info)
{
	int _Type = script->GetNumber();
	int _Index = script->GetAsNumber();

	info.KeyIndex = (_Type << 9) + _Index;
	info.EfectType = script->GetAsNumber();
	info.BoneIndex = script->GetAsNumber();

	script->GetEmpty();

	info.EffectColor = script->GetAsCLRREFF();
	info.EffectSize = script->GetAsFloatNumber();
	info.EffectAngle = script->GetAsFloatNumber();
	info.EffectSubType = script->GetAsNumber();
}