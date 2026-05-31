#pragma once
#include "Singleton.h"

class CGMCharacter
{
public:
	CGMCharacter();
	~CGMCharacter();
	void Init();
	void Release();
	CHARACTER* GetCharacter(int index);
	CHARACTER* GetDummyCharacter();
	int GetCharacterIndex(CHARACTER* pCha);

	static CGMCharacter* Instance();

private:
	std::vector<CHARACTER> CharacterMemoryDump;
};

extern CHARACTER* CharactersClient;
extern CHARACTER* GetCharacterDataUnsafe(int i);

#define gmCharacters			(CGMCharacter::Instance())