/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2020 Joël Krähemann
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

#include <monothek/ui/view/monothek_outage_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_outage_view_class_init(MonothekOutageViewClass *outage_view);
void monothek_outage_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_outage_view_init(MonothekOutageView *outage_view);
void monothek_outage_view_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec);
void monothek_outage_view_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec);
void monothek_outage_view_finalize(GObject *gobject);

void monothek_outage_view_connect(AgsConnectable *connectable);
void monothek_outage_view_disconnect(AgsConnectable *connectable);

void monothek_outage_view_draw(MonothekView *view);

/**
 * SECTION:monothek_outage_view
 * @short_description: The outage view object.
 * @title: MonothekOutageView
 * @section_id:
 * @include: monothek/ui/view/monothek_outage_view.h
 *
 * #MonothekOutageView is the MVC's outage view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_outage_view_parent_class = NULL;
static AgsConnectableInterface *monothek_outage_view_parent_connectable_interface;

GType
monothek_outage_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_outage_view = 0;

    static const GTypeInfo monothek_outage_view_info = {
      sizeof (MonothekOutageViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_outage_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekOutageView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_outage_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_outage_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_outage_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
						       "MonothekOutageView", &monothek_outage_view_info,
						       0);
    
    g_type_add_interface_static(monothek_type_outage_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_outage_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_outage_view_class_init(MonothekOutageViewClass *outage_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_outage_view_parent_class = g_type_class_peek_parent(outage_view);

  /* GObjectClass */
  gobject = (GObjectClass *) outage_view;

  gobject->set_property = monothek_outage_view_set_property;
  gobject->get_property = monothek_outage_view_get_property;

  gobject->finalize = monothek_outage_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) outage_view;

  view->draw = monothek_outage_view_draw;
}

void
monothek_outage_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_outage_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_outage_view_connect;
  connectable->disconnect = monothek_outage_view_disconnect;
}

void
monothek_outage_view_init(MonothekOutageView *outage_view)
{
}

void
monothek_outage_view_set_property(GObject *gobject,
				  guint prop_id,
				  const GValue *value,
				  GParamSpec *param_spec)
{
  MonothekOutageView *outage_view;

  outage_view = MONOTHEK_OUTAGE_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_outage_view_get_property(GObject *gobject,
				  guint prop_id,
				  GValue *value,
				  GParamSpec *param_spec)
{
  MonothekOutageView *outage_view;

  outage_view = MONOTHEK_OUTAGE_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_outage_view_finalize(GObject *gobject)
{
  MonothekOutageView *outage_view;

  outage_view = (MonothekOutageView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_outage_view_parent_class)->finalize(gobject);
}

void
monothek_outage_view_connect(AgsConnectable *connectable)
{
  MonothekOutageView *outage_view;

  GList *list, *list_outage;

  outage_view = MONOTHEK_OUTAGE_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(outage_view)->flags)) != 0){
    return;
  }

  monothek_outage_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_outage_view_disconnect(AgsConnectable *connectable)
{
  MonothekOutageView *outage_view;

  GList *list, *list_outage;

  outage_view = MONOTHEK_OUTAGE_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(outage_view)->flags)) == 0){
    return;
  }

  monothek_outage_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_outage_view_draw(MonothekView *view)
{
  MonothekOutageView *outage_view;
  
  cairo_t *cr;

  static const gdouble white_gc = 65535.0;

  outage_view = MONOTHEK_OUTAGE_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_outage_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

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
    pango_layout_set_text(layout, "UNFORTUNATELY\nTHERE IS NO INTERNET\nCONNECTION", -1);
    desc = pango_font_description_from_string(generic_font);
    pango_font_description_set_size(desc,
				    80 * PANGO_SCALE);
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

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *generic_font;
    
    static const guint font_size = 100;

    generic_font = g_strdup_printf("%s Bold", view->font);

    /* notice */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "PLEASE COME BACK LATER", -1);
    desc = pango_font_description_from_string(generic_font);
    pango_font_description_set_size(desc,
				    38 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 0.0 + ((1920.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 844.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* free font string */
    g_free(generic_font);
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
 * monothek_outage_view_new:
 *
 * Creates an #MonothekOutageView
 *
 * Returns: a new #MonothekOutageView
 *
 * Since: 1.0.0
 */
MonothekOutageView*
monothek_outage_view_new()
{
  MonothekOutageView *outage_view;

  outage_view = (MonothekOutageView *) g_object_new(MONOTHEK_TYPE_OUTAGE_VIEW,
						    NULL);
  
  return(outage_view);
}
