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

#ifndef __MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER_H__
#define __MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_controller.h>

#define MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER                (monothek_diskjokey_sequencer_controller_get_type())
#define MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER, MonothekDiskjokeySequencerController))
#define MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER, MonothekDiskjokeySequencerControllerClass))
#define MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER))
#define MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER))
#define MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER, MonothekDiskjokeySequencerControllerClass))

typedef struct _MonothekDiskjokeySequencerController MonothekDiskjokeySequencerController;
typedef struct _MonothekDiskjokeySequencerControllerClass MonothekDiskjokeySequencerControllerClass;

struct _MonothekDiskjokeySequencerController
{
  MonothekController controller;

  MonothekActionBox ***pad;

  MonothekActionBox *techno;
  MonothekActionBox *house;
  MonothekActionBox *hiphop;

  MonothekActionBox *clear;
  MonothekActionBox *random;

  MonothekActionBox *run;

  MonothekActionBox **tab;

  MonothekActionSlider *bpm;
  MonothekActionSlider *swing;

  struct timespec *start_time;
  struct timespec *timer;
};

struct _MonothekDiskjokeySequencerControllerClass
{
  MonothekControllerClass controller;

  void (*toggle_pad)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
		     guint x, guint y);

  void (*switch_page)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
		      guint page_index);
  void (*select_bank)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
		      guint bank_0, guint bank_1);

  void (*change_bpm)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
		     gdouble bpm);
  void (*change_swing)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
		       gdouble swing);
  void (*change_duration)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
			  guint step_count);

  void (*load_drum_kit)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
			gchar *drum_kit);

  void (*clear)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
  void (*random)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

  void (*progress)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
		   gdouble value);

  void (*run)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
	      gboolean do_run);

  void (*completed)(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
};

GType monothek_diskjokey_sequencer_controller_get_type(void);

void monothek_diskjokey_sequencer_controller_toggle_pad(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							guint x, guint y);

void monothek_diskjokey_sequencer_controller_switch_page(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							 guint page_index);
void monothek_diskjokey_sequencer_controller_select_bank(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							 guint bank_0, guint bank_1);

void monothek_diskjokey_sequencer_controller_change_bpm(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							gdouble bpm);
void monothek_diskjokey_sequencer_controller_change_swing(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							  gdouble swing);
void monothek_diskjokey_sequencer_controller_change_duration(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							     guint step_count);
  
void monothek_diskjokey_sequencer_controller_load_drum_kit(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							   gchar *drum_kit);

void monothek_diskjokey_sequencer_controller_clear(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_random(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_progress(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						      gdouble value);

void monothek_diskjokey_sequencer_controller_run(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						 gboolean do_run);

void monothek_diskjokey_sequencer_controller_completed(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

gboolean monothek_diskjokey_sequencer_controller_position_timeout(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
gboolean monothek_diskjokey_sequencer_controller_progress_increase_timeout(GObject *gobject);

MonothekDiskjokeySequencerController* monothek_diskjokey_sequencer_controller_new();

#endif /*__MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER_H__*/
