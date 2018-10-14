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

#include "mixer.h"

Mixer::Mixer() {
  snd_mixer_selem_id_t *sid;

  const char *card = "default";
  const char *selem_name = "Master";

  snd_mixer_open(&this->mixerHandle, 0);
  snd_mixer_attach(this->mixerHandle, card);
  snd_mixer_selem_register(this->mixerHandle, NULL, NULL);
  snd_mixer_load(this->mixerHandle);

  snd_mixer_selem_id_alloca(&sid);
  snd_mixer_selem_id_set_index(sid, 0);
  snd_mixer_selem_id_set_name(sid, selem_name);
  mixerElem = snd_mixer_find_selem(this->mixerHandle, sid);

  snd_mixer_selem_get_playback_volume_range(mixerElem, &this->minVolume,
                                            &this->maxVolume);
}

Mixer::~Mixer() { snd_mixer_close(this->mixerHandle); }

void Mixer::setMasterVolume(long volume) {
  snd_mixer_selem_set_playback_volume_all(mixerElem,
                                          volume * this->maxVolume / 100);
}

long Mixer::getMasterVolume() {
  long returnValue;
  snd_mixer_selem_get_playback_volume(mixerElem, SND_MIXER_SCHN_MONO,
                                      &returnValue);
  return returnValue * 100 / this->maxVolume;
}