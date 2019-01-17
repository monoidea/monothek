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

#include <monothek/ui/controller/monothek_diskjokey_sequencer_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/audio/monothek_rack.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_sequencer_model.h>

#include <monothek/ui/view/monothek_diskjokey_sequencer_view.h>
#include <monothek/ui/view/monothek_diskjokey_end_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_sequencer_controller_class_init(MonothekDiskjokeySequencerControllerClass *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_sequencer_controller_init(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_finalize(GObject *gobject);

void monothek_diskjokey_sequencer_controller_connect(AgsConnectable *connectable);
void monothek_diskjokey_sequencer_controller_disconnect(AgsConnectable *connectable);

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

void monothek_diskjokey_sequencer_controller_real_run(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						      gboolean do_run);

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
     RUN,
     LAST_SIGNAL,
};

static gpointer monothek_diskjokey_sequencer_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_diskjokey_sequencer_controller_parent_connectable_interface;

static guint diskjokey_sequencer_controller_signals[LAST_SIGNAL];

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

  monothek_diskjokey_sequencer_controller_parent_class = g_type_class_peek_parent(diskjokey_sequencer_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_sequencer_controller;

  gobject->finalize = monothek_diskjokey_sequencer_controller_finalize;

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

  diskjokey_sequencer_controller->run = monothek_diskjokey_sequencer_controller_real_run;

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

  guint i, j;

  /* pattern */
  diskjokey_sequencer_controller->pad = (MonothekActionBox ***) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT * sizeof(MonothekActionBox **));

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
    diskjokey_sequencer_controller->pad[i] = (MonothekActionBox **) malloc(MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT * sizeof(MonothekActionBox *));

    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; j++){
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

}

void
monothek_diskjokey_sequencer_controller_finalize(GObject *gobject)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = (MonothekDiskjokeySequencerController *) gobject;
  
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
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; j++){
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
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; j++){
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
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_techno_enter_callback), diskjokey_sequencer_controller,
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
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_house_enter_callback), diskjokey_sequencer_controller,
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
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_hiphop_enter_callback), diskjokey_sequencer_controller,
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
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_clear_enter_callback), diskjokey_sequencer_controller,
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
		      G_CALLBACK(monothek_diskjokey_sequencer_controller_random_enter_callback), diskjokey_sequencer_controller,
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
			G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_enter_callback), diskjokey_sequencer_controller,
			diskjokey_sequencer_controller,
			"any_signal::leave",
			G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_leave_callback),
			diskjokey_sequencer_controller,
			"any_signal::clicked",
			G_CALLBACK(monothek_diskjokey_sequencer_controller_tab_clicked_callback),
			diskjokey_sequencer_controller,
			NULL);
  }
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
  AgsChannel *channel;

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

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "input", &channel,
	       NULL);
  
  channel = ags_channel_pad_nth(channel,
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
}

void
monothek_diskjokey_sequencer_controller_pad_clicked_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  MonothekDiskjokeySequencerView *view;
  
  MonothekDiskjokeySequencerModel *model;

  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *channel;

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
  

  /* get rack */
  rack_value = g_hash_table_lookup(session->value,
				   "rack");

  rack = g_value_get_object(rack_value);

  /* input */
  sequencer = rack->sequencer;
  g_object_get(sequencer,
	       "input", &channel,
	       NULL);
  
  channel = ags_channel_pad_nth(channel,
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
}

void
monothek_diskjokey_sequencer_controller_techno_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_techno_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_techno_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_house_enter_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_house_leave_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_house_clicked_callback(MonothekActionBox *action_box,
							       MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_hiphop_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_hiphop_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_hiphop_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_random_enter_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_random_leave_callback(MonothekActionBox *action_box,
							      MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_random_clicked_callback(MonothekActionBox *action_box,
								MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_clear_enter_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_clear_leave_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_clear_clicked_callback(MonothekActionBox *action_box,
							       MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_tab_enter_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_tab_leave_callback(MonothekActionBox *action_box,
							   MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_tab_clicked_callback(MonothekActionBox *action_box,
							     MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //TODO:JK: implement me
}

void
monothek_diskjokey_sequencer_controller_real_toggle_pad(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
							guint x, guint y)
{
  MonothekRack *rack;
  AgsAudio *sequencer;
  AgsChannel *channel;

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
	       "input", &channel,
	       NULL);

  channel = ags_channel_pad_nth(channel,
				y);

  for(i = 0; i < audio_channels; i++){
    GList *start_pattern;

    g_object_get(channel,
		 "pattern", &start_pattern,
		 NULL);
    
    ags_pattern_toggle_bit(start_pattern->data,
			   0, 0, x);
    
    g_object_get(channel,
		 "next", &channel,
		 NULL);
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
  //TODO:JK: implement me
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
  //TODO:JK: implement me
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
  //TODO:JK: implement me
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
  //TODO:JK: implement me
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
monothek_diskjokey_sequencer_controller_real_run(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller,
						 gboolean do_run)
{
  //TODO:JK: implement me
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
