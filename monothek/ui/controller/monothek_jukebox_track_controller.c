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

#include <monothek/ui/controller/monothek_jukebox_track_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_track_model.h>

#include <monothek/ui/view/monothek_jukebox_track_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_track_controller_class_init(MonothekJukeboxTrackControllerClass *jukebox_track_controller);
void monothek_jukebox_track_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_track_controller_init(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_finalize(GObject *gobject);

void monothek_jukebox_track_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_track_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_track_controller_real_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_real_play_time_expired(MonothekJukeboxTrackController *jukebox_track_controller);

/**
 * SECTION:monothek_jukebox_track_controller
 * @short_description: The jukebox track controller object.
 * @title: MonothekJukeboxTrackController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_track_controller.h
 *
 * #MonothekJukeboxTrackController is the MVC's jukebox track controller.
 */

enum{
  PLAY_TIME_EXPIRED,
  TEST_TIME_EXPIRED,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_track_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_track_controller_parent_connectable_interface;

static guint jukebox_track_controller_signals[LAST_SIGNAL];

GType
monothek_jukebox_track_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_track_controller = 0;

    static const GTypeInfo monothek_jukebox_track_controller_info = {
      sizeof (MonothekJukeboxTrackControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_track_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxTrackController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_track_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_track_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_track_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								    "MonothekJukeboxTrackController", &monothek_jukebox_track_controller_info,
								    0);

    g_type_add_interface_static(monothek_type_jukebox_track_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_track_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_track_controller_class_init(MonothekJukeboxTrackControllerClass *jukebox_track_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_jukebox_track_controller_parent_class = g_type_class_peek_parent(jukebox_track_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_track_controller;

  gobject->finalize = monothek_jukebox_track_controller_finalize;

  /* MonothekJukeboxTrackController */
  jukebox_track_controller->test_time_expired = monothek_jukebox_track_controller_real_test_time_expired;
  jukebox_track_controller->play_time_expired = monothek_jukebox_track_controller_real_play_time_expired;

  /* signals */
  /**
   * MonothekJukeboxTrackController::test-time-expired:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::test-time-expired signal notifies about test time expired.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[TEST_TIME_EXPIRED] =
    g_signal_new("test-time-expired",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, test_time_expired),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);

  /**
   * MonothekJukeboxTrackController::play-time-expired:
   * @jukebox_track_controller: the #MonothekJukeboxTrackController
   *
   * The ::play-time-expired signal notifies about play time expired.
   *
   * Since: 1.0.0
   */
  jukebox_track_controller_signals[PLAY_TIME_EXPIRED] =
    g_signal_new("play-time-expired",
		 G_TYPE_FROM_CLASS(jukebox_track_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxTrackControllerClass, play_time_expired),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__VOID,
		 G_TYPE_NONE, 0);
}

void
monothek_jukebox_track_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_track_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_track_controller_connect;
  connectable->disconnect = monothek_jukebox_track_controller_disconnect;
}

void
monothek_jukebox_track_controller_init(MonothekJukeboxTrackController *jukebox_track_controller)
{
}

void
monothek_jukebox_track_controller_finalize(GObject *gobject)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = (MonothekJukeboxTrackController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_track_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_track_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = MONOTHEK_JUKEBOX_TRACK_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_track_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_track_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_jukebox_track_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = MONOTHEK_JUKEBOX_TRACK_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_track_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_track_controller_parent_connectable_interface->disconnect(connectable);
}

void
monothek_jukebox_track_controller_real_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller)
{
  //TODO:JK: implement me
}

/**
 * monothek_jukebox_track_controller_test_time_expired:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Test time of track expired.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[TEST_TIME_EXPIRED], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}

void
monothek_jukebox_track_controller_real_play_time_expired(MonothekJukeboxTrackController *jukebox_track_controller)
{
  //TODO:JK: implement me
}

/**
 * monothek_jukebox_track_controller_play_time_expired:
 * @jukebox_track_controller: the #MonothekJukeboxTrackController
 * 
 * Play time of track expired.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_track_controller_play_time_expired(MonothekJukeboxTrackController *jukebox_track_controller)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(jukebox_track_controller));
  
  g_object_ref((GObject *) jukebox_track_controller);
  g_signal_emit(G_OBJECT(jukebox_track_controller),
		jukebox_track_controller_signals[PLAY_TIME_EXPIRED], 0);
  g_object_unref((GObject *) jukebox_track_controller);
}

/**
 * monothek_jukebox_track_controller_new:
 *
 * Creates an #MonothekJukeboxTrackController
 *
 * Returns: a new #MonothekJukeboxTrackController
 *
 * Since: 1.0.0
 */
MonothekJukeboxTrackController*
monothek_jukebox_track_controller_new()
{
  MonothekJukeboxTrackController *jukebox_track_controller;

  jukebox_track_controller = (MonothekJukeboxTrackController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER,
									     NULL);
  
  return(jukebox_track_controller);
}
