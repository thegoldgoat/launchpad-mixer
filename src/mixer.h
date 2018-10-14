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

#ifndef _MIXER_CPP
#define _MIXER_CPP

#include <alsa/asoundlib.h>
#include <alsa/mixer.h>

class Mixer {
 public:
  Mixer();
  ~Mixer();

  // Set the master mixer volume
  void setMasterVolume(long volume);
  // Get the master mixer volume
  long getMasterVolume();

 private:
  snd_mixer_t *mixerHandle = NULL;
  snd_mixer_elem_t *mixerElem = NULL;

  long minVolume;
  long maxVolume;
};

#endif  // _MIXER_CPP