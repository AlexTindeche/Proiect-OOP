#pragma once

#include "portativ.h"

#include "ArpegioDoc.h"

class MIDIPlayer
{
public:
	MIDIPlayer();
	void play(Portativ *p, bool* running, CMutex* mutex, HWND hWnd);
};