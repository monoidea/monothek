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

#include <monothek/ui/view/monothek_load_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_load_view_class_init(MonothekLoadViewClass *load_view);
void monothek_load_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_load_view_init(MonothekLoadView *load_view);
void monothek_load_view_set_property(GObject *gobject,
				     guint prop_id,
				     const GValue *value,
				     GParamSpec *param_spec);
void monothek_load_view_get_property(GObject *gobject,
				     guint prop_id,
				     GValue *value,
				     GParamSpec *param_spec);
void monothek_load_view_finalize(GObject *gobject);

void monothek_load_view_connect(AgsConnectable *connectable);
void monothek_load_view_disconnect(AgsConnectable *connectable);

void monothek_load_view_draw(MonothekView *view);

/**
 * SECTION:monothek_load_view
 * @short_description: The load view object.
 * @title: MonothekLoadView
 * @section_id:
 * @include: monothek/ui/view/monothek_load_view.h
 *
 * #MonothekLoadView is the MVC's load view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_load_view_parent_class = NULL;
static AgsConnectableInterface *monothek_load_view_parent_connectable_interface;

GType
monothek_load_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_load_view = 0;

    static const GTypeInfo monothek_load_view_info = {
      sizeof (MonothekLoadViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_load_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekLoadView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_load_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_load_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_load_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
						     "MonothekLoadView", &monothek_load_view_info,
						     0);
    
    g_type_add_interface_static(monothek_type_load_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_load_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_load_view_class_init(MonothekLoadViewClass *load_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_load_view_parent_class = g_type_class_peek_parent(load_view);

  /* GObjectClass */
  gobject = (GObjectClass *) load_view;

  gobject->set_property = monothek_load_view_set_property;
  gobject->get_property = monothek_load_view_get_property;

  gobject->finalize = monothek_load_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) load_view;

  view->draw = monothek_load_view_draw;
}

void
monothek_load_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_load_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_load_view_connect;
  connectable->disconnect = monothek_load_view_disconnect;
}

void
monothek_load_view_init(MonothekLoadView *load_view)
{
  load_view->progress_x0 = 840;
  load_view->progress_y0 = 680;
  
  load_view->progress_width = 240;
  load_view->progress_height = 40;

  load_view->progress_position = 0;
}

void
monothek_load_view_set_property(GObject *gobject,
				guint prop_id,
				const GValue *value,
				GParamSpec *param_spec)
{
  MonothekLoadView *load_view;

  load_view = MONOTHEK_LOAD_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_load_view_get_property(GObject *gobject,
				guint prop_id,
				GValue *value,
				GParamSpec *param_spec)
{
  MonothekLoadView *load_view;

  load_view = MONOTHEK_LOAD_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_load_view_finalize(GObject *gobject)
{
  MonothekLoadView *load_view;

  load_view = (MonothekLoadView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_load_view_parent_class)->finalize(gobject);
}

void
monothek_load_view_connect(AgsConnectable *connectable)
{
  MonothekLoadView *load_view;

  GList *list, *list_load;

  load_view = MONOTHEK_LOAD_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(load_view)->flags)) != 0){
    return;
  }

  monothek_load_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_load_view_disconnect(AgsConnectable *connectable)
{
  MonothekLoadView *load_view;

  GList *list, *list_load;

  load_view = MONOTHEK_LOAD_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(load_view)->flags)) == 0){
    return;
  }

  monothek_load_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_load_view_draw(MonothekView *view)
{
  MonothekLoadView *load_view;
  
  cairo_t *cr;

  guint width, height;
  guint x_load, y_load;
  guint i;
  
  static const gdouble white_gc = 65535.0;

  load_view = MONOTHEK_LOAD_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_load_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_load = 0;
  y_load = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  cairo_set_source_rgb(cr,
		       1.0,
		       1.0,
		       1.0);

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *generic_font;
    
    static const guint font_size = 100;

    generic_font = g_strdup_printf("%s Bold", view->font);

    /* message */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "SYSTEM IS LOADING\nPLEASE BE PATIENT.\nEXPECTED LOADING TIME 5 MINUTES", -1);
    desc = pango_font_description_from_string(generic_font);
    pango_font_description_set_size(desc,
				    38 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 0.0 + ((1920.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 280.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* free font string */
    g_free(generic_font);
  }
  
  cairo_set_source_rgb(cr,
		       1.0,
		       1.0,
		       1.0);

  for(i = 0; i < 3; i++){
    cairo_rectangle(cr,
		    (double) load_view->progress_x0 + ((load_view->progress_position + (2 * i)) * load_view->progress_height), (double) load_view->progress_y0,
		    (double) load_view->progress_height, (double) load_view->progress_height);
    cairo_fill(cr);
  }

  if(load_view->progress_position == 3){
    load_view->progress_position = 0;
  }else{
    load_view->progress_position += 1;
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

/**
 * monothek_load_view_new:
 *
 * Creates an #MonothekLoadView
 *
 * Returns: a new #MonothekLoadView
 *
 * Since: 1.0.0
 */
MonothekLoadView*
monothek_load_view_new()
{
  MonothekLoadView *load_view;

  load_view = (MonothekLoadView *) g_object_new(MONOTHEK_TYPE_LOAD_VIEW,
						NULL);
  
  return(load_view);
}
