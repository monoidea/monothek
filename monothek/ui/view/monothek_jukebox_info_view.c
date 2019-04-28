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

#include <monothek/ui/view/monothek_jukebox_info_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/model/monothek_jukebox_info_model.h>

void monothek_jukebox_info_view_class_init(MonothekJukeboxInfoViewClass *jukebox_info_view);
void monothek_jukebox_info_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_info_view_init(MonothekJukeboxInfoView *jukebox_info_view);
void monothek_jukebox_info_view_set_property(GObject *gobject,
					     guint prop_id,
					     const GValue *value,
					     GParamSpec *param_spec);
void monothek_jukebox_info_view_get_property(GObject *gobject,
					     guint prop_id,
					     GValue *value,
					     GParamSpec *param_spec);
void monothek_jukebox_info_view_finalize(GObject *gobject);

void monothek_jukebox_info_view_connect(AgsConnectable *connectable);
void monothek_jukebox_info_view_disconnect(AgsConnectable *connectable);

void monothek_jukebox_info_view_draw(MonothekView *view);

void monothek_jukebox_info_view_reset(MonothekView *view,
				      gboolean reset_defaults, gboolean reset_current);
void monothek_jukebox_info_view_clear(MonothekView *view,
				      gboolean clear_all, gboolean clear_hover);

/**
 * SECTION:monothek_jukebox_info_view
 * @short_description: The jukebox_info view object.
 * @title: MonothekJukeboxInfoView
 * @section_id:
 * @include: monothek/ui/view/monothek_jukebox_info_view.h
 *
 * #MonothekJukeboxInfoView is the MVC's jukebox_info view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_info_view_parent_class = NULL;
static AgsConnectableInterface *monothek_jukebox_info_view_parent_connectable_interface;

GType
monothek_jukebox_info_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_info_view = 0;

    static const GTypeInfo monothek_jukebox_info_view_info = {
      sizeof (MonothekJukeboxInfoViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_info_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxInfoView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_info_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_info_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_info_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
							     "MonothekJukeboxInfoView", &monothek_jukebox_info_view_info,
							     0);
    
    g_type_add_interface_static(monothek_type_jukebox_info_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_info_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_info_view_class_init(MonothekJukeboxInfoViewClass *jukebox_info_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_jukebox_info_view_parent_class = g_type_class_peek_parent(jukebox_info_view);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_info_view;

  gobject->set_property = monothek_jukebox_info_view_set_property;
  gobject->get_property = monothek_jukebox_info_view_get_property;

  gobject->finalize = monothek_jukebox_info_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) jukebox_info_view;

  view->draw = monothek_jukebox_info_view_draw;

  view->reset = monothek_jukebox_info_view_reset;
  view->clear = monothek_jukebox_info_view_clear;
}

void
monothek_jukebox_info_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_jukebox_info_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_jukebox_info_view_connect;
  connectable->disconnect = monothek_jukebox_info_view_disconnect;
}

void
monothek_jukebox_info_view_init(MonothekJukeboxInfoView *jukebox_info_view)
{
}

void
monothek_jukebox_info_view_set_property(GObject *gobject,
					guint prop_id,
					const GValue *value,
					GParamSpec *param_spec)
{
  MonothekJukeboxInfoView *jukebox_info_view;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_info_view_get_property(GObject *gobject,
					guint prop_id,
					GValue *value,
					GParamSpec *param_spec)
{
  MonothekJukeboxInfoView *jukebox_info_view;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_info_view_finalize(GObject *gobject)
{
  MonothekJukeboxInfoView *jukebox_info_view;

  jukebox_info_view = (MonothekJukeboxInfoView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_info_view_parent_class)->finalize(gobject);
}

void
monothek_jukebox_info_view_connect(AgsConnectable *connectable)
{
  MonothekJukeboxInfoView *jukebox_info_view;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_info_view)->flags)) != 0){
    return;
  }

  monothek_jukebox_info_view_parent_connectable_interface->connect(connectable);
}

void
monothek_jukebox_info_view_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxInfoView *jukebox_info_view;

  GList *list, *list_jukebox_info;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_info_view)->flags)) == 0){
    return;
  }

  monothek_jukebox_info_view_parent_connectable_interface->disconnect(connectable);
}

void
monothek_jukebox_info_view_draw(MonothekView *view)
{
  MonothekJukeboxInfoView *jukebox_info_view;

  cairo_t *cr;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  /* paint */
  cairo_pop_group_to_source(cr);
  cairo_paint(cr);

  cairo_surface_mark_dirty(cairo_get_target(cr));
  cairo_destroy(cr);

#ifndef __APPLE__
  //  pango_fc_font_map_cache_clear(pango_cairo_font_map_get_default());
#endif
}

void
monothek_jukebox_info_view_reset(MonothekView *view,
				 gboolean reset_defaults, gboolean reset_current)
{
  //TODO:JK: implement me
}
  
void
monothek_jukebox_info_view_clear(MonothekView *view,
				 gboolean clear_all, gboolean clear_hover)
{
  MonothekJukeboxInfoModel *jukebox_info_model;

  g_object_get(view,
	       "model", &jukebox_info_model,
	       NULL);

  if(clear_all || clear_hover){
    jukebox_info_model->jukebox_ok_active = FALSE;
  }
  
  //TODO:JK: implement me
}

/**
 * monothek_jukebox_info_view_new:
 *
 * Creates an #MonothekJukeboxInfoView
 *
 * Returns: a new #MonothekJukeboxInfoView
 *
 * Since: 1.0.0
 */
MonothekJukeboxInfoView*
monothek_jukebox_info_view_new()
{
  MonothekJukeboxInfoView *jukebox_info_view;

  jukebox_info_view = (MonothekJukeboxInfoView *) g_object_new(MONOTHEK_TYPE_JUKEBOX_INFO_VIEW,
							       NULL);
  
  return(jukebox_info_view);
}
