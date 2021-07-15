#pragma once
#include "stdafx.h"

class AI {
public:
	AI() {
		memset(pKeysBuffer, 0, sizeof(UCHAR) * 256);
		SetNextState();
	}
	UCHAR* GetInput() {
		return pKeysBuffer;
	}
	void Update(float fTime) {
		fCurTime += fTime;
		if(fCurTime >= fLastPeriod) SetNextState();
	}

private:
	void SetNextState() {
		switch (rand() % 9)
		{
		case 0: pKeysBuffer[KeyCode::_W] = 0xF0; pKeysBuffer[KeyCode::_A] = 0x00; pKeysBuffer[KeyCode::_S] = 0x00; pKeysBuffer[KeyCode::_D] = 0x00; break;
		case 1: pKeysBuffer[KeyCode::_W] = 0x00; pKeysBuffer[KeyCode::_A] = 0xF0; pKeysBuffer[KeyCode::_S] = 0x00; pKeysBuffer[KeyCode::_D] = 0x00; break;
		case 2: pKeysBuffer[KeyCode::_W] = 0x00; pKeysBuffer[KeyCode::_A] = 0x00; pKeysBuffer[KeyCode::_S] = 0xF0; pKeysBuffer[KeyCode::_D] = 0x00; break;
		case 3: pKeysBuffer[KeyCode::_W] = 0x00; pKeysBuffer[KeyCode::_A] = 0x00; pKeysBuffer[KeyCode::_S] = 0x00; pKeysBuffer[KeyCode::_D] = 0xF0; break;
		case 4: pKeysBuffer[KeyCode::_W] = 0xF0; pKeysBuffer[KeyCode::_A] = 0xF0; pKeysBuffer[KeyCode::_S] = 0x00; pKeysBuffer[KeyCode::_D] = 0x00; break;
		case 5: pKeysBuffer[KeyCode::_W] = 0x00; pKeysBuffer[KeyCode::_A] = 0xF0; pKeysBuffer[KeyCode::_S] = 0xF0; pKeysBuffer[KeyCode::_D] = 0x00; break;
		case 6: pKeysBuffer[KeyCode::_W] = 0x00; pKeysBuffer[KeyCode::_A] = 0x00; pKeysBuffer[KeyCode::_S] = 0xF0; pKeysBuffer[KeyCode::_D] = 0xF0; break;
		case 7: pKeysBuffer[KeyCode::_W] = 0xF0; pKeysBuffer[KeyCode::_A] = 0x00; pKeysBuffer[KeyCode::_S] = 0x00; pKeysBuffer[KeyCode::_D] = 0xF0; break;
		case 8: pKeysBuffer[KeyCode::_W] = 0x00; pKeysBuffer[KeyCode::_A] = 0x00; pKeysBuffer[KeyCode::_S] = 0x00; pKeysBuffer[KeyCode::_D] = 0x00; break;
		default:break;
		}
		fCurTime = 0;
		fLastPeriod = (rand() % 3000) * 0.001f;
	}

private:
	UCHAR pKeysBuffer[256];
	float fLastPeriod;
	float fCurTime;
};