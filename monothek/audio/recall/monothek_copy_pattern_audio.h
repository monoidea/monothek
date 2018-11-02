/* Monothek - monoidea's monothek
 * Copyright (C) 2018 Joël Krähemann
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

#ifndef __MONOTHEK_COPY_PATTERN_AUDIO_H__
#define __MONOTHEK_COPY_PATTERN_AUDIO_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define MONOTHEK_TYPE_COPY_PATTERN_AUDIO                (monothek_copy_pattern_audio_get_type())
#define MONOTHEK_COPY_PATTERN_AUDIO(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_COPY_PATTERN_AUDIO, MonothekCopyPatternAudio))
#define MONOTHEK_COPY_PATTERN_AUDIO_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_COPY_PATTERN_AUDIO, MonothekCopyPatternAudioClass))
#define MONOTHEK_IS_COPY_PATTERN_AUDIO(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_COPY_PATTERN_AUDIO))
#define MONOTHEK_IS_COPY_PATTERN_AUDIO_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_COPY_PATTERN_AUDIO))
#define MONOTHEK_COPY_PATTERN_AUDIO_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_COPY_PATTERN_AUDIO, MonothekCopyPatternAudioClass))

typedef struct _MonothekCopyPatternAudio MonothekCopyPatternAudio;
typedef struct _MonothekCopyPatternAudioClass MonothekCopyPatternAudioClass;

struct _MonothekCopyPatternAudio
{
  AgsCopyPatternAudio copy_pattern_audio;
};

struct _MonothekCopyPatternAudioClass
{
  AgsCopyPatternAudioClass copy_pattern_audio;
};

GType monothek_copy_pattern_audio_get_type();

MonothekCopyPatternAudio* monothek_copy_pattern_audio_new();

#endif /*__MONOTHEK_COPY_PATTERN_AUDIO_H__*/
