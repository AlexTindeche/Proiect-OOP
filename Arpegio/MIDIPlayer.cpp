#include "pch.h"
#include "MIDIPlayer.h"

#include <mmsystem.h>  /* multimedia functions (such as MIDI) for Windows */
#pragma comment(lib, "winmm.lib")

#include "MainFrm.h"

MIDIPlayer::MIDIPlayer()
{

}

void MIDIPlayer::play(Portativ* p, bool* running, CMutex* mutex, HWND hWnd)
{
	// setup data
	int velocity = 100; // MIDI note velocity parameter value
	int midiport = 0;       // select which MIDI output port to open (default 0)
	int flag;           // monitor the status of returning functions
	HMIDIOUT device;    // MIDI device interface for sending MIDI output

	// MIDI message
	union { unsigned long word; unsigned char data[4]; } message;
	message.data[0] = 0x90;  // command byte of the MIDI message, for example: 0x90: note on
	message.data[1] = 60;    // first data byte of the MIDI message, MIDI note-on message: Key number (60 = middle C)
	message.data[2] = 100;   // second data byte of the MIDI message, MIDI note-on message: Key velocity (100 = loud)
	message.data[3] = 0;     // Unused parameter

	// Open the MIDI output port
	flag = midiOutOpen(&device, midiport, 0, 0, CALLBACK_NULL);
	if (flag != MMSYSERR_NOERROR) {
		OutputDebugString(L"Error opening MIDI Output.\n");
		return;
	}

	// Setup timing
	int bpm = 120;
	int time = 60000 / bpm;
	int whtime = time * 4;

	// Process notes
	int size = p->get_nr_elemente();
	bool keep_running = true;
	for (int i = 0; i < size && keep_running; i++)
	{
		PostMessage(hWnd, WM_PLAYBACK_UPDATE, i, NULL);

		Element* e = p->get_element(i);

		if (e->get_tip_element() == TipElement::NOTA)
		{
			Nota* n = (Nota*)e;
			Inaltime i = n->get_inaltime();
			int height = (n->get_octava() + 1) * 12 + 2 * (int) i - (i >= Inaltime::FA);

			// play current note
			message.data[1] = height;
			message.data[2] = velocity;

			flag = midiOutShortMsg(device, message.word);
			if (flag != MMSYSERR_NOERROR) {
				OutputDebugString(L"Warning: MIDI Output is not open.\n");
			}
		}

		// wait for note duration
		Sleep(whtime * e->get_durata().get_numarator() / e->get_durata().get_numitor());

		// 0 velocity = note off
		message.data[2] = 0;

		flag = midiOutShortMsg(device, message.word);
		if (flag != MMSYSERR_NOERROR) {
			OutputDebugString(L"Warning: MIDI Output is not open.\n");
		}

		// check if it's running
		mutex->Lock();
		keep_running = *running;
		mutex->Unlock();
	}

	// turn any MIDI notes currently playing:
	midiOutReset(device);

	// Remove any data in MIDI device and close the MIDI Output port
	midiOutClose(device);
}