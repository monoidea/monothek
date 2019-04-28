/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2019 Joël Krähemann
 *
 * This file is part of Monothek.
 *
 * Monothek is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Monothek is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Monothek.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MONOTHEK_DELAY_AUDIO_H__
#define __MONOTHEK_DELAY_AUDIO_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define MONOTHEK_TYPE_DELAY_AUDIO                (monothek_delay_audio_get_type())
#define MONOTHEK_DELAY_AUDIO(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DELAY_AUDIO, MonothekDelayAudio))
#define MONOTHEK_DELAY_AUDIO_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_DELAY_AUDIO, MonothekDelayAudioClass))
#define MONOTHEK_IS_DELAY_AUDIO(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_DELAY_AUDIO))
#define MONOTHEK_IS_DELAY_AUDIO_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_DELAY_AUDIO))
#define MONOTHEK_DELAY_AUDIO_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_DELAY_AUDIO, MonothekDelayAudioClass))

typedef struct _MonothekDelayAudio MonothekDelayAudio;
typedef struct _MonothekDelayAudioClass MonothekDelayAudioClass;

struct _MonothekDelayAudio
{
  AgsDelayAudio delay_audio;

  AgsPort *sequencer_paused;
};

struct _MonothekDelayAudioClass
{
  AgsDelayAudioClass delay_audio;
};

GType monothek_delay_audio_get_type();

MonothekDelayAudio* monothek_delay_audio_new();

#endif /*__MONOTHEK_DELAY_AUDIO_H__*/
