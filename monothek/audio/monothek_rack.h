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

#ifndef __MONOTHEK_RACK_H__
#define __MONOTHEK_RACK_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define MONOTHEK_TYPE_RACK                (monothek_rack_get_type())
#define MONOTHEK_RACK(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_RACK, MonothekRack))
#define MONOTHEK_RACK_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_RACK, MonothekRackClass))
#define MONOTHEK_IS_RACK(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_RACK))
#define MONOTHEK_IS_RACK_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_RACK))
#define MONOTHEK_RACK_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_RACK, MonothekRackClass))

#define MONOTHEK_RACK_DEFAULT_AUDIO_CHANNELS (2)

#define MONOTHEK_RACK_DEFAULT_SEQUENCER_INPUT_PADS (9)

typedef struct _MonothekRack MonothekRack;
typedef struct _MonothekRackClass MonothekRackClass;

typedef enum{
  MONOTHEK_RACK_ADDED_TO_REGISTRY           = 1,
  MONOTHEK_RACK_CONNECTED                   = 1 <<  1,
}MonothekRackFlags;

struct _MonothekRack
{
  GObject gobject;

  guint flags;

  pthread_mutex_t *obj_mutex;
  pthread_mutexattr_t *obj_mutexattr;

  AgsUUID *uuid;

  guint audio_channels;

  GObject *output_soundcard;
  
  AgsAudio *panel;
  AgsAudio *mixer;
  AgsAudio *player;
  AgsAudio *sequencer;
};

struct _MonothekRackClass
{
  GObjectClass gobject;
};

GType monothek_rack_get_type();

pthread_mutex_t* monothek_rack_get_class_mutex();

gboolean monothek_rack_test_flags(MonothekRack *rack, guint flags);
void monothek_rack_set_flags(MonothekRack *rack, guint flags);
void monothek_rack_unset_flags(MonothekRack *rack, guint flags);

AgsAudio* monothek_rack_create_panel(MonothekRack *rack);
AgsAudio* monothek_rack_create_mixer(MonothekRack *rack);
AgsAudio* monothek_rack_create_player(MonothekRack *rack);
AgsAudio* monothek_rack_create_sequencer(MonothekRack *rack);

void monothek_rack_setup_tree(MonothekRack *rack);

MonothekRack* monothek_rack_new(GObject *output_soundcard);

#endif /*__MONOTHEK_RACK_H__*/
