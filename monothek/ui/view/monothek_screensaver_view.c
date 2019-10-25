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

#include <monothek/ui/view/monothek_screensaver_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_screensaver_view_class_init(MonothekScreensaverViewClass *screensaver_view);
void monothek_screensaver_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_screensaver_view_init(MonothekScreensaverView *screensaver_view);
void monothek_screensaver_view_set_property(GObject *gobject,
					    guint prop_id,
					    const GValue *value,
					    GParamSpec *param_spec);
void monothek_screensaver_view_get_property(GObject *gobject,
					    guint prop_id,
					    GValue *value,
					    GParamSpec *param_spec);
void monothek_screensaver_view_finalize(GObject *gobject);

void monothek_screensaver_view_connect(AgsConnectable *connectable);
void monothek_screensaver_view_disconnect(AgsConnectable *connectable);

void monothek_screensaver_view_draw(MonothekView *view);

/**
 * SECTION:monothek_screensaver_view
 * @short_description: The screensaver view object.
 * @title: MonothekScreensaverView
 * @section_id:
 * @include: monothek/ui/view/monothek_screensaver_view.h
 *
 * #MonothekScreensaverView is the MVC's screensaver view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_screensaver_view_parent_class = NULL;
static AgsConnectableInterface *monothek_screensaver_view_parent_connectable_interface;

GType
monothek_screensaver_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_screensaver_view = 0;

    static const GTypeInfo monothek_screensaver_view_info = {
      sizeof (MonothekScreensaverViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_screensaver_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekScreensaverView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_screensaver_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_screensaver_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_screensaver_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
							    "MonothekScreensaverView", &monothek_screensaver_view_info,
							    0);
    
    g_type_add_interface_static(monothek_type_screensaver_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_screensaver_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_screensaver_view_class_init(MonothekScreensaverViewClass *screensaver_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_screensaver_view_parent_class = g_type_class_peek_parent(screensaver_view);

  /* GObjectClass */
  gobject = (GObjectClass *) screensaver_view;

  gobject->set_property = monothek_screensaver_view_set_property;
  gobject->get_property = monothek_screensaver_view_get_property;

  gobject->finalize = monothek_screensaver_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) screensaver_view;

  view->draw = monothek_screensaver_view_draw;
}

void
monothek_screensaver_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_screensaver_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_screensaver_view_connect;
  connectable->disconnect = monothek_screensaver_view_disconnect;
}

void
monothek_screensaver_view_init(MonothekScreensaverView *screensaver_view)
{
}

void
monothek_screensaver_view_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec)
{
  MonothekScreensaverView *screensaver_view;

  screensaver_view = MONOTHEK_SCREENSAVER_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_screensaver_view_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec)
{
  MonothekScreensaverView *screensaver_view;

  screensaver_view = MONOTHEK_SCREENSAVER_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_screensaver_view_finalize(GObject *gobject)
{
  MonothekScreensaverView *screensaver_view;

  screensaver_view = (MonothekScreensaverView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_screensaver_view_parent_class)->finalize(gobject);
}

void
monothek_screensaver_view_connect(AgsConnectable *connectable)
{
  MonothekScreensaverView *screensaver_view;

  GList *list, *list_screensaver;

  screensaver_view = MONOTHEK_SCREENSAVER_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(screensaver_view)->flags)) != 0){
    return;
  }

  monothek_screensaver_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_screensaver_view_disconnect(AgsConnectable *connectable)
{
  MonothekScreensaverView *screensaver_view;

  GList *list, *list_screensaver;

  screensaver_view = MONOTHEK_SCREENSAVER_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(screensaver_view)->flags)) == 0){
    return;
  }

  monothek_screensaver_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_screensaver_view_draw(MonothekView *view)
{
  MonothekScreensaverView *screensaver_view;
  
  cairo_t *cr;

  static const gdouble white_gc = 65535.0;

  screensaver_view = MONOTHEK_SCREENSAVER_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_screensaver_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  cairo_surface_flush(cairo_get_target(cr));

  {
    cairo_surface_t *surface;

    surface = cairo_image_surface_create_from_png(MONOTHEK_SCREENSAVER_VIEW_PICTURE);
    cairo_surface_reference(surface);
    
    cairo_set_source_surface(cr,
			     surface,
			     0.0, 0.0);

    cairo_paint(cr);

    cairo_surface_destroy(surface);
  }

  cairo_push_group(cr);

  cairo_set_source_rgb(cr,
		       1.0,
		       1.0,
		       1.0);

  //TODO:JK implement me
  
  /* paint */
  cairo_pop_group_to_source(cr);
  cairo_paint(cr);

  cairo_surface_mark_dirty(cairo_get_target(cr));
  cairo_destroy(cr);

#ifndef __APPLE__
  //  pango_fc_font_map_cache_clear(pango_cairo_font_map_get_default());
#endif
}

/**
 * monothek_screensaver_view_new:
 *
 * Creates an #MonothekScreensaverView
 *
 * Returns: a new #MonothekScreensaverView
 *
 * Since: 1.0.0
 */
MonothekScreensaverView*
monothek_screensaver_view_new()
{
  MonothekScreensaverView *screensaver_view;

  screensaver_view = (MonothekScreensaverView *) g_object_new(MONOTHEK_TYPE_SCREENSAVER_VIEW,
							      NULL);
  
  return(screensaver_view);
}
