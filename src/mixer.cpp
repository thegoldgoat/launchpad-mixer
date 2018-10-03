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