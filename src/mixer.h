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