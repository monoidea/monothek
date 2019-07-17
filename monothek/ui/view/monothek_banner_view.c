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

#include <monothek/ui/view/monothek_banner_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_banner_view_class_init(MonothekBannerViewClass *banner_view);
void monothek_banner_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_banner_view_init(MonothekBannerView *banner_view);
void monothek_banner_view_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec);
void monothek_banner_view_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec);
void monothek_banner_view_finalize(GObject *gobject);

void monothek_banner_view_connect(AgsConnectable *connectable);
void monothek_banner_view_disconnect(AgsConnectable *connectable);

void monothek_banner_view_draw(MonothekView *view);

/**
 * SECTION:monothek_banner_view
 * @short_description: The banner view object.
 * @title: MonothekBannerView
 * @section_id:
 * @include: monothek/ui/view/monothek_banner_view.h
 *
 * #MonothekBannerView is the MVC's banner view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_banner_view_parent_class = NULL;
static AgsConnectableInterface *monothek_banner_view_parent_connectable_interface;

GType
monothek_banner_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_banner_view = 0;

    static const GTypeInfo monothek_banner_view_info = {
      sizeof (MonothekBannerViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_banner_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekBannerView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_banner_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_banner_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_banner_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
						       "MonothekBannerView", &monothek_banner_view_info,
						       0);
    
    g_type_add_interface_static(monothek_type_banner_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_banner_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_banner_view_class_init(MonothekBannerViewClass *banner_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_banner_view_parent_class = g_type_class_peek_parent(banner_view);

  /* GObjectClass */
  gobject = (GObjectClass *) banner_view;

  gobject->set_property = monothek_banner_view_set_property;
  gobject->get_property = monothek_banner_view_get_property;

  gobject->finalize = monothek_banner_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) banner_view;

  view->draw = monothek_banner_view_draw;
}

void
monothek_banner_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_banner_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_banner_view_connect;
  connectable->disconnect = monothek_banner_view_disconnect;
}

void
monothek_banner_view_init(MonothekBannerView *banner_view)
{
}

void
monothek_banner_view_set_property(GObject *gobject,
				  guint prop_id,
				  const GValue *value,
				  GParamSpec *param_spec)
{
  MonothekBannerView *banner_view;

  banner_view = MONOTHEK_BANNER_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_banner_view_get_property(GObject *gobject,
				  guint prop_id,
				  GValue *value,
				  GParamSpec *param_spec)
{
  MonothekBannerView *banner_view;

  banner_view = MONOTHEK_BANNER_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_banner_view_finalize(GObject *gobject)
{
  MonothekBannerView *banner_view;

  banner_view = (MonothekBannerView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_banner_view_parent_class)->finalize(gobject);
}

void
monothek_banner_view_connect(AgsConnectable *connectable)
{
  MonothekBannerView *banner_view;

  GList *list, *list_banner;

  banner_view = MONOTHEK_BANNER_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(banner_view)->flags)) != 0){
    return;
  }

  monothek_banner_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_banner_view_disconnect(AgsConnectable *connectable)
{
  MonothekBannerView *banner_view;

  GList *list, *list_banner;

  banner_view = MONOTHEK_BANNER_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(banner_view)->flags)) == 0){
    return;
  }

  monothek_banner_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_banner_view_draw(MonothekView *view)
{
  MonothekBannerView *banner_view;
  
  cairo_t *cr;

  static const gdouble white_gc = 65535.0;

  banner_view = MONOTHEK_BANNER_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_banner_view_parent_class)->draw(view);

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
    pango_layout_set_text(layout, "TOUCH ME", -1);
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
    pango_layout_set_text(layout, "WARNING: CABINE IS VIDEO MONITORED", -1);
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
 * monothek_banner_view_new:
 *
 * Creates an #MonothekBannerView
 *
 * Returns: a new #MonothekBannerView
 *
 * Since: 1.0.0
 */
MonothekBannerView*
monothek_banner_view_new()
{
  MonothekBannerView *banner_view;

  banner_view = (MonothekBannerView *) g_object_new(MONOTHEK_TYPE_BANNER_VIEW,
						    NULL);
  
  return(banner_view);
}
