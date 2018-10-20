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

#include <monothek/ui/controller/monothek_start_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_start_controller_class_init(MonothekStartControllerClass *start_controller);
void monothek_start_controller_init(MonothekStartController *start_controller);
void monothek_start_controller_finalize(GObject *gobject);

void monothek_start_controller_real_launch_jukebox(MonothekStartController *start_controller);
void monothek_start_controller_real_launch_diskjokey(MonothekStartController *start_controller);

/**
 * SECTION:monothek_start_controller
 * @short_description: The start controller object.
 * @title: MonothekStartController
 * @section_id:
 * @include: monothek/ui/controller/monothek_start_controller.h
 *
 * #MonothekStartController is the MVC's start controller.
 */

enum{
  LAUNCH_JUKEBOX,
  LAUNCH_DISKJOKEY,
  LAST_SIGNAL,
};

static gpointer monothek_start_controller_parent_class = NULL;

static guint start_controller_signals[LAST_SIGNAL];

GType
monothek_start_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_start_controller = 0;

    static const GTypeInfo monothek_start_controller_info = {
      sizeof (MonothekStartControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_start_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekStartController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_start_controller_init,
    };

    monothek_type_start_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
							    "MonothekStartController", &monothek_start_controller_info,
							    0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_start_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_start_controller_class_init(MonothekStartControllerClass *start_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_start_controller_parent_class = g_type_class_peek_parent(start_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) start_controller;

  gobject->finalize = monothek_start_controller_finalize;

  /* MonothekStartController */
  start_controller->launch_jukebox = monothek_start_controller_real_launch_jukebox;
  start_controller->launch_diskjokey = monothek_start_controller_real_launch_diskjokey;

  /* signals */
  /**
   * MonothekStartController::launch-jukebox:
   * @start_controller: the #MonothekStartController
   *
   * The ::launch-jukebox signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  start_controller_signals[LAUNCH_JUKEBOX] =
    g_signal_new("launch-jukebox",
		 G_TYPE_FROM_CLASS(start_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekStartControllerClass, launch_jukebox),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekStartController::launch-diskjokey:
   * @start_controller: the #MonothekStartController
   *
   * The ::launch-diskjokey signal notifies about key pressed.
   *
   * Since: 1.0.0
   */
  start_controller_signals[LAUNCH_DISKJOKEY] =
    g_signal_new("launch-diskjokey",
		 G_TYPE_FROM_CLASS(start_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekStartControllerClass, launch_diskjokey),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_start_controller_init(MonothekStartController *start_controller)
{
  //TODO:JK: implement me
}

void
monothek_start_controller_finalize(GObject *gobject)
{
  MonothekStartController *start_controller;

  start_controller = (MonothekStartController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_start_controller_parent_class)->finalize(gobject);
}

void
monothek_start_controller_real_launch_jukebox(MonothekStartController *start_controller)
{
  //TODO:JK: implement me
}

/**
 * monothek_start_controller_launch_jukebox:
 * @start_controller: the #MonothekStartController
 * 
 * Launch jukebox.
 * 
 * Since: 1.0.0
 */
void
monothek_start_controller_launch_jukebox(MonothekStartController *start_controller)
{
  g_return_if_fail(MONOTHEK_IS_START_CONTROLLER(start_controller));
  
  g_object_ref((GObject *) start_controller);
  g_signal_emit(G_OBJECT(start_controller),
		start_controller_signals[LAUNCH_JUKEBOX], 0);
  g_object_unref((GObject *) start_controller);
}

void
monothek_start_controller_real_launch_diskjokey(MonothekStartController *start_controller)
{
  //TODO:JK: implement me
}

/**
 * monothek_start_controller_launch_diskjokey:
 * @start_controller: the #MonothekStartController
 * 
 * Launch diskjokey.
 * 
 * Since: 1.0.0
 */
void
monothek_start_controller_launch_diskjokey(MonothekStartController *start_controller)
{
  g_return_if_fail(MONOTHEK_IS_START_CONTROLLER(start_controller));
  
  g_object_ref((GObject *) start_controller);
  g_signal_emit(G_OBJECT(start_controller),
		start_controller_signals[LAUNCH_DISKJOKEY], 0);
  g_object_unref((GObject *) start_controller);
}

/**
 * monothek_start_controller_new:
 *
 * Creates an #MonothekStartController
 *
 * Returns: a new #MonothekStartController
 *
 * Since: 1.0.0
 */
MonothekStartController*
monothek_start_controller_new()
{
  MonothekStartController *start_controller;

  start_controller = (MonothekStartController *) g_object_new(MONOTHEK_TYPE_START_CONTROLLER,
							      NULL);
  
  return(start_controller);
}
