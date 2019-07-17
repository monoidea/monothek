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

#include <monothek/ui/controller/monothek_jukebox_playlist_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_playlist_model.h>

#include <monothek/ui/view/monothek_jukebox_playlist_view.h>
#include <monothek/ui/view/monothek_jukebox_playlist_view.h>
#include <monothek/ui/view/monothek_jukebox_track_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_playlist_controller_class_init(MonothekJukeboxPlaylistControllerClass *jukebox_playlist_controller);
void monothek_jukebox_playlist_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_playlist_controller_init(MonothekJukeboxPlaylistController *jukebox_playlist_controller);
void monothek_jukebox_playlist_controller_finalize(GObject *gobject);

void monothek_jukebox_playlist_controller_connect(AgsConnectable *connectable);
void monothek_jukebox_playlist_controller_disconnect(AgsConnectable *connectable);

void monothek_jukebox_playlist_controller_reset(MonothekController *controller);

void monothek_jukebox_playlist_controller_jukebox_song_select_enter_callback(MonothekActionBox *action_box,
									     MonothekJukeboxPlaylistController *jukebox_playlist_controller);
void monothek_jukebox_playlist_controller_jukebox_song_select_leave_callback(MonothekActionBox *action_box,
									     MonothekJukeboxPlaylistController *jukebox_playlist_controller);
void monothek_jukebox_playlist_controller_jukebox_song_select_clicked_callback(MonothekActionBox *action_box,
									       MonothekJukeboxPlaylistController *jukebox_playlist_controller);

void monothek_jukebox_playlist_controller_real_select_song(MonothekJukeboxPlaylistController *jukebox_playlist_controller,
							   gchar *song_filename);

/**
 * SECTION:monothek_jukebox_playlist_controller
 * @short_description: The jukebox mode controller object.
 * @title: MonothekJukeboxPlaylistController
 * @section_id:
 * @include: monothek/ui/controller/monothek_jukebox_playlist_controller.h
 *
 * #MonothekJukeboxPlaylistController is the MVC's jukebox mode controller.
 */

enum{
  SELECT_SONG,
  LAST_SIGNAL,
};

static gpointer monothek_jukebox_playlist_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_jukebox_playlist_controller_parent_connectable_interface;

static guint jukebox_playlist_controller_signals[LAST_SIGNAL];

GType
monothek_jukebox_playlist_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_playlist_controller = 0;

    static const GTypeInfo monothek_jukebox_playlist_controller_info = {
      sizeof (MonothekJukeboxPlaylistControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_playlist_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxPlaylistController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_playlist_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_playlist_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_playlist_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
								       "MonothekJukeboxPlaylistController", &monothek_jukebox_playlist_controller_info,
								       0);

    g_type_add_interface_static(monothek_type_jukebox_playlist_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_playlist_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_playlist_controller_class_init(MonothekJukeboxPlaylistControllerClass *jukebox_playlist_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekControllerClass *controller;

  monothek_jukebox_playlist_controller_parent_class = g_type_class_peek_parent(jukebox_playlist_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_playlist_controller;

  gobject->finalize = monothek_jukebox_playlist_controller_finalize;

  /* MonothekControllerClass */
  controller = (MonothekControllerClass *) jukebox_playlist_controller;
  
  controller->reset = monothek_jukebox_playlist_controller_reset;

  /* MonothekJukeboxPlaylistController */
  jukebox_playlist_controller->select_song = monothek_jukebox_playlist_controller_real_select_song;
  
  /* signals */
  /**
   * MonothekJukeboxPlaylistController::select-song:
   * @jukebox_playlist_controller: the #MonothekJukeboxPlaylistController
   *
   * The ::select-song signal notifies about action box event.
   *
   * Since: 1.0.0
   */
  jukebox_playlist_controller_signals[SELECT_SONG] =
    g_signal_new("select-song",
		 G_TYPE_FROM_CLASS(jukebox_playlist_controller),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(MonothekJukeboxPlaylistControllerClass, select_song),
		 NULL, NULL,
		 g_cclosure_marshal_VOID__STRING,
		 G_TYPE_NONE, 1,
		 G_TYPE_STRING);
}

void
monothek_jukebox_playlist_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_jukebox_playlist_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_jukebox_playlist_controller_connect;
  connectable->disconnect = monothek_jukebox_playlist_controller_disconnect;
}

void
monothek_jukebox_playlist_controller_init(MonothekJukeboxPlaylistController *jukebox_playlist_controller)
{
  MonothekActionBox *action_box;

  guint i;
  
  jukebox_playlist_controller->song_select = (MonothekActionBox **) malloc(MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_SONG_ROW_COUNT * sizeof(MonothekActionBox *));

  for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_SONG_ROW_COUNT; i++){
    gchar *str;

    str = g_strdup_printf("jukebox-playlist-track-%d", i);
    
    jukebox_playlist_controller->song_select[i] =
      action_box = g_object_new(MONOTHEK_TYPE_ACTION_BOX,
				"action-identifier", str,
				"x0", 80,
				"y0", 80 + (i * (60 + 20)),
				"width", 1760,
				"height", 60,
				NULL);
    monothek_controller_add_action_box(jukebox_playlist_controller,
				       action_box);

    g_free(str);
  }
}

void
monothek_jukebox_playlist_controller_finalize(GObject *gobject)
{
  MonothekJukeboxPlaylistController *jukebox_playlist_controller;

  jukebox_playlist_controller = (MonothekJukeboxPlaylistController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_playlist_controller_parent_class)->finalize(gobject);
}

void
monothek_jukebox_playlist_controller_connect(AgsConnectable *connectable)
{
  MonothekJukeboxPlaylistController *jukebox_playlist_controller;

  guint i;
  
  jukebox_playlist_controller = MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER(connectable);

  if(monothek_controller_test_flags(jukebox_playlist_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_playlist_controller_parent_connectable_interface->connect(connectable);

  for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_SONG_ROW_COUNT; i++){
    g_signal_connect(jukebox_playlist_controller->song_select[i], "enter",
		     G_CALLBACK(monothek_jukebox_playlist_controller_jukebox_song_select_enter_callback), jukebox_playlist_controller);
    g_signal_connect(jukebox_playlist_controller->song_select[i], "leave",
		     G_CALLBACK(monothek_jukebox_playlist_controller_jukebox_song_select_leave_callback), jukebox_playlist_controller);
    g_signal_connect(jukebox_playlist_controller->song_select[i], "clicked",
		     G_CALLBACK(monothek_jukebox_playlist_controller_jukebox_song_select_clicked_callback), jukebox_playlist_controller);
  }
}

void
monothek_jukebox_playlist_controller_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxPlaylistController *jukebox_playlist_controller;

  guint i;

  jukebox_playlist_controller = MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(jukebox_playlist_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_jukebox_playlist_controller_parent_connectable_interface->disconnect(connectable);

  for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_CONTROLLER_SONG_ROW_COUNT; i++){
    g_object_disconnect(jukebox_playlist_controller->song_select[i],
			"any_signal::enter",
			G_CALLBACK(monothek_jukebox_playlist_controller_jukebox_song_select_enter_callback),
			jukebox_playlist_controller,
			"any_signal::leave",
			G_CALLBACK(monothek_jukebox_playlist_controller_jukebox_song_select_leave_callback),
			jukebox_playlist_controller,
			"any_signal::clicked",
			G_CALLBACK(monothek_jukebox_playlist_controller_jukebox_song_select_clicked_callback),
			jukebox_playlist_controller,
			NULL);
  }
}

void
monothek_jukebox_playlist_controller_reset(MonothekController *controller)
{
  MonothekJukeboxPlaylistModel *model;

  guint i;
  
  g_object_get(controller,
	       "model", &model,
	       NULL);

  for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_MODEL_SONG_ROW_COUNT; i++){
    model->song_select_active[i] = FALSE;
  }
}

void
monothek_jukebox_playlist_controller_jukebox_song_select_enter_callback(MonothekActionBox *action_box,
									MonothekJukeboxPlaylistController *jukebox_playlist_controller)
{
  MonothekJukeboxPlaylistView *view;
  
  MonothekJukeboxPlaylistModel *model;

  gint position;
  
  g_object_get(jukebox_playlist_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  position = -1;
  sscanf(action_box->action_identifier,
	 "jukebox-playlist-track-%d",
	 &position);
  
  if(position >= 0){
    model->song_select_active[position] = TRUE;
  }

  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_playlist_controller_jukebox_song_select_leave_callback(MonothekActionBox *action_box,
									MonothekJukeboxPlaylistController *jukebox_playlist_controller)
{
  MonothekJukeboxPlaylistView *view;
  
  MonothekJukeboxPlaylistModel *model;

  gint position;
  
  g_object_get(jukebox_playlist_controller,
	       "model", &model,
	       "view", &view,
	       NULL);

  position = -1;
  sscanf(action_box->action_identifier,
	 "jukebox-playlist-track-%d",
	 &position);
  
  if(position >= 0){
    model->song_select_active[position] = FALSE;
  }
  
  gtk_widget_queue_draw(view);
}

void
monothek_jukebox_playlist_controller_jukebox_song_select_clicked_callback(MonothekActionBox *action_box,
									  MonothekJukeboxPlaylistController *jukebox_playlist_controller)
{
  MonothekJukeboxPlaylistModel *model;

  gchar *song_filename;

  gint position;

  g_object_get(jukebox_playlist_controller,
	       "model", &model,
	       NULL);

  position = -1;
  sscanf(action_box->action_identifier,
	 "jukebox-playlist-track-%d",
	 &position);

  song_filename = NULL;

  if(position >= 0){
    song_filename = model->song_filename[position];
  }
  
  monothek_jukebox_playlist_controller_select_song(jukebox_playlist_controller,
						   song_filename);
}

void
monothek_jukebox_playlist_controller_real_select_song(MonothekJukeboxPlaylistController *jukebox_playlist_controller,
						      gchar *song_filename)
{
  MonothekWindow *window;
  MonothekJukeboxPlaylistView *view;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GValue *jukebox_song_filename;

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  /* set jukebox song filename */
  jukebox_song_filename = g_hash_table_lookup(session->value,
					      "jukebox-song-filename");

  g_value_set_string(jukebox_song_filename,
		     song_filename);
  
  /* change view */
  g_object_get(jukebox_playlist_controller,
	       "view", &view,
	       NULL);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  monothek_window_change_view(window,
			      MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW, G_TYPE_NONE);
}

/**
 * monothek_jukebox_playlist_controller_select_song:
 * @jukebox_playlist_controller: the #MonothekJukeboxPlaylistController
 * 
 * Select song.
 * 
 * Since: 1.0.0
 */
void
monothek_jukebox_playlist_controller_select_song(MonothekJukeboxPlaylistController *jukebox_playlist_controller,
						 gchar *song_filename)
{
  g_return_if_fail(MONOTHEK_IS_JUKEBOX_PLAYLIST_CONTROLLER(jukebox_playlist_controller));
  
  g_object_ref((GObject *) jukebox_playlist_controller);
  g_signal_emit(G_OBJECT(jukebox_playlist_controller),
		jukebox_playlist_controller_signals[SELECT_SONG], 0,
		song_filename);
  g_object_unref((GObject *) jukebox_playlist_controller);
}

/**
 * monothek_jukebox_playlist_controller_new:
 *
 * Creates an #MonothekJukeboxPlaylistController
 *
 * Returns: a new #MonothekJukeboxPlaylistController
 *
 * Since: 1.0.0
 */
MonothekJukeboxPlaylistController*
monothek_jukebox_playlist_controller_new()
{
  MonothekJukeboxPlaylistController *jukebox_playlist_controller;

  jukebox_playlist_controller = (MonothekJukeboxPlaylistController *) g_object_new(MONOTHEK_TYPE_JUKEBOX_PLAYLIST_CONTROLLER,
										   NULL);
  
  return(jukebox_playlist_controller);
}
