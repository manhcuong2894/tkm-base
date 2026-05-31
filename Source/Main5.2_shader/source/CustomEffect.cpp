#include "stdafx.h"
#include "ZzzEffect.h"
#include "MonkSystem.h"
#include "CustomEffect.h"

void CustomEffect::LoadEffects(FILE* fp, int Type)
{
	SManager<SEFFECT> effectList;

	effectList.LoadResources(fp);

	for (auto& e : effectList.AllItems())
	{
		RegisterEffect(e, Type);
	}
}

void CustomEffect::RegisterEffect(SEFFECT info, int STATE)
{
	auto& list = EffectByIndex[info.KeyIndex]; // crea si no existe
	info.RenderType = STATE;
	list.effects.emplace_back(std::move(info));

	/*auto it = EffectByIndex.find(info.KeyIndex);

	if (it == EffectByIndex.end())
	{
		EffectList newCont;
		info.RenderType = 0;
		newCont.effects.push_back(info);
		EffectByIndex[info.KeyIndex] = newCont;
	}
	else
	{
		info.RenderType = 0;
		it->second.effects.push_back(info);
	}*/
}

MONSTEREFFECT::MONSTEREFFECT()
{
}

MONSTEREFFECT::~MONSTEREFFECT()
{
	EffectByIndex.clear();
}

MONSTEREFFECT& MONSTEREFFECT::Instance()
{
	static MONSTEREFFECT s_instance;
	return s_instance;
}

bool MONSTEREFFECT::RenderEffectVisual(BMD* pModel, CHARACTER* sCharacter, OBJECT* pObject)
{
	auto it = EffectByIndex.find(sCharacter->MonsterIndex);

	if (it != EffectByIndex.end())
	{
		for (auto& eff : it->second.effects)
		{
			vec4_t* bonetransform = pObject->GetBoneTransform(eff.BoneIndex);

			if (bonetransform)
			{
				vec3_t v3EffectPosition, v3EffectColor;
				vec3_t position = { 0.f, 0.f, 0.f };

				eff.GetColor(v3EffectColor);

				pModel->TransformPosition(bonetransform, position, v3EffectPosition, true);

				if (eff.RenderType != RENDER_PARTICLE)
					CreateSprite(eff.EfectType, v3EffectPosition, eff.EffectSize, v3EffectColor, pObject, eff.EffectAngle, eff.EffectSubType);
				else
					CreateParticleSync(eff.EfectType, v3EffectPosition, pObject->Angle, v3EffectColor, eff.EffectSubType, eff.EffectSize, NULL);
			}
		}
	}

	return true;
}

ITEMEFFECT::~ITEMEFFECT()
{
}

ITEMEFFECT& ITEMEFFECT::Instance()
{
	static ITEMEFFECT s_instance;
	return s_instance;
}

bool ITEMEFFECT::RenderPetEffects(int _index, BMD* pModel, OBJECT* pObject)
{
	int ItemModelType = (_index)-MODEL_ITEM;

	if (ItemModelType >= 0 && ItemModelType < MAX_ITEM)
	{
		auto it = EffectByIndex.find(ItemModelType);

		if (it != EffectByIndex.end())
		{
			for (auto& eff : it->second.effects)
			{
				vec3_t vAngle, v3EffectPosition, v3EffectColor;
				vec3_t position = { 0.f, 0.f, 0.f };

				eff.GetColor(v3EffectColor);

				Vector(0.0f, 0.0f, eff.EffectAngle, vAngle);

				pModel->TransformPosition(BoneTransform[eff.BoneIndex], position, v3EffectPosition);

				if (eff.RenderType != RENDER_PARTICLE)
					CreateSprite(eff.EfectType, v3EffectPosition, eff.EffectSize, v3EffectColor, pObject, vAngle[2], eff.EffectSubType);
				else
					CreateParticleSync(eff.EfectType, v3EffectPosition, vAngle, v3EffectColor, eff.EffectSubType, eff.EffectSize, pObject);
			}
		}
	}

	return true;
}

bool ITEMEFFECT::RenderEffectVisual(int _index, BMD* pModel, OBJECT* pObject)
{
	int ItemModelType = g_CMonkSystem.EqualItemModelType(_index) - MODEL_ITEM;

	if (ItemModelType >= 0 && ItemModelType < MAX_ITEM)
	{
		auto it = EffectByIndex.find(ItemModelType);

		if (it != EffectByIndex.end())
		{
			for (auto& eff : it->second.effects)
			{
				vec3_t vAngle, v3EffectPosition, v3EffectColor;

				eff.GetColor(v3EffectColor);

				Vector(0.0f, 0.0f, eff.EffectAngle, vAngle);

				pModel->TransformByObjectBone(v3EffectPosition, pObject, eff.BoneIndex, NULL);

				if (eff.RenderType != RENDER_PARTICLE)
					CreateSprite(eff.EfectType, v3EffectPosition, eff.EffectSize, v3EffectColor, pObject, vAngle[2], eff.EffectSubType);
				else
					CreateParticleSync(eff.EfectType, v3EffectPosition, vAngle, v3EffectColor, eff.EffectSubType, eff.EffectSize, pObject);
			}
		}
	}

	return true;
}
