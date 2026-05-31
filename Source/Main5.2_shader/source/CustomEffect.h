#pragma once
#include "CScriptManager.h"

#define RENDER_SPRITE					0
#define RENDER_PARTICLE					1

class SEFFECT
{
public:
	int32_t KeyIndex;
	int32_t EfectType;
	int32_t EffectSubType;
	int16_t BoneIndex;
	int16_t RenderType;
	//-- none
	uint32_t EffectColor;
	float EffectSize;
	float EffectAngle;
public:
	bool operator == (int32_t idx) const
	{
		return this->KeyIndex == idx;
	};

	void GetColor(float* out) const
	{
		out[0] = (EffectColor & 0xff) / 255.f;
		out[1] = ((EffectColor >> 8) & 0xff) / 255.f;
		out[2] = ((EffectColor >> 16) & 0xff) / 255.f;
	}
};

typedef struct
{
	std::vector<SEFFECT> effects;
}EffectList;

class CustomEffect
{
	friend class ITEMEFFECT;
	friend class MONSTEREFFECT;
public:
	void LoadEffects(FILE* fp, int Type);
	void RegisterEffect(SEFFECT info, int Type);
private:
	std::unordered_map<int, EffectList> EffectByIndex;
};


class MONSTEREFFECT : public CustomEffect
{
public:
	MONSTEREFFECT();
	~MONSTEREFFECT();

	MONSTEREFFECT(const MONSTEREFFECT&) = delete;
	MONSTEREFFECT& operator=(const MONSTEREFFECT&) = delete;
	static MONSTEREFFECT& Instance();

public:
	bool RenderEffectVisual(BMD* pModel, CHARACTER* sCharacter, OBJECT* pObject);
};


class ITEMEFFECT : public CustomEffect
{
public:
	ITEMEFFECT() = default;
	~ITEMEFFECT();

	ITEMEFFECT(const ITEMEFFECT&) = delete;
	ITEMEFFECT& operator=(const ITEMEFFECT&) = delete;
	static ITEMEFFECT& Instance();

public:
	bool RenderPetEffects(int _index, BMD* pModel, OBJECT* pObject);
	bool RenderEffectVisual(int _index, BMD* pModel, OBJECT* pObject);
};



#define gMonsterEffect				(&MONSTEREFFECT::Instance())
#define gItemEffect					(&ITEMEFFECT::Instance())

