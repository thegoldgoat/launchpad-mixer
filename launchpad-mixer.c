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

#include <rtmidi/RtMidi.h>
#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void SetAlsaMasterVolume(long volume);
void launchpadMidiCallback(double, std::vector<unsigned char> *, void *);

int main(int argc, char *argv[])
{

  // Init launchpad
  RtMidiIn *midiInput = new RtMidiIn();
  int midiPortCount = midiInput->getPortCount();
  cout << "Choose midi device:\n";
  for (int i = 0; i < midiPortCount; i++)
    cout << i << " -> " << midiInput->getPortName(i) << endl;

  int choosen;
  cin >> choosen;

  if (choosen < 0 || choosen >= midiPortCount)
  {
    cout << "Wrong port number.\n";
    exit(1);
  }
  midiInput->openPort(choosen);
  midiInput->setCallback(launchpadMidiCallback);

  midiInput->getMessage()

  sleep(10);
  SetAlsaMasterVolume(100);

  return EXIT_SUCCESS;
}

// Set the master volume, the parametre must be between 0 and 100
void SetAlsaMasterVolume(long volume)
{
  long min, max;
  snd_mixer_t *handle;
  snd_mixer_selem_id_t *sid;
  const char *card = "default";
  const char *selem_name = "Master";

  snd_mixer_open(&handle, 0);
  snd_mixer_attach(handle, card);
  snd_mixer_selem_register(handle, NULL, NULL);
  snd_mixer_load(handle);

  snd_mixer_selem_id_alloca(&sid);
  snd_mixer_selem_id_set_index(sid, 0);
  snd_mixer_selem_id_set_name(sid, selem_name);
  snd_mixer_elem_t *elem = snd_mixer_find_selem(handle, sid);

  snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
  snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

  snd_mixer_close(handle);
}

void launchpadMidiCallback(double delta, std::vector<unsigned char> *_message, void *data)
{
  std::vector<unsigned char> message = *_message;

  if (message[0] != 144)
    return;
  if (message[2] != 127)
    return;
  if ((message[1] - 8) % 16 != 0)
    return;
  // Change the volume
  float newVolume = (float(message[1]) - 8.0) / 16.0 * (100.0 / 7.0);
  SetAlsaMasterVolume(newVolume);
}
