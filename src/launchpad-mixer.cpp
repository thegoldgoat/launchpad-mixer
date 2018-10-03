/* launchpad-mixer.c
 *
 * Copyright 2018 thegoldgoat
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <rtmidi/RtMidi.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "mixer.h"

using namespace std;

void setLaunchpadVolume(long volume);

bool stillRunning = true;
RtMidiOut *midiOutput = NULL;
RtMidiIn *midiInput = NULL;

static void finish(int ignore) { stillRunning = false; }

int main(int argc, char *argv[]) {
  // Init mixer class
  Mixer mixer;

  // Init launchpad
  midiInput = new RtMidiIn();
  midiOutput = new RtMidiOut();
  int midiPortCount = midiInput->getPortCount();
  cout << "Choose midi device:\n";
  for (int i = 0; i < midiPortCount; i++)
    cout << i << " -> " << midiInput->getPortName(i) << endl;

  int choosen;
  cin >> choosen;

  if (choosen < 0 || choosen >= midiPortCount) {
    cout << "Wrong port number.\n";
    exit(1);
  }
  midiInput->openPort(choosen);
  midiOutput->openPort(choosen);

  std::vector<unsigned char> message(3);

  // Clear the launchpad
  message[0] = 176;
  message[1] = 0;
  message[2] = 0;

  midiOutput->sendMessage(&message);

  // Signal handler
  signal(SIGINT, finish);

  setLaunchpadVolume(mixer.getMasterVolume());

  while (stillRunning) {
    midiInput->getMessage(&message);

    usleep(10000);

    if (message.size() != 3) continue;

    if (message[0] != 144) continue;
    if (message[2] != 127) continue;
    if ((message[1] - 8) % 16 != 0) continue;
    // Change the volume
    int pressedButton = 7 - (float(message[1]) - 8.0) / 16.0;
    float newVolume = pressedButton * (100.0 / 7.0);
    mixer.setMasterVolume(newVolume);
    setLaunchpadVolume(newVolume);
  }

  return EXIT_SUCCESS;
}

void setLaunchpadVolume(long volume) {
  // Numbers to be shown in the launchpad
  int bottonNumber = volume / (100 / 7) + 1;
  std::vector<unsigned char> message(3);
  for (int i = 0; i < 8; i++) {
    message[0] = 144;
    message[1] = i * 16 + 8;
    message[2] = 8 - i <= bottonNumber ? 3 : 0;

    midiOutput->sendMessage(&message);
  }
}