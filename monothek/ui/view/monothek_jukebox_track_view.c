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

#include <monothek/ui/view/monothek_jukebox_track_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_track_view_class_init(MonothekJukeboxTrackViewClass *jukebox_track_view);
void monothek_jukebox_track_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_track_view_init(MonothekJukeboxTrackView *jukebox_track_view);
void monothek_jukebox_track_view_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_track_view_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_track_view_finalize(GObject *gobject);

void monothek_jukebox_track_view_connect(AgsConnectable *connectable);
void monothek_jukebox_track_view_disconnect(AgsConnectable *connectable);

void monothek_jukebox_track_view_draw(MonothekView *view);

/**
 * SECTION:monothek_jukebox_track_view
 * @short_description: The jukebox_track view object.
 * @title: MonothekJukeboxTrackView
 * @section_id:
 * @include: monothek/ui/view/monothek_jukebox_track_view.h
 *
 * #MonothekJukeboxTrackView is the MVC's jukebox_track view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_track_view_parent_class = NULL;
static AgsConnectableInterface *monothek_jukebox_track_view_parent_connectable_interface;

GType
monothek_jukebox_track_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_track_view = 0;

    static const GTypeInfo monothek_jukebox_track_view_info = {
      sizeof (MonothekJukeboxTrackViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_track_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxTrackView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_track_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_track_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_track_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
							      "MonothekJukeboxTrackView", &monothek_jukebox_track_view_info,
							      0);
    
    g_type_add_interface_static(monothek_type_jukebox_track_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_track_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_track_view_class_init(MonothekJukeboxTrackViewClass *jukebox_track_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_jukebox_track_view_parent_class = g_type_class_peek_parent(jukebox_track_view);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_track_view;

  gobject->set_property = monothek_jukebox_track_view_set_property;
  gobject->get_property = monothek_jukebox_track_view_get_property;

  gobject->finalize = monothek_jukebox_track_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) jukebox_track_view;

  view->draw = monothek_jukebox_track_view_draw;
}

void
monothek_jukebox_track_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_jukebox_track_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_jukebox_track_view_connect;
  connectable->disconnect = monothek_jukebox_track_view_disconnect;
}

void
monothek_jukebox_track_view_init(MonothekJukeboxTrackView *jukebox_track_view)
{
  /* cover */
  jukebox_track_view->cover_surface = NULL;
  
  jukebox_track_view->cover_box_line_width = 5.0;

  jukebox_track_view->cover_box_x0 = 740.0;
  jukebox_track_view->cover_box_y0 = 160.0;

  jukebox_track_view->cover_box_width = 440.0;
  jukebox_track_view->cover_box_height = 440.0;

  /* progress */
  jukebox_track_view->progress_adjustment = gtk_adjustment_new(0.0, 0.0, 1.0, 1.0, 1.0, 1.0;
  
  jukebox_track_view->progress_box_line_width = 5.0;

  jukebox_track_view->progress_box_x0 = 440.0;
  jukebox_track_view->progress_box_y0 = 680.0;

  jukebox_track_view->progress_box_width = 1040.0;
  jukebox_track_view->progress_box_height = 20.0;
}

void
monothek_jukebox_track_view_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_track_view_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_track_view_finalize(GObject *gobject)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = (MonothekJukeboxTrackView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_track_view_parent_class)->finalize(gobject);
}

void
monothek_jukebox_track_view_connect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  GList *list, *list_jukebox_track;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_track_view)->flags)) != 0){
    return;
  }

  monothek_jukebox_track_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_track_view_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  GList *list, *list_jukebox_track;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_track_view)->flags)) == 0){
    return;
  }

  monothek_jukebox_track_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_track_view_draw(MonothekView *view)
{
  MonothekJukeboxTrackView *jukebox_track_view;
  
  cairo_t *cr;

  guint width, height;
  guint x_jukebox_track, y_jukebox_track;

  static const gdouble white_gc = 65535.0;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_jukebox_track_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_jukebox_track = 0;
  y_jukebox_track = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  //TODO:JK: implement me
  
  /* paint */
  cairo_pop_group_to_source(cr);
  cairo_paint(cr);

  cairo_surface_mark_dirty(cairo_get_target(cr));
  cairo_destroy(cr);

#ifndef __APPLE__
  pango_fc_font_map_cache_clear(pango_cairo_font_map_get_default());
#endif
}

/**
 * monothek_jukebox_track_view_new:
 *
 * Creates an #MonothekJukeboxTrackView
 *
 * Returns: a new #MonothekJukeboxTrackView
 *
 * Since: 1.0.0
 */
MonothekJukeboxTrackView*
monothek_jukebox_track_view_new()
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = (MonothekJukeboxTrackView *) g_object_new(MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW,
								 NULL);
  
  return(jukebox_track_view);
}
