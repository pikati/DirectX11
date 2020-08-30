#pragma once
#include "xaudio2.h"

enum SoundLabel
{
	BGM_TITLE,
	BGM_MAIN,
	BGM_RESULT,
	SE_DECIDE,
	SE_GET,
	SE_JUMP,
	LABEL_MAX
};

struct SOUNDPARAM
{
	char* pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
};

class AudioManager
{
private:
	
	static IXAudio2* m_xAudio2;
	static IXAudio2MasteringVoice* m_masteringVoice;
	static IXAudio2SourceVoice* m_sourceVoice[LABEL_MAX];
	static BYTE* m_dataAudio[LABEL_MAX];
	static DWORD m_sizeAudio[LABEL_MAX];
	static SOUNDPARAM m_param[LABEL_MAX];

	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

public:
	static void Initialize();
	static void Finalize();
	static void PlaySound(SoundLabel label);
	static void StopSound();
};

