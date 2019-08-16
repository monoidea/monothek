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

#include <monothek/ui/controller/monothek_diskjokey_sequencer_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/audio/monothek_rack.h>

#include <monothek/audio/task/monothek_export_output.h>

#include <monothek/audio/thread/monothek_export_thread.h>

#include <monothek/ui/monothek_application_context.h>
#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_sequencer_model.h>

#include <monothek/ui/view/monothek_diskjokey_sequencer_view.h>
#include <monothek/ui/view/monothek_diskjokey_end_view.h>

#ifdef __APPLE__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <monothek/i18n.h>

void monothek_diskjokey_sequencer_controller_class_init(MonothekDiskjokeySequencerControllerClass *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_sequencer_controller_init(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_finalize(GObject *gobject);

void monothek_diskjokey_sequencer_controller_connect(AgsConnectable *connectable);
void monothek_diskjokey_sequencer_controller_disconnect(AgsConnectable *connectable);

void monothek_diskjokey_sequencer_controller_reset(MonothekController *controller);

void monothek_diskjokey_sequencer_controller_pad_enter_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_pad_leave_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_pad_clicked_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_techno_enter_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_techno_leave_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_techno_clicked_callback(MonothekActionBox *action_box,
								     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_house_enter_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_house_leave_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_house_clicked_callback(MonothekActionBox *action_box,
								    MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_hiphop_enter_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_hiphop_leave_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_hiphop_clicked_callback(MonothekActionBox *action_box,
								     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_random_enter_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_random_leave_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_random_clicked_callback(MonothekActionBox *action_box,
								     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_clear_enter_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_clear_leave_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_clear_clicked_callback(MonothekActionBox *action_box,
								    MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_tab_enter_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_tab_leave_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_tab_clicked_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_bpm_change_value_callback(MonothekActionBox *action_box,
								       gdouble new_value,
								       MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_bpm_move_slider_callback(MonothekActionBox *action_box,
								      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_swing_change_value_callback(MonothekActionBox *action_box,
									 gdouble new_value,
									 MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_swing_move_slider_callback(MonothekActionBox *action_box,
									MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_run_enter_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_run_leave_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_run_clicked_callback(MonothekActionBox *action_box,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_real_toggle_pad(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							     guint x, guint y);

void monothek_diskjokey_sequencer_controller_real_switch_page(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							      guint page_index);
void monothek_diskjokey_sequencer_controller_real_select_bank(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							      guint bank_0, guint bank_1);

void monothek_diskjokey_sequencer_controller_real_change_bpm(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							     gdouble bpm);
void monothek_diskjokey_sequencer_controller_real_change_swing(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							       gdouble swing);
void monothek_diskjokey_sequencer_controller_real_change_duration(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
								  guint step_count);
  
void monothek_diskjokey_sequencer_controller_real_load_drum_kit(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
								gchar *drum_kit);

void monothek_diskjokey_sequencer_controller_real_clear(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_real_random(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

void monothek_diskjokey_sequencer_controller_real_progress(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							   gdouble value);

void monothek_diskjokey_sequencer_controller_real_run(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						      gboolean do_run);

void monothek_diskjokey_sequencer_controller_real_completed(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);

/**
 * SECTION:monothek_diskjokey_sequencer_controller
 * @short_description: The diskjokey_sequencer controller object.
 * @title: MonothekDiskjokeySequencerController
 * @section_id:
 * @include: monothek/ui/controller/monothek_diskjokey_sequencer_controller.h
 *
 * #MonothekDiskjokeySequencerController is the MVC's diskjokey_sequencer controller.
 */

enum{
  TOGGLE_PAD,
  SWITCH_PAGE,
  SELECT_BANK,
  CHANGE_BPM,
  CHANGE_SWING,
  CHANGE_DURATION,
  LOAD_DRUM_KIT,
  CLEAR,
  RANDOM,
  PROGRESS,
  RUN,
  COMPLETED,
  LAST_SIGNAL,
};

static gpointer monothek_diskjokey_sequencer_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_diskjokey_sequencer_controller_parent_connectable_interface;

static guint diskjokey_sequencer_controller_signals[LAST_SIGNAL];

GHashTable *monothek_diskjokey_sequencer_controller_position = NULL;
GHashTable *monothek_diskjokey_sequencer_controller_progress_increase = NULL;

GType
monothek_diskjokey_sequencer_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_sequencer_controller = 0;

    static const GTypeInfo monothek_diskjokey_sequencer_controller_info = {
      sizeof (MonothekDiskjokeySequencerControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_sequencer_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeySequencerController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_sequencer_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_diskjokey_sequencer_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_diskjokey_sequencer_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
									  "MonothekDiskjokeySequencerController", &monothek_diskjokey_sequencer_controller_info,
									  0);

    g_type_add_interface_static(monothek_type_diskjokey_sequencer_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_sequencer_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_sequencer_controller_class_init(MonothekDiskjokeySequencerControllerClass *diskjokey_sequencer_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_diskjokey_sequencer_controller_parent_class = g_type_class_peek_parent(diskjokey_sequencer_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_sequencer_controller;

  gobject->finalize = monothek_diskjokey_sequencer_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) diskjokey_sequencer_controller;
  
  controller->reset = monothek_diskjokey_sequencer_controller_reset;

  /* MonothekDiskjokeySequencerController */
  diskjokey_sequencer_controller->toggle_pad = monothek_diskjokey_sequencer_controller_real_toggle_pad;

  diskjokey_sequencer_controller->switch_page = monothek_diskjokey_sequencer_controller_real_switch_page;
  diskjokey_sequencer_controller->select_bank = monothek_diskjokey_sequencer_controller_real_select_bank;

  diskjokey_sequencer_controller->change_bpm = monothek_diskjokey_sequencer_controller_real_change_bpm;
  diskjokey_sequencer_controller->change_swing = monothek_diskjokey_sequencer_controller_real_change_swing;
  diskjokey_sequencer_controller->change_duration = monothek_diskjokey_sequencer_controller_real_change_duration;

  diskjokey_sequencer_controller->load_drum_kit = monothek_diskjokey_sequencer_controller_real_load_drum_kit;
  
  diskjokey_sequencer_controller->clear = monothek_diskjokey_sequencer_controller_real_clear;
  diskjokey_sequencer_controller->random = monothek_diskjokey_sequencer_controller_real_random;

  diskjokey_sequencer_controller->progress = monothek_diskjokey_sequencer_controller_real_progress;
  diskjokey_sequencer_controller->run = monothek_diskjokey_sequencer_controller_real_run;
  diskjokey_sequencer_controller->completed = monothek_diskjokey_sequencer_controller_real_completed;

  /* signals */
  /**
   * MonothekDiskjokeySequencerController::toggle-pad:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @x: the event's pad x offset
   * @y: the event's pad y offset
   *
   * The ::toggle-pad signal notifies about toggling pad.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[TOGGLE_PAD] =
    g_signal_new("toggle-pad",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, toggle_pad),
		 NULL, NULL,
		 ags_cclosure_marshal_VOID__UINT_UINT,
		 G_TYPE_NONE, 2,
		 G_TYPE_UINT,
		 G_TYPE_UINT);

  /**
   * MonothekDiskjokeySequencerController::switch-page:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @page_index: the page's index
   *
   * The ::switch-page signal notifies about switching page.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[SWITCH_PAGE] =
    g_signal_new("switch-page",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, switch_page),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__UINT,
		 G_TYPE_NONE, 1,
		 G_TYPE_UINT);

  /**
   * MonothekDiskjokeySequencerController::select-bank:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @bank_0: the bank's index 0
   * @bank_1: the bank's index 1
   *
   * The ::select-bank signal notifies about selecting bank.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[SELECT_BANK] =
    g_signal_new("select-bank",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, select_bank),
		 NULL, NULL,
		 ags_cclosure_marshal_VOID__UINT_UINT,
		 G_TYPE_NONE, 2,
		 G_TYPE_UINT,
		 G_TYPE_UINT);

  /**
   * MonothekDiskjokeySequencerController::change-bpm:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @bpm: the bpm
   *
   * The ::change-bpm signal notifies about changed bpm.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[CHANGE_BPM] =
    g_signal_new("change-bpm",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, change_bpm),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__DOUBLE,
		 G_TYPE_NONE, 1,
		 G_TYPE_DOUBLE);

  /**
   * MonothekDiskjokeySequencerController::change-swing:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @swing: the amount to swing ranging from 0.0 to 1.0
   *
   * The ::change-swing signal notifies about changed swing.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[CHANGE_SWING] =
    g_signal_new("change-swing",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, change_swing),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__DOUBLE,
		 G_TYPE_NONE, 1,
		 G_TYPE_DOUBLE);
  
  /**
   * MonothekDiskjokeySequencerController::change-duration:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @step_count: the count of steps
   *
   * The ::change-duration signal notifies about changed duration.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[CHANGE_DURATION] =
    g_signal_new("change-duration",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, change_duration),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__UINT,
		 G_TYPE_NONE, 1,
		 G_TYPE_UINT);

  /**
   * MonothekDiskjokeySequencerController::load-drum-kit:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @drum_kit: the filename of the drum kit
   *
   * The ::load-drum-kit signal notifies about loading drum kit.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[LOAD_DRUM_KIT] =
    g_signal_new("load-drum-kit",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, load_drum_kit),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__STRING,
		 G_TYPE_NONE, 1,
		 G_TYPE_STRING);

  /**
   * MonothekDiskjokeySequencerController::clear:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   *
   * The ::clear signal notifies about cleared pattern.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[CLEAR] =
    g_signal_new("clear",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, clear),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekDiskjokeySequencerController::random:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   *
   * The ::random signal notifies about filled random pattern.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[RANDOM] =
    g_signal_new("random",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, random),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekDiskjokeySequencerController::progress:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @value: a gdouble ranging from 0.0 to 1.0
   *
   * The ::progress signal notifies about progress.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[PROGRESS] =
    g_signal_new("progress",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, progress),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__DOUBLE,
		 G_TYPE_NONE, 1,
		 G_TYPE_DOUBLE);

  /**
   * MonothekDiskjokeySequencerController::run:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   * @do_run: if %TRUE start playback, else if %FALSE stop playback
   *
   * The ::run signal notifies about start/stop playback.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[RUN] =
    g_signal_new("run",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, run),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__BOOLEAN,
		 G_TYPE_NONE, 1,
		 G_TYPE_BOOLEAN);

  /**
   * MonothekDiskjokeySequencerController::completed:
   * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
   *
   * The ::completed signal notifies about completed sequencer session.
   *
   * Since: 1.0.0
   */
  diskjokey_sequencer_controller_signals[COMPLETED] =
    g_signal_new("completed",
		 G_TYPE_FROM_CLASS(diskjokey_sequencer_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekDiskjokeySequencerControllerClass, completed),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_diskjokey_sequencer_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_diskjokey_sequencer_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_diskjokey_sequencer_controller_connect;
  connectable->disconnect = monothek_diskjokey_sequencer_controller_disconnect;
}

void
monothek_diskjokey_sequencer_controller_init(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekActionBox *action_box;
  MonothekActionSlider *action_slider;

  guint i, j;

  /* pattern */
  diskjokey_sequencer_controller->pad = (MonothekActionBox ***) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT * sizeof(MonothekActionBox **));

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    diskjokey_sequencer_controller->pad[i] = (MonothekActionBox **) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT * sizeof(MonothekActionBox *));

    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT / MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; j++){
      gchar *str;

      str = g_strdup_printf("pad: %d,%d", j, i);
      
      diskjokey_sequencer_controller->pad[i][j] =
	action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
							"action-identifier", str,
							"x0", 160 + j * 80,
							"y0", 80 + i * 80,
							"width", 60,
							"height", 60,
							NULL);

      monothek_controller_add_action_box(diskjokey_sequencer_controller,
					 action_box);

      g_free(str);
    }
  }

  /* techno */
  diskjokey_sequencer_controller->techno =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "techno",
						    "x0", 1460,
						    "y0", 80,
						    "width", 420,
						    "height", 140,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* house */
  diskjokey_sequencer_controller->house =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "house",
						    "x0", 1460,
						    "y0", 240,
						    "width", 420,
						    "height", 140,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* hiphop */
  diskjokey_sequencer_controller->hiphop =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "hiphop",
						    "x0", 1460,
						    "y0", 400,
						    "width", 420,
						    "height", 140,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* clear */
  diskjokey_sequencer_controller->clear =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "clear",
						    "x0", 1680,
						    "y0", 560,
						    "width", 200,
						    "height", 60,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* random */
  diskjokey_sequencer_controller->random =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "random",
						    "x0", 1460,
						    "y0", 560,
						    "width", 200,
						    "height", 60,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* run */
  diskjokey_sequencer_controller->run =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "run",
						    "x0", 1460,
						    "y0", 880,
						    "width", 420,
						    "height", 140,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);
  
  /* tab #0 */
  diskjokey_sequencer_controller->tab = (MonothekActionBox **) malloc(4 * sizeof(MonothekActionBox *));

  diskjokey_sequencer_controller->tab[0] =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "tab: 0",
						    "x0", 160,
						    "y0", 800,
						    "width", 300,
						    "height", 60,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* tab #1 */
  diskjokey_sequencer_controller->tab[1] =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "tab: 1",
						    "x0", 480,
						    "y0", 800,
						    "width", 300,
						    "height", 60,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* tab #2 */
  diskjokey_sequencer_controller->tab[2] =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "tab: 2",
						    "x0", 800,
						    "y0", 800,
						    "width", 300,
						    "height", 60,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* tab #3 */
  diskjokey_sequencer_controller->tab[3] =
    action_box = (MonothekActionBox *) g_object_new(MONOTHEK_TYPE_ACTION_BOX,
						    "action-identifier", "tab: 3",
						    "x0", 1120,
						    "y0", 800,
						    "width", 300,
						    "height", 60,
						    NULL);

  monothek_controller_add_action_box(diskjokey_sequencer_controller,
				     action_box);

  /* bpm */
  diskjokey_sequencer_controller->bpm =
    action_slider = (MonothekActionSlider *) g_object_new(MONOTHEK_TYPE_ACTION_SLIDER,
							  "action-identifier", "bpm",
							  "x0", 160,
							  "y0", 990,
							  "width", 620,
							  "height", 60,
							  NULL);
  
  action_slider->adjustment->upper = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_UPPER;
  action_slider->adjustment->lower = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_LOWER;
  action_slider->adjustment->value = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_DEFAULT;
  
  monothek_controller_add_action_slider(diskjokey_sequencer_controller,
					action_slider);
  
  /* position timeout */
  if(monothek_diskjokey_sequencer_controller_position == NULL){
    monothek_diskjokey_sequencer_controller_position = g_hash_table_new_full(g_direct_hash, g_direct_equal,
									     NULL,
									     NULL);
  }

  g_hash_table_insert(monothek_diskjokey_sequencer_controller_position,
                      diskjokey_sequencer_controller, monothek_diskjokey_sequencer_controller_position_timeout);
  
  g_timeout_add(1000 / 30,
                (GSourceFunc) monothek_diskjokey_sequencer_controller_position_timeout,
                (gpointer) diskjokey_sequencer_controller);

  /* progress */
  if(monothek_diskjokey_sequencer_controller_progress_increase == NULL){
    monothek_diskjokey_sequencer_controller_progress_increase = g_hash_table_new_full(g_direct_hash, g_direct_equal,
										      NULL,
										      NULL);
  }

  diskjokey_sequencer_controller->start_time = (struct timespec *) malloc(sizeof(struct timespec));
  diskjokey_sequencer_controller->start_time->tv_sec = 0;
  diskjokey_sequencer_controller->start_time->tv_nsec = 0;
  
  diskjokey_sequencer_controller->timer = (struct timespec *) malloc(sizeof(struct timespec));
  diskjokey_sequencer_controller->timer->tv_sec = 0;
  diskjokey_sequencer_controller->timer->tv_nsec = 0;

  /* progress timeout - add */
  g_hash_table_insert(monothek_diskjokey_sequencer_controller_progress_increase,
		      diskjokey_sequencer_controller, monothek_diskjokey_sequencer_controller_progress_increase_timeout);
  
  g_timeout_add(1000 / 30,
		(GSourceFunc) monothek_diskjokey_sequencer_controller_progress_increase_timeout,
		(gpointer) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_finalize(GObject *gobject)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = (MonothekDiskjokeySequencerController *) gobject;

  /* remove position */
  g_hash_table_remove(monothek_diskjokey_sequencer_controller_position,
                      diskjokey_sequencer_controller);
  
  /* progress timeout - remove */
  g_hash_table_remove(monothek_diskjokey_sequencer_controller_progress_increase,
		      diskjokey_sequencer_controller);

  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_sequencer_controller_parent_class)->finalize(gobject);
}

void
monothek_diskjokey_sequencer_controller_connect(AgsConnectable *connectable)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  guint i, j;

  diskjokey_sequencer_controller = MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER(connectable);

  if(monothek_controller_test_flags(diskjokey_sequencer_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_sequencer_controller_parent_connectable_interface->connect(connectable);

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT / MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; j++){
      g_signal_connect(diskjokey_sequencer_controller->pad[i][j], "enter",
		       G_CALLBACK(monothek_diskjokey_sequencer_controller_pad_enter_callback), diskjokey_sequencer_controller);
      g_signal_connect(diskjokey_sequencer_controller->pad[i][j], "leave",
		       G_CALLBACK(monothek_diskjokey_sequencer_controller_pad_leave_callback), diskjokey_sequencer_controller);
      g_signal_connect(diskjokey_sequencer_controller->pad[i][j], "clicked",
		       G_CALLBACK(monothek_diskjokey_sequencer_controller_pad_clicked_callback), diskjokey_sequencer_controller);
    }
  }

  g_signal_connect(diskjokey_sequencer_controller->techno, "enter",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_enter_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->techno, "leave",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_leave_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->techno, "clicked",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_clicked_callback), diskjokey_sequencer_controller);

  g_signal_connect(diskjokey_sequencer_controller->house, "enter",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_house_enter_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->house, "leave",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_house_leave_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->house, "clicked",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_house_clicked_callback), diskjokey_sequencer_controller);

  g_signal_connect(diskjokey_sequencer_controller->hiphop, "enter",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_enter_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->hiphop, "leave",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_leave_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->hiphop, "clicked",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_clicked_callback), diskjokey_sequencer_controller);

  g_signal_connect(diskjokey_sequencer_controller->clear, "enter",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_enter_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->clear, "leave",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_leave_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->clear, "clicked",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_clicked_callback), diskjokey_sequencer_controller);

  g_signal_connect(diskjokey_sequencer_controller->random, "enter",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_random_enter_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->random, "leave",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_random_leave_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->random, "clicked",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_random_clicked_callback), diskjokey_sequencer_controller);

  for(i = 0; i < 4; i++){
    g_signal_connect(diskjokey_sequencer_controller->tab[i], "enter",
		     G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_enter_callback), diskjokey_sequencer_controller);
    g_signal_connect(diskjokey_sequencer_controller->tab[i], "leave",
		     G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_leave_callback), diskjokey_sequencer_controller);
    g_signal_connect(diskjokey_sequencer_controller->tab[i], "clicked",
		     G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_clicked_callback), diskjokey_sequencer_controller);
  }

  g_signal_connect(diskjokey_sequencer_controller->bpm, "change-value",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_bpm_change_value_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->bpm, "move-slider",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_bpm_move_slider_callback), diskjokey_sequencer_controller);
  
  g_signal_connect(diskjokey_sequencer_controller->run, "enter",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_run_enter_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->run, "leave",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_run_leave_callback), diskjokey_sequencer_controller);
  g_signal_connect(diskjokey_sequencer_controller->run, "clicked",
		   G_CALLBACK(monothek_diskjokey_sequencer_controller_run_clicked_callback), diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_disconnect(AgsConnectable *connectable)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  guint i, j;

  diskjokey_sequencer_controller = MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(diskjokey_sequencer_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_sequencer_controller_parent_connectable_interface->disconnect(connectable);

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT / MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; j++){
      g_object_disconnect(diskjokey_sequencer_controller->pad[i][j],
			  "any_signal::enter",
			  G_CALLBACK(monothek_diskjokey_sequencer_controller_pad_enter_callback),
			  diskjokey_sequencer_controller,
			  "any_signal::leave",
			  G_CALLBACK(monothek_diskjokey_sequencer_controller_pad_leave_callback),
			  diskjokey_sequencer_controller,
			  "any_signal::clicked",
			  G_CALLBACK(monothek_diskjokey_sequencer_controller_pad_clicked_callback),
			  diskjokey_sequencer_controller,
			  NULL);
    }
  }

  g_object_disconnect(diskjokey_sequencer_controller->techno,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_enter_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_leave_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_clicked_callback),
		      diskjokey_sequencer_controller,
		      NULL);

  g_object_disconnect(diskjokey_sequencer_controller->house,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_house_enter_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_house_leave_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_house_clicked_callback),
		      diskjokey_sequencer_controller,
		      NULL);

  g_object_disconnect(diskjokey_sequencer_controller->hiphop,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_enter_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_leave_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_clicked_callback),
		      diskjokey_sequencer_controller,
		      NULL);

  g_object_disconnect(diskjokey_sequencer_controller->clear,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_enter_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_leave_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_clicked_callback),
		      diskjokey_sequencer_controller,
		      NULL);

  g_object_disconnect(diskjokey_sequencer_controller->random,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_random_enter_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_random_leave_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_random_clicked_callback),
		      diskjokey_sequencer_controller,
		      NULL);

  for(i = 0; i < 4; i++){
    g_object_disconnect(diskjokey_sequencer_controller->tab[i],
			"any_signal::enter",
			G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_enter_callback),
			diskjokey_sequencer_controller,
			"any_signal::leave",
			G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_leave_callback),
			diskjokey_sequencer_controller,
			"any_signal::clicked",
			G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_clicked_callback),
			diskjokey_sequencer_controller,
			NULL);
  }

  g_object_disconnect(diskjokey_sequencer_controller->bpm,
		      "any_signal::change-value",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_bpm_change_value_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::move-slider",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_bpm_move_slider_callback),
		      diskjokey_sequencer_controller,
		      NULL);

  g_object_disconnect(diskjokey_sequencer_controller->run,
		      "any_signal::enter",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_run_enter_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::leave",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_run_leave_callback),
		      diskjokey_sequencer_controller,
		      "any_signal::clicked",
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_run_clicked_callback),
		      diskjokey_sequencer_controller,
		      NULL);
}

void
monothek_diskjokey_sequencer_controller_reset(MonothekController *controller)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;
  
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *value;

  diskjokey_sequencer_controller = MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER(controller);
  
  g_object_get(diskjokey_sequencer_controller,
	       "model", &diskjokey_sequencer_model,
	       "view", &diskjokey_sequencer_view,
	       NULL);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* check preserve jukebox FALSE */
  value = g_hash_table_lookup(session->value,
			      "preserve-diskjokey");

  if(!g_value_get_boolean(value)){
    gdouble bpm;
    gdouble swing;
    
#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    g_value_set_boolean(value,
			TRUE);
    
    diskjokey_sequencer_model->current_genre = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO;

    diskjokey_sequencer_model->techno_active = TRUE;
    diskjokey_sequencer_model->house_active = FALSE;
    diskjokey_sequencer_model->hiphop_active = FALSE;

    diskjokey_sequencer_model->random_active = FALSE;
    diskjokey_sequencer_model->clear_active = FALSE;

    diskjokey_sequencer_model->run_active = FALSE;

    diskjokey_sequencer_model->active_column = -1;

    diskjokey_sequencer_model->current_tab = 0;

    diskjokey_sequencer_model->tab_active[0] = TRUE;
    diskjokey_sequencer_model->tab_active[1] = FALSE;
    diskjokey_sequencer_model->tab_active[2] = FALSE;
    diskjokey_sequencer_model->tab_active[3] = FALSE;

    /* pads */
    monothek_diskjokey_sequencer_controller_clear(diskjokey_sequencer_controller);

    /* bpm */
    bpm = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_DEFAULT;
  
    diskjokey_sequencer_model->bpm = bpm;
    monothek_diskjokey_sequencer_controller_change_bpm(diskjokey_sequencer_controller,
						       bpm);

    /* swing */
    swing = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_SWING_DEFAULT;
  
    diskjokey_sequencer_model->swing = swing;
    monothek_diskjokey_sequencer_controller_change_swing(diskjokey_sequencer_controller,
							 swing);
  
    /* load drum kit */
    monothek_diskjokey_sequencer_controller_load_drum_kit(diskjokey_sequencer_controller,
							  MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME);

    /* reset timer */
#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    diskjokey_sequencer_controller->start_time->tv_sec = mts.tv_sec;
    diskjokey_sequencer_controller->start_time->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, diskjokey_sequencer_controller->start_time);
#endif

    diskjokey_sequencer_controller->timer->tv_sec = 0;
    diskjokey_sequencer_controller->timer->tv_nsec = 0;
  }
    
  g_object_set(diskjokey_sequencer_model,
	       "run-active", TRUE,
	       NULL);
  
  gtk_widget_queue_draw(diskjokey_sequencer_view);

  /* start run */
  monothek_diskjokey_sequencer_controller_run(diskjokey_sequencer_controller,
					      TRUE);
}

void
monothek_diskjokey_sequencer_controller_pad_enter_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  gchar *action_identifier;
  
  guint x, y;
  
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_get(action_box,
	       "action-identifier", &action_identifier,
	       NULL);
  
  sscanf(action_identifier, "pad: %d,%d", &x, &y);

  x += (model->current_tab * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT);
  
  monothek_diskjokey_sequencer_model_set_pad_active(model,
						    x, y,
						    TRUE);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_pad_leave_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *start_channel, *channel;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GList *start_pattern;

  gchar *action_identifier;  

  guint x, y;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_get(action_box,
	       "action-identifier", &action_identifier,
	       NULL);
  
  sscanf(action_identifier, "pad: %d,%d", &x, &y);

  x += (model->current_tab * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "input", &start_channel,
	       NULL);
  
  channel = ags_channel_pad_nth(start_channel,
				y);

  g_object_get(channel,
	       "pattern", &start_pattern,
	       NULL);

  if(!ags_pattern_get_bit(start_pattern->data,
			  0, 0, x)){
    monothek_diskjokey_sequencer_model_set_pad_active(model,
						      x, y,
						      FALSE);
    gtk_widget_queue_draw(view);
  }

  g_object_unref(start_channel);
  g_object_unref(channel);
  
  g_list_free_full(start_pattern,
		   g_object_unref);
}

void
monothek_diskjokey_sequencer_controller_pad_clicked_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *start_channel, *channel;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GList *start_pattern;

  gchar *action_identifier;
  
  guint x, y;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_get(action_box,
	       "action-identifier", &action_identifier,
	       NULL);
  
  sscanf(action_identifier, "pad: %d,%d", &x, &y);

  x += (model->current_tab * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "input", &start_channel,
	       NULL);
  
  channel = ags_channel_pad_nth(start_channel,
				y);

  g_object_get(channel,
	       "pattern", &start_pattern,
	       NULL);

  if(ags_pattern_get_bit(start_pattern->data,
			 0, 0, x)){
    monothek_diskjokey_sequencer_model_set_pad_active(model,
						      x, y,
						      FALSE);
  }else{
    monothek_diskjokey_sequencer_model_set_pad_active(model,
						      x, y,
						      TRUE);
  }
  
  gtk_widget_queue_draw(view);

  monothek_diskjokey_sequencer_controller_toggle_pad(diskjokey_sequencer_controller,
						     x, y);

  g_object_unref(start_channel);

  g_object_unref(channel);
  
  g_list_free_full(start_pattern,
		   g_object_unref);
}

void
monothek_diskjokey_sequencer_controller_techno_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "techno-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_techno_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "techno-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_techno_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "house-active", FALSE,
	       "hiphop-active", FALSE,
	       NULL);

  model->current_genre = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO;
  gtk_widget_queue_draw(view);

  monothek_diskjokey_sequencer_controller_load_drum_kit(diskjokey_sequencer_controller,
							MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME);
}

void
monothek_diskjokey_sequencer_controller_house_enter_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "house-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_house_leave_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "house-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_house_clicked_callback(MonothekActionBox *action_box,
							       MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "techno-active", FALSE,
	       "hiphop-active", FALSE,
	       NULL);

  model->current_genre = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE;
  gtk_widget_queue_draw(view);

  monothek_diskjokey_sequencer_controller_load_drum_kit(diskjokey_sequencer_controller,
							MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE_FILENAME);
}

void
monothek_diskjokey_sequencer_controller_hiphop_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "hiphop-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_hiphop_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "hiphop-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_hiphop_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "techno-active", FALSE,
	       "house-active", FALSE,
	       NULL);

  model->current_genre = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP;
  gtk_widget_queue_draw(view);

  monothek_diskjokey_sequencer_controller_load_drum_kit(diskjokey_sequencer_controller,
							MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP_FILENAME);
}

void
monothek_diskjokey_sequencer_controller_random_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "random-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_random_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "random-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_random_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  monothek_diskjokey_sequencer_controller_random(diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_clear_enter_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "clear-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_clear_leave_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "clear-active", FALSE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_clear_clicked_callback(MonothekActionBox *action_box,
							       MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  monothek_diskjokey_sequencer_controller_clear(diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_tab_enter_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  gchar *action_identifier;  

  guint x;
  guint i;
  
  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_get(action_box,
	       "action-identifier", &action_identifier,
	       NULL);
  
  sscanf(action_identifier, "tab: %d", &x);

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; i++){
    gboolean is_active;

    is_active = FALSE;

    if(i == x){
      is_active = TRUE;
    }

    if(i == model->current_tab){
      is_active = TRUE;
    }
    
    monothek_diskjokey_sequencer_model_set_tab_active(model,
						      i,
						      is_active);
  }
  
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_tab_leave_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  gchar *action_identifier;  

  guint x;
  guint i;
  
  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_get(action_box,
	       "action-identifier", &action_identifier,
	       NULL);
  
  sscanf(action_identifier, "tab: %d", &x);

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; i++){
    gboolean is_active;

    is_active = FALSE;
    
    if(i == x){
      is_active = FALSE;
    }

    if(i == model->current_tab){
      is_active = TRUE;
    }

    monothek_diskjokey_sequencer_model_set_tab_active(model,
						      i,
						      is_active);
  }
  
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_tab_clicked_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  gchar *action_identifier;  

  guint x;
  guint i;
  
  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_get(action_box,
	       "action-identifier", &action_identifier,
	       NULL);
  
  sscanf(action_identifier, "tab: %d", &x);

  model->current_tab = x;

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TAB_COUNT; i++){
    gboolean is_active;

    is_active = FALSE;

    if(i == model->current_tab){
      is_active = TRUE;
    }
    
    monothek_diskjokey_sequencer_model_set_tab_active(model,
						      i,
						      is_active);
  }

  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_bpm_change_value_callback(MonothekActionBox *action_box,
								  gdouble new_value,
								  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  model->bpm = new_value;
  gtk_widget_queue_draw(view);
  
  monothek_diskjokey_sequencer_controller_change_bpm(diskjokey_sequencer_controller,
						     new_value);
}

void
monothek_diskjokey_sequencer_controller_bpm_move_slider_callback(MonothekActionBox *action_box,
								 MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "view", &view,
	       NULL);

  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_swing_change_value_callback(MonothekActionBox *action_box,
								    gdouble new_value,
								    MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  monothek_diskjokey_sequencer_controller_change_swing(diskjokey_sequencer_controller,
						       new_value);
}

void
monothek_diskjokey_sequencer_controller_swing_move_slider_callback(MonothekActionBox *action_box,
								   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "view", &view,
	       NULL);

  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_run_enter_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  g_object_set(model,
	       "run-active", TRUE,
	       NULL);
  gtk_widget_queue_draw(view);
}

void
monothek_diskjokey_sequencer_controller_run_leave_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;
  
  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *channel;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GList *recall_id;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* check scope */
  sequencer = rack->sequencer;

  recall_id = ags_audio_check_scope(sequencer,
				    AGS_SOUND_SCOPE_SEQUENCER);
  
  /* model and view */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  if(recall_id == NULL){
    g_object_set(model,
		 "run-active", FALSE,
		 NULL);
    gtk_widget_queue_draw(view);
  }

  g_list_free(recall_id);
}

void
monothek_diskjokey_sequencer_controller_run_clicked_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //empty
}

void
monothek_diskjokey_sequencer_controller_real_toggle_pad(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							guint x, guint y)
{
  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *start_channel, *channel, *next_channel;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  guint audio_channels;
  guint i;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "audio-channels", &audio_channels,
	       "input", &start_channel,
	       NULL);

  channel = ags_channel_pad_nth(start_channel,
				y);

  for(i = 0; i < audio_channels; i++){
    GList *start_pattern;

    g_object_get(channel,
		 "pattern", &start_pattern,
		 NULL);
    
    ags_pattern_toggle_bit(start_pattern->data,
			   0, 0, x);

    g_list_free_full(start_pattern,
		     g_object_unref);

    /* iterate */
    next_channel = ags_channel_next(channel);

    g_object_unref(channel);

    channel = next_channel;
  }

  g_object_unref(start_channel);
  
  if(channel != NULL){
    g_object_unref(channel);
  }
}

/**
 * monothek_diskjokey_sequencer_controller_toggle_pad:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @x: the event's pad x offset
 * @y: the event's pad y offset
 * 
 * Toggling pad.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_toggle_pad(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						   guint x, guint y)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[TOGGLE_PAD], 0,
		x, y);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_switch_page(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							 guint page_index)
{
  //TODO:JK: implement me
}

/**
 * monothek_diskjokey_sequencer_controller_switch_page:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @page_index: the page's index
 * 
 * Change duration to @step_count.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_switch_page(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						    guint page_index)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[SWITCH_PAGE], 0,
		page_index);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_select_bank(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							 guint bank_0, guint bank_1)
{
  //TODO:JK: implement me
}

/**
 * monothek_diskjokey_sequencer_controller_select_bank:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @bank_0: the bank's index 0
 * @bank_1: the bank's index 1
 * 
 * Toggling pad.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_select_bank(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						    guint bank_0, guint bank_1)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[SELECT_BANK], 0,
		bank_0, bank_1);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_change_bpm(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							gdouble bpm)
{
  AgsApplyBpm *apply_bpm;

  AgsTaskThread *task_thread;

  AgsApplicationContext *application_context;

  application_context = ags_application_context_get_instance();

  task_thread = ags_concurrency_provider_get_task_thread(AGS_CONCURRENCY_PROVIDER(application_context));

  /* get task thread */
  apply_bpm = ags_apply_bpm_new(application_context,
				bpm);

  ags_task_thread_append_task(task_thread,
			      apply_bpm);
}

/**
 * monothek_diskjokey_sequencer_controller_change_bpm:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @bpm: the bpm
 * 
 * Change bpm.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_change_bpm(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						   gdouble bpm)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[CHANGE_BPM], 0,
		bpm);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_change_swing(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							  gdouble swing)
{
  //TODO:JK: implement me
}

/**
 * monothek_diskjokey_sequencer_controller_change_swing:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @swing: the amount to swing ranging from 0.0 to 1.0
 * 
 * Change swing with the amount of @swing.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_change_swing(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						     gdouble swing)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[CHANGE_SWING], 0,
		swing);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_change_duration(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							     guint step_count)
{
  //TODO:JK: implement me
}

/**
 * monothek_diskjokey_sequencer_controller_change_duration:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @step_count: the count of steps
 * 
 * Change duration to @step_count.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_change_duration(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							guint step_count)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[CHANGE_DURATION], 0,
		step_count);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_load_drum_kit(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							   gchar *drum_kit)
{
  MonothekDiskjokeySequencerModel *model;

  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *start_input, *input, *next_input;
  
  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GObject *output_soundcard;
  
  gchar **iter;

  guint audio_channels;
  guint i;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* model */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       NULL);
  
  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* start/stop sequencer */
  sequencer = rack->sequencer;

  g_object_get(sequencer,
	       "audio-channels", &audio_channels,
	       "output-soundcard", &output_soundcard,
	       "input", &start_input,
	       NULL);

  input = start_input;
  g_object_ref(input);
  
  if(!g_strcmp0(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME,
		drum_kit)){
    iter = model->techno_sample;

    for(; iter[0] != NULL; iter++){
      AgsRecycling *recycling;
      AgsAudioFile *audio_file;

      GList *audio_signal;
      GList *start_recall, *recall;
      
      gchar *str;

      guint pad;
      
      for(i = 0; i < audio_channels; i++){
	g_message("%s", iter[0]);
      
	/* open audio file and read audio signal */
	audio_file = ags_audio_file_new(iter[0],
					output_soundcard,
					i);

	if(!ags_audio_file_open(audio_file)){
	  g_message("unable to open file - %s", iter[0]);
	
	  continue;
	}

	ags_audio_file_read_audio_signal(audio_file);
      
	audio_signal = audio_file->audio_signal;
      
	g_object_get(input,
		     "pad", &pad,
		     "first-recycling", &recycling,
		     "recall", &start_recall,
		     NULL);

	ags_audio_signal_set_flags(audio_signal->data,
				   AGS_AUDIO_SIGNAL_TEMPLATE);
	ags_recycling_add_audio_signal(recycling,
				       audio_signal->data);

	/* apply know controls */
	str = g_hash_table_lookup(model->techno_control[pad],
				  "volume");

	if(str != NULL){
	  recall = ags_recall_template_find_type(start_recall,
						 AGS_TYPE_VOLUME_CHANNEL);

	  if(recall != NULL){
	    AgsPort *port;
	    
	    GValue value = {0,};

	    g_object_get(recall->data,
			 "volume", &port,
			 NULL);

	    g_value_init(&value, G_TYPE_FLOAT);
	    g_value_set_float(&value, g_ascii_strtod(str,
						     NULL));

	    ags_port_safe_write(port,
				&value);
	  }
	}

	g_list_free(start_recall);
	
	/* iterate */
	next_input = ags_channel_next(input);

	g_object_unref(input);

	input = next_input;

	//	audio_signal = audio_signal->next;
      }
    }
  }else if(!g_strcmp0(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE_FILENAME,
		      drum_kit)){
    iter = model->house_sample;
    
    for(; iter[0] != NULL; iter++){
      AgsRecycling *recycling;
      AgsAudioFile *audio_file;

      GList *audio_signal;
      GList *start_recall, *recall;
      
      gchar *str;

      guint pad;

      /* open audio file and read audio signal */
      audio_file = ags_audio_file_new(iter[0],
				      output_soundcard,
				      -1);

      if(!ags_audio_file_open(audio_file)){
	g_message("unable to open file - %s", iter[0]);
	
	continue;
      }
	   
      ags_audio_file_read_audio_signal(audio_file);
	   
      audio_signal = audio_file->audio_signal;
      
      for(i = 0; i < audio_channels && audio_signal != NULL; i++){
	g_message("%s", iter[0]);

	g_object_get(input,
		     "first-recycling", &recycling,
		     NULL);

	ags_audio_signal_set_flags(audio_signal->data,
				   AGS_AUDIO_SIGNAL_TEMPLATE);
	ags_recycling_add_audio_signal(recycling,
				       audio_signal->data);

	/* apply know controls */
	str = g_hash_table_lookup(model->house_control[pad],
				  "volume");

	if(str != NULL){
	  recall = ags_recall_template_find_type(start_recall,
						 AGS_TYPE_VOLUME_CHANNEL);

	  if(recall != NULL){
	    AgsPort *port;
	    
	    GValue value = {0,};

	    g_object_get(recall->data,
			 "volume", &port,
			 NULL);

	    g_value_init(&value, G_TYPE_FLOAT);
	    g_value_set_float(&value, g_ascii_strtod(str,
						     NULL));

	    ags_port_safe_write(port,
				&value);
	  }
	}

	g_list_free(start_recall);

	/* iterate */
	next_input = ags_channel_next(input);

	g_object_unref(input);

	input = next_input;

	//	audio_signal = audio_signal->next;
      }
    }
  }else if(!g_strcmp0(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP_FILENAME,
		      drum_kit)){
    iter = model->hiphop_sample;

    for(; iter[0] != NULL; iter++){
      AgsRecycling *recycling;
      AgsAudioFile *audio_file;

      GList *audio_signal;
      GList *start_recall, *recall;
      
      gchar *str;

      guint pad;

      /* open audio file and read audio signal */
      audio_file = ags_audio_file_new(iter[0],
				      output_soundcard,
				      -1);

      if(!ags_audio_file_open(audio_file)){
	g_message("unable to open file - %s", iter[0]);
	
	continue;
      }

      ags_audio_file_read_audio_signal(audio_file);
      
      audio_signal = audio_file->audio_signal;
      
      for(i = 0; i < audio_channels && audio_signal != NULL; i++){
	g_message("%s", iter[0]);

	g_object_get(input,
		     "first-recycling", &recycling,
		     NULL);

	ags_audio_signal_set_flags(audio_signal->data,
				   AGS_AUDIO_SIGNAL_TEMPLATE);
	ags_recycling_add_audio_signal(recycling,
				       audio_signal->data);

	/* apply know controls */
	str = g_hash_table_lookup(model->hiphop_control[pad],
				  "volume");

	if(str != NULL){
	  recall = ags_recall_template_find_type(start_recall,
						 AGS_TYPE_VOLUME_CHANNEL);

	  if(recall != NULL){
	    AgsPort *port;
	    
	    GValue value = {0,};

	    g_object_get(recall->data,
			 "volume", &port,
			 NULL);

	    g_value_init(&value, G_TYPE_FLOAT);
	    g_value_set_float(&value, g_ascii_strtod(str,
						     NULL));

	    ags_port_safe_write(port,
				&value);
	  }
	}

	g_list_free(start_recall);

	/* iterate */
	next_input = ags_channel_next(input);

	g_object_unref(input);

	input = next_input;

	//	audio_signal = audio_signal->next;
      }
    }
  }

  g_object_unref(start_input);
}

/**
 * monothek_diskjokey_sequencer_controller_load_drum_kit:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @drum_kit: the filename of the drum kit
 * 
 * Load drum kit from @drum_kit.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_load_drum_kit(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						      gchar *drum_kit)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[LOAD_DRUM_KIT], 0,
		drum_kit);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_clear(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *start_channel, *channel, *next_channel;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  guint audio_channels;
  guint x;
  guint i, j;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* model */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "audio-channels", &audio_channels,
	       "input", &start_channel,
	       NULL);

  channel = start_channel;
  g_object_ref(channel);
  
  while(channel != NULL){
    GList *start_pattern;

    g_object_get(channel,
		 "pattern", &start_pattern,
		 NULL);

    for(x = 0; x < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; x++){
      if(ags_pattern_get_bit(AGS_PATTERN(start_pattern->data), 0, 0, x)){
	ags_pattern_toggle_bit(AGS_PATTERN(start_pattern->data),
			       0, 0, x);
      }
    }

    g_list_free_full(start_pattern,
		     g_object_unref);

    /* iterate */
    next_channel = ags_channel_next(channel);

    g_object_unref(channel);

    channel = next_channel;
  }

  g_object_unref(start_channel);
  
  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; j++){
      monothek_diskjokey_sequencer_model_set_pad_active(model,
							j, i,
							FALSE);
    }
  }

  gtk_widget_queue_draw(view);
}

/**
 * monothek_diskjokey_sequencer_controller_clear:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * 
 * Clear pattern.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_clear(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[CLEAR], 0);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_random(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *start_channel, *channel, *next_channel;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GHashTable **bank;
  
  gchar *bank_name;
  
  guint audio_channels;
  guint x;
  guint i, j;
  
  GValue *rack_value;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* model */
  g_object_get(diskjokey_sequencer_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "audio-channels", &audio_channels,
	       "input", &start_channel,
	       NULL);

  bank = NULL;
  bank_name = NULL;
  
  if(model->current_genre == MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO){
    bank = model->techno_bank;

    if(model->techno_bank_name != NULL){
      bank_name = model->techno_bank_name[random() % g_strv_length(model->techno_bank_name)];
    }
  }else if(model->current_genre == MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE){
    bank = model->house_bank;

    if(model->house_bank_name != NULL){
      bank_name = model->house_bank_name[random() % g_strv_length(model->house_bank_name)];
    }
  }else if(model->current_genre == MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP){
    bank = model->hiphop_bank;

    if(model->hiphop_bank_name != NULL){
      bank_name = model->hiphop_bank_name[random() % g_strv_length(model->hiphop_bank_name)];
    }
  }

  if(bank_name != NULL){
    channel = start_channel;
    g_object_ref(channel);
    
    for(i = 0; channel != NULL; i++){
      GList *start_pattern;

      guint *pattern_data;

      guint pad;
      gboolean success;
      
      g_object_get(channel,
		   "pad", &pad,
		   "pattern", &start_pattern,
		   NULL);
      
      success = g_hash_table_lookup_extended(bank[pad],
					     bank_name,
					     NULL,
					     &pattern_data);

      if(success){
	if(pattern_data != NULL){
	  memcpy(AGS_PATTERN(start_pattern->data)->pattern[0][0], pattern_data, (guint) ceil(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT / (8 * sizeof(guint))) * sizeof(guint));
	}

	for(x = 0, j = 0; x < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; x++, j++){	
	  if(ags_pattern_get_bit(AGS_PATTERN(start_pattern->data), 0, 0, x)){
	    monothek_diskjokey_sequencer_model_set_pad_active(model,
							      j, pad,
							      TRUE);
	  }else{
	    monothek_diskjokey_sequencer_model_set_pad_active(model,
							      j, pad,
							      FALSE);
	  }
	}
      }else{
	memset(AGS_PATTERN(start_pattern->data)->pattern[0][0], 0, (guint) ceil(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT / (8 * sizeof(guint))) * sizeof(guint));
	
	for(x = 0, j = 0; x < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; x++, j++){	
	  monothek_diskjokey_sequencer_model_set_pad_active(model,
							    j, pad,
							    FALSE);
	}
      }
      
      g_list_free_full(start_pattern,
		       g_object_unref);

      /* iterate */
      next_channel = ags_channel_next(channel);

      g_object_unref(channel);
      
      channel = next_channel;
    }
  }

  g_object_unref(start_channel);
  
  gtk_widget_queue_draw(view);
}

/**
 * monothek_diskjokey_sequencer_controller_random:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * 
 * Fill random pattern.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_random(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[RANDOM], 0);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_progress(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						      gdouble value)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;
  
  MonothekSessionManager *session_manager;
  MonothekSession *session;

  g_object_get(diskjokey_sequencer_controller,
	       "view", &diskjokey_sequencer_view,
	       NULL);

  gtk_adjustment_set_value(diskjokey_sequencer_view->progress,
			   value);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* stop sequencer */
  if(value >= 1.0){
    monothek_diskjokey_sequencer_controller_run(diskjokey_sequencer_controller,
						FALSE);

    diskjokey_sequencer_controller->start_time->tv_sec = 0;
    diskjokey_sequencer_controller->start_time->tv_nsec = 0;

    /* emit completed */
    monothek_diskjokey_sequencer_controller_completed(diskjokey_sequencer_controller);
  }
}

/**
 * monothek_diskjokey_sequencer_controller_progress:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @value: a gdouble ranging from 0.0 to 1.0
 * 
 * Notify about progress.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_progress(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						 gdouble value)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[PROGRESS], 0,
		value);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_run(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						 gboolean do_run)
{
  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *channel;

  AgsTaskThread *task_thread;
  
  AgsApplicationContext *application_context;  
  MonothekSessionManager *session_manager;
  MonothekSession *session;
  
  GValue *rack_value;

  application_context = ags_application_context_get_instance();

  task_thread = ags_concurrency_provider_get_task_thread(AGS_CONCURRENCY_PROVIDER(application_context));

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* start/stop sequencer */
  sequencer = rack->sequencer;

  if(do_run){
    AgsStartAudio *start_audio;
    MonothekExportOutput *export_output;
    AgsStartSoundcard *start_soundcard;

    MonothekExportThread *export_thread;

    GObject *output_soundcard;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec duration;
    
    GList *task;

    export_thread = MONOTHEK_APPLICATION_CONTEXT(application_context)->default_export_thread;
    
    g_object_get(sequencer,
		 "output-soundcard", &output_soundcard,
		 NULL);
    
#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    diskjokey_sequencer_controller->start_time->tv_sec = mts.tv_sec;
    diskjokey_sequencer_controller->start_time->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, diskjokey_sequencer_controller->start_time);
#endif

    /* start audio */
    task = NULL;
    
    start_audio = ags_start_audio_new(sequencer,
				      AGS_SOUND_SCOPE_SEQUENCER);
    task = g_list_prepend(task,
			  start_audio);
    
    duration.tv_sec = 600;
    duration.tv_nsec = 0;

    export_output = monothek_export_output_new(export_thread,
					       output_soundcard,
					       &duration);
    task = g_list_prepend(task,
			  export_output);    
    
    start_soundcard = ags_start_soundcard_new(application_context);
    task = g_list_prepend(task,
			  start_soundcard);

    task = g_list_reverse(task);
    
    ags_task_thread_append_tasks(task_thread,
				 task);

    g_object_unref(output_soundcard);
  }else{
    AgsCancelAudio *cancel_audio;

    GList *task;
    
    /* cancel audio */
    task = NULL;
    
    cancel_audio = ags_cancel_audio_new(sequencer,
					AGS_SOUND_SCOPE_SEQUENCER);
    task = g_list_prepend(task,
			  cancel_audio);

    task = g_list_reverse(task);
    
    ags_task_thread_append_tasks(task_thread,
				 task);
  }
}

/**
 * monothek_diskjokey_sequencer_controller_run:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * @do_run: if %TRUE start playback, else if %FALSE stop playback
 * 
 * Run step sequencer.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_run(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
					    gboolean do_run)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[RUN], 0,
		do_run);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

void
monothek_diskjokey_sequencer_controller_real_completed(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekWindow *window;
  MonothekDiskjokeySequencerView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  /* change view */
  g_object_get(diskjokey_sequencer_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* change view */
  monothek_window_change_view(window,
			      MONOTHEK_TYPE_DISKJOKEY_END_VIEW, G_TYPE_NONE);
}

/**
 * monothek_diskjokey_sequencer_controller_completed:
 * @diskjokey_sequencer_controller: the #MonothekDiskjokeySequencerController
 * 
 * Completed sequencer session.
 * 
 * Since: 1.0.0
 */
void
monothek_diskjokey_sequencer_controller_completed(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  g_return_if_fail(MONOTHEK_IS_DISKJOKEY_SEQUENCER_CONTROLLER(diskjokey_sequencer_controller));
  
  g_object_ref((GObject *) diskjokey_sequencer_controller);
  g_signal_emit(G_OBJECT(diskjokey_sequencer_controller),
		diskjokey_sequencer_controller_signals[COMPLETED], 0);
  g_object_unref((GObject *) diskjokey_sequencer_controller);
}

gboolean
monothek_diskjokey_sequencer_controller_position_timeout(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  if(g_hash_table_lookup(monothek_diskjokey_sequencer_controller_position,
                         diskjokey_sequencer_controller) != NULL){
    MonothekDiskjokeySequencerView *view;
    
    MonothekDiskjokeySequencerModel *model;
    
    MonothekRack *rack;
    AgsAudio *sequencer;
    AgsRecyclingContext *recycling_context, *parent_recycling_context;

    MonothekSessionManager *session_manager;
    MonothekSession *session;

    GList *start_recall_id, *recall_id;
    
    GValue *rack_value;

    /* find session */
    session_manager = monothek_session_manager_get_instance();
    session = monothek_session_manager_find_session(session_manager,
						    MONOTHEK_SESSION_DEFAULT_SESSION);

    /* model */
    g_object_get(diskjokey_sequencer_controller,
		 "model", &model,
		 "view", &view,
		 NULL);

    /* get rack */
    rack_value = g_hash_table_lookup(session->value,
				     "rack");

    if(rack_value == NULL){
      return(TRUE);
    }

    rack = g_value_get_object(rack_value);

    if(rack == NULL){
      return(TRUE);
    }
    
    /* start/stop sequencer */
    sequencer = rack->sequencer;

    recall_id = 
      start_recall_id = ags_audio_check_scope(sequencer,
					      AGS_SOUND_SCOPE_SEQUENCER);

    recycling_context = NULL;
    parent_recycling_context = NULL;
    
    while(recall_id != NULL){
      g_object_get(recall_id->data,
		   "recycling-context", &recycling_context,
		   NULL);

      if(recycling_context != NULL){
	g_object_get(recycling_context,
		     "parent", &parent_recycling_context,
		     NULL);

	if(parent_recycling_context == NULL){
	  break;
	}
      }
      
      recall_id = recall_id->next;
    }
    
    if(recall_id != NULL){
      GList *start_play, *play;

      g_object_get(sequencer,
		   "play", &start_play,
		   NULL);

      play = ags_recall_find_type_with_recycling_context(start_play,
							 AGS_TYPE_COUNT_BEATS_AUDIO_RUN,
							 (GObject *) recycling_context);

      if(play != NULL){
	guint64 sequencer_counter;
	
	g_object_get(play->data,
		     "sequencer-counter", &sequencer_counter,
		     NULL);

	model->active_column = (guint) sequencer_counter;
	
	gtk_widget_queue_draw(view);
      }else{
	model->active_column = -1;
      }

      g_list_free_full(start_play,
		       g_object_unref);
    }else{
      model->active_column = -1;
    }

    if(recycling_context != NULL){
      g_object_unref(recycling_context);
    }

    if(parent_recycling_context != NULL){
      g_object_unref(parent_recycling_context);
    }
    
    g_list_free_full(start_recall_id,
		     g_object_unref);
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

gboolean
monothek_diskjokey_sequencer_controller_progress_increase_timeout(GObject *gobject)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = gobject;
  
  if(g_hash_table_lookup(monothek_diskjokey_sequencer_controller_progress_increase,
			 gobject) != NULL){
    MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

#ifdef __APPLE__
    clock_serv_t cclock;
    mach_timespec_t mts;
#endif

    struct timespec *duration;
    struct timespec time_now;
    
    gdouble value;

    if(diskjokey_sequencer_controller->start_time->tv_sec == 0){
      return(TRUE);
    }
    
    g_object_get(diskjokey_sequencer_controller,
		 "model", &diskjokey_sequencer_model,
		 NULL);
    
    g_object_get(diskjokey_sequencer_model,
		 "duration", &duration,
		 NULL);
    
#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    time_now.tv_sec = mts.tv_sec;
    time_now.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, &time_now);
#endif

    /* calculate timer */
    if(time_now.tv_nsec >= diskjokey_sequencer_controller->start_time->tv_nsec){
      diskjokey_sequencer_controller->timer->tv_sec = time_now.tv_sec - diskjokey_sequencer_controller->start_time->tv_sec;
      diskjokey_sequencer_controller->timer->tv_nsec = time_now.tv_nsec - diskjokey_sequencer_controller->start_time->tv_nsec;
    }else{
      diskjokey_sequencer_controller->timer->tv_sec = time_now.tv_sec - diskjokey_sequencer_controller->start_time->tv_sec - 1;
      diskjokey_sequencer_controller->timer->tv_nsec = NSEC_PER_SEC - diskjokey_sequencer_controller->start_time->tv_nsec + time_now.tv_sec;
    }

    /* calculate progress */
    if(duration->tv_sec > 0){
      if(duration->tv_sec < diskjokey_sequencer_controller->timer->tv_sec){
	value = 1.0;
      }else{
	value = 1.0 / duration->tv_sec * diskjokey_sequencer_controller->timer->tv_sec;
      }
    }else{
      value = 0.0;
    }
    
    monothek_diskjokey_sequencer_controller_progress(diskjokey_sequencer_controller,
						     value);
    
    return(TRUE);
  }else{
    return(FALSE);
  }
}

/**
 * monothek_diskjokey_sequencer_controller_new:
 *
 * Creates an #MonothekDiskjokeySequencerController
 *
 * Returns: a new #MonothekDiskjokeySequencerController
 *
 * Since: 1.0.0
 */
MonothekDiskjokeySequencerController*
monothek_diskjokey_sequencer_controller_new()
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = (MonothekDiskjokeySequencerController *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER,
											 NULL);
  
  return(diskjokey_sequencer_controller);
}
