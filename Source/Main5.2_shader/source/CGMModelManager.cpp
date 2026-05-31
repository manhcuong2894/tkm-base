#include "stdafx.h"
#include "ZzzTexture.h"
#include "CGMModelManager.h"
#include "./Utilities/Log/muConsoleDebug.h"

BMD* ClientModels;

CGMModelManager::CGMModelManager()
{
	ClientModels = NULL;
	Init();
}

CGMModelManager::~CGMModelManager()
{
	Release();
}

void CGMModelManager::Init()
{
	modelMemoryDump.resize((MAX_MODELS + 1024));
	ClientModels = (modelMemoryDump.data()) + rand() % 1024;
}

void CGMModelManager::Release()
{
	ClientModels = NULL;
	modelMemoryDump.clear();
}

BMD* CGMModelManager::GetModel(int index)
{
	if (index >= 0 && index < MAX_MODELS)
	{
		return &ClientModels[index];
	}
	return nullptr;
}

BMD* CGMModelManager::GetPlayerModel()
{
	return &ClientModels[MODEL_PLAYER];
}

Action_t* CGMModelManager::GetPlayerModelAction(int i)
{
	return &ClientModels[MODEL_PLAYER].Actions[i];
}

Action_t* CGMModelManager::GetModelAction(int index, int i)
{
	BMD* pModel = GetModel(index);

	if (pModel)
	{
		return &pModel->Actions[i];
	}

	return nullptr;
}

Mesh_t* CGMModelManager::GetModelMesh(int index, int i)
{
	BMD* pModel = GetModel(index);

	if (pModel)
	{
		return pModel->GetMesh(i);
	}

	return nullptr;
}

void CGMModelManager::AccessModel(int Type, char* Path, char* FileName, int i)
{
	BMD* pModel = this->GetModel(Type);

	if (!pModel) return;

	char Name[64];

	if (i == -1)
		sprintf(Name, "%s.bmd", FileName);
	else if (i >= 10)
		sprintf(Name, "%s%d.bmd", FileName, i);
	else
		sprintf(Name, "%s0%d.bmd", FileName, i);

	pModel->m_iBMDSeqID = Type;

	if (!pModel->Open2(Path, Name))
	{
		if ((strcmp(FileName, "Monster") == NULL || strcmp(FileName, "Player") == NULL || strcmp(FileName, "PlayerTest") == NULL || strcmp(FileName, "Angel") == NULL))
		{
			char Text[256];
			sprintf(Text, "[%s][%s] file does not exist.", Path, Name);

			MessageBox(gwinhandle->GethWnd(), Text, NULL, MB_OK);
			SendMessage(gwinhandle->GethWnd(), WM_DESTROY, 0, 0);
		}
	}
	else
	{
#if jdk_shader_local330
		OGL330MODEL::ConvertOldMeshToVaoMesh(Type);
#endif
	}
}

void CGMModelManager::OpenTexture(int _Type, char* SubFolder, int Wrap, int Type, bool Check)
{
	BMD* pModel = this->GetModel(_Type);

	if (!pModel) return;

	for (int i = 0; i < pModel->NumMeshs; i++)
	{
		Texture_t* pTexture = &pModel->Textures[i];
		pModel->IndexTexture[i] = BITMAP_UNKNOWN;
		pModel->LightTexture[i] = 0;

		char szFullPath[256] = { 0 };
		strcpy(szFullPath, "Data\\");
		strcat(szFullPath, SubFolder);
		strcat(szFullPath, pTexture->FileName);

		const char* name = pTexture->FileName;
		if (!memcmp(name, "nude_", 5))          pModel->LightTexture[i] = 1;
		else if (!memcmp(name, "cotton_", 7))   pModel->LightTexture[i] = 2;
		else if (!memcmp(name, "cotton2_", 8))  pModel->LightTexture[i] = 3;
		else if (!memcmp(name, "K_nude", 6))    pModel->LightTexture[i] = 4;

		if (!memcmp(name, "skin_", 5) || !memcmp(name, "level", 5))
			pModel->IndexTexture[i] = BITMAP_SKIN;
		else if (!memcmp(name, "hid", 3))
			pModel->IndexTexture[i] = BITMAP_HIDE;
		else if (!memcmp(name, "hair", 4))
			pModel->IndexTexture[i] = BITMAP_HAIR;
		else if (!memcmp(name, "HQskin3", 7))
			pModel->IndexTexture[i] = BITMAP_HQSKIN3;
		else if (!memcmp(name, "HQskin2", 7))
			pModel->IndexTexture[i] = BITMAP_HQSKIN2;
		else if (!memcmp(name, "HQskin", 6))
			pModel->IndexTexture[i] = BITMAP_HQSKIN;
		else if (!memcmp(name, "HQlevel_", 8))
			pModel->IndexTexture[i] = BITMAP_HQSKIN;
		else if (!memcmp(name, "HQhair_", 7) || !memcmp(name, "HQhair", 6))
			pModel->IndexTexture[i] = BITMAP_HQHAIR;

		if (pModel->IndexTexture[i] == BITMAP_UNKNOWN)
		{
			char __ext[_MAX_EXT] = { 0 };
			_splitpath(name, NULL, NULL, NULL, __ext);

			char ext = tolower(__ext[1]);
			if (ext == 't')      // TGA
				pModel->IndexTexture[i] = Bitmaps.LoadImage(szFullPath, GL_NEAREST, Wrap);
			else if (ext == 'j') // JPG
				pModel->IndexTexture[i] = Bitmaps.LoadImage(szFullPath, Type, Wrap);
		}

		// --- Fallback: intentar buscar una textura ya cargada ---
		if (pModel->IndexTexture[i] == BITMAP_UNKNOWN)
		{
			BITMAP_t* pBitmap = Bitmaps.FindTextureByName(name);

			if (pBitmap)
			{
				Bitmaps.LoadImage(pBitmap->BitmapIndex, pBitmap->FileName);
				pModel->IndexTexture[i] = pBitmap->BitmapIndex;
			}
			else
			{
				char szErrorMsg[256] = { 0 };
				sprintf(szErrorMsg, "OpenTextureFast Failed: %s of %s   %s", szFullPath, pModel->Name, name);
				PopUpErrorCheckMsgBox(szErrorMsg, Check);
				// break;  // en este caso no corto el loop, para intentar seguir con las demas
			}
		}
	}
}

void CGMModelManager::OpenPack(int Type, char* Path, char* FileName, int i)
{
	char Name[64];
	if (i == -1)
		sprintf(Name, "%s.pack", FileName);
	else if (i >= 10)
		sprintf(Name, "%s%d.pack", FileName, i);
	else
		sprintf(Name, "%s0%d.pack", FileName, i);

	bool Success = false;

	BMD* pModel = this->GetModel(Type);

	if (pModel)
	{
		//pModel->m_iBMDSeqID = Type;
		//Success = pModel->OpenPack(Path, Name);
		//
		//if (Success == false)
		//{
		//	if ((strcmp(FileName, "Monster") == NULL || strcmp(FileName, "Player") == NULL || strcmp(FileName, "PlayerTest") == NULL || strcmp(FileName, "Angel") == NULL))
		//	{
		//		char Text[256];
		//		sprintf(Text, "%s file does not exist.", Name);
		//		MessageBox(gwinhandle->GethWnd(), Text, NULL, MB_OK);
		//		SendMessage(gwinhandle->GethWnd(), WM_DESTROY, 0, 0);
		//	}
		//}
	}
}

void CGMModelManager::Save(int Type)
{
	char urlFileName[128];
	CreateDirectory("Data2", 0);
	CreateDirectory("Data2\\Player", 0);

	sprintf_s(urlFileName, "Data2\\Player\\");

	BMD* pModel = this->GetModel(Type);

	if (pModel && pModel->NumMeshs > 0)
	{
		pModel->Save2(urlFileName, pModel->FileName);

		for (int i = 0; i < pModel->NumMeshs; i++)
		{
			Texture_t* pTexture = &pModel->Textures[i];

			char FileName1[128]; sprintf_s(FileName1, "Data\\Player\\%s", pTexture->FileName);
			char FileName2[128]; sprintf_s(FileName2, "Data2\\Player\\%s", pTexture->FileName);

			copy_file_run(FileName1, FileName2);
		}
	}
}

void CGMModelManager::SaveImg(int Type)
{
	CreateDirectory("Data2", 0);
	CreateDirectory("Data2\\Player", 0);

	BITMAP_t* pBitmap = &Bitmaps[Type];

	if (pBitmap->FileName[0] != '\0')
	{
		char FileName[128];
		sprintf_s(FileName, "Data2\\Player\\%s", GetFileNameWithExtension(pBitmap->FileName).c_str());

		copy_file_run(pBitmap->FileName, FileName);
	}
}

void CGMModelManager::copy_file_run(std::string FileOrigin, std::string FileLocation)
{
	changeExtension(FileOrigin);
	changeExtension(FileLocation);

	FILE* fp = fopen(FileOrigin.c_str(), "rb");

	if (fp)
	{
		fseek(fp, 0, SEEK_END);
		int fileLen = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		BYTE* Buffer = new BYTE[fileLen];

		fread(Buffer, fileLen, 1, fp);

		fclose(fp);

		fp = fopen(FileLocation.c_str(), "wb");

		if (fp)
		{
			fwrite(Buffer, fileLen, 1u, fp);
			fclose(fp);
		}
		delete[]Buffer;
	}
}

CGMModelManager* CGMModelManager::Instance()
{
	static CGMModelManager s_Instance;
	return &s_Instance;
}

BMD* GetModelDataUnsafe(int i)
{
	return &ClientModels[i];
}
