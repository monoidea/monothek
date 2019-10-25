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

#ifndef __MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_H__
#define __MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#define MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL                (monothek_diskjokey_sequencer_model_get_type())
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL, MonothekDiskjokeySequencerModel))
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL, MonothekDiskjokeySequencerModelClass))
#define MONOTHEK_IS_DISKJOKEY_SEQUENCER_MODEL(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL))
#define MONOTHEK_IS_DISKJOKEY_SEQUENCER_MODEL_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL))
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_MODEL, MonothekDiskjokeySequencerModelClass))

#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT (9)
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT (64)

#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT (4)

#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_LOWER (80.0)
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_UPPER (160.0)
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_DEFAULT (120.0)

#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_SWING_LOWER (0.0)
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_SWING_UPPER (5.0)
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_SWING_DEFAULT (0.0)

#if MONOTHEK_DEVEL_MODE
#if defined(MONOTHEK_W32API)
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME "share\\monothek\\drumkit\\techno.xml"
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE_FILENAME "share\\monothek\\drumkit\\house.xml"
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP_FILENAME "share\\monothek\\drumkit\\hip-hop.xml"
#else
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME SRCDIR "/monothek.share/monothek/drumkit/techno.xml"
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE_FILENAME SRCDIR "/monothek.share/monothek/drumkit/house.xml"
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP_FILENAME SRCDIR "/monothek.share/monothek/drumkit/hip-hop.xml"
#endif
#else
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME "/usr/share/monothek/drumkit/techno.xml"
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE_FILENAME "/usr/share/monothek/drumkit/house.xml"
#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP_FILENAME "/usr/share/monothek/drumkit/hip-hop.xml"
#endif

#define MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_DEFAULT_DURATION_SEC (600)

typedef struct _MonothekDiskjokeySequencerModel MonothekDiskjokeySequencerModel;
typedef struct _MonothekDiskjokeySequencerModelClass MonothekDiskjokeySequencerModelClass;

typedef enum{
  MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO,
  MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE,
  MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP,
}MonothekDiskjokeySequencerModelGenre;

struct _MonothekDiskjokeySequencerModel
{
  GObject gobject;

  gboolean **pad_active;

  guint current_genre;
  gboolean techno_active;
  gboolean house_active;
  gboolean hiphop_active;

  gboolean random_active;
  gboolean clear_active;
  
  gboolean run_active;

  gint active_column;

  guint current_tab;
  gboolean *tab_active;

  gdouble bpm_upper;
  gdouble bpm_lower;
  gdouble bpm;

  gdouble swing_upper;
  gdouble swing_lower;
  gdouble swing;
  
  gchar **techno_label;
  gchar **techno_sample;

  gchar **techno_bank_name;
  gchar *default_techno_bank_name;
  
  GHashTable **techno_control;
  GHashTable **techno_bank;

  gchar **house_label;
  gchar **house_sample;

  gchar **house_bank_name;
  gchar *default_house_bank_name;

  GHashTable **house_control;
  GHashTable **house_bank;

  gchar **hiphop_label;
  gchar **hiphop_sample;

  gchar **hiphop_bank_name;
  gchar *default_hiphop_bank_name;

  GHashTable **hiphop_control;
  GHashTable **hiphop_bank;
  
  struct timespec *duration;
};

struct _MonothekDiskjokeySequencerModelClass
{
  GObjectClass gobject;
};

GType monothek_diskjokey_sequencer_model_get_type(void);

void monothek_diskjokey_sequencer_model_set_pad_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
						       guint x, guint y,
						       gboolean is_active);
gboolean monothek_diskjokey_sequencer_model_get_pad_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
							   guint x, guint y);

void monothek_diskjokey_sequencer_model_set_tab_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
						       guint x,
						       gboolean is_active);
gboolean monothek_diskjokey_sequencer_model_get_tab_active(MonothekDiskjokeySequencerModel *diskjokey_sequencer_model,
							   guint x);

MonothekDiskjokeySequencerModel* monothek_diskjokey_sequencer_model_new();

#endif /*__MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_H__*/
