#pragma once
#include "MemScript.h"

struct EFFECT_ENTRY
{
	int32_t KeyIndex;
	int32_t EfectType;
	int32_t EffectSubType;
	int16_t BoneIndex;
	int16_t RenderType;
	uint32_t EffectColor;
	float EffectSize;
	float EffectAngle;

	static void parser(CMemScript* script, EFFECT_ENTRY& info);
	static void parserItem(CMemScript* script, EFFECT_ENTRY& info);
};


template<typename EntryType, typename FuncType>
void ReadScripted(const std::string& filename, std::vector<EntryType>& container, size_t maxSize, FuncType parser)
{
	if (auto script = std::unique_ptr<CMemScript>(new CMemScript))
	{
		if (script->SetBuffer(filename))
		{
			try
			{
				size_t inserted = 0; // contador por archivo

				while (true)
				{
					if (script->GetToken() == TOKEN_END || strcmp("end", script->GetString()) == 0)
						break;

					if (inserted >= maxSize)
						break; // alcanzamos el límite local de este archivo

					EntryType info{};
					parser(script.get(), info);

					container.push_back(info);
					++inserted;
				}
			}
			catch (...)
			{
				printf(script->GetLastError());
			}
		}
		else
		{
			printf(script->GetLastError());
		}
	}
	else
	{
		printf(MEM_SCRIPT_ALLOC_ERROR, filename.c_str());
	}
}

