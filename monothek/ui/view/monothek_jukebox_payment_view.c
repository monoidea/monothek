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

#include <monothek/ui/view/monothek_jukebox_payment_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_payment_view_class_init(MonothekJukeboxPaymentViewClass *jukebox_payment_view);
void monothek_jukebox_payment_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_payment_view_init(MonothekJukeboxPaymentView *jukebox_payment_view);
void monothek_jukebox_payment_view_set_property(GObject *gobject,
						guint prop_id,
						const GValue *value,
						GParamSpec *param_spec);
void monothek_jukebox_payment_view_get_property(GObject *gobject,
						guint prop_id,
						GValue *value,
						GParamSpec *param_spec);
void monothek_jukebox_payment_view_finalize(GObject *gobject);

void monothek_jukebox_payment_view_connect(AgsConnectable *connectable);
void monothek_jukebox_payment_view_disconnect(AgsConnectable *connectable);

void monothek_jukebox_payment_view_draw(MonothekView *view);

/**
 * SECTION:monothek_jukebox_payment_view
 * @short_description: The jukebox_payment view object.
 * @title: MonothekJukeboxPaymentView
 * @section_id:
 * @include: monothek/ui/view/monothek_jukebox_payment_view.h
 *
 * #MonothekJukeboxPaymentView is the MVC's jukebox_payment view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_payment_view_parent_class = NULL;
static AgsConnectableInterface *monothek_jukebox_payment_view_parent_connectable_interface;

GType
monothek_jukebox_payment_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_payment_view = 0;

    static const GTypeInfo monothek_jukebox_payment_view_info = {
      sizeof (MonothekJukeboxPaymentViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_payment_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxPaymentView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_payment_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_payment_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_payment_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
								"MonothekJukeboxPaymentView", &monothek_jukebox_payment_view_info,
								0);
    
    g_type_add_interface_static(monothek_type_jukebox_payment_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_payment_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_payment_view_class_init(MonothekJukeboxPaymentViewClass *jukebox_payment_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_jukebox_payment_view_parent_class = g_type_class_peek_parent(jukebox_payment_view);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_payment_view;

  gobject->set_property = monothek_jukebox_payment_view_set_property;
  gobject->get_property = monothek_jukebox_payment_view_get_property;

  gobject->finalize = monothek_jukebox_payment_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) jukebox_payment_view;

  view->draw = monothek_jukebox_payment_view_draw;
}

void
monothek_jukebox_payment_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_jukebox_payment_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_jukebox_payment_view_connect;
  connectable->disconnect = monothek_jukebox_payment_view_disconnect;
}

void
monothek_jukebox_payment_view_init(MonothekJukeboxPaymentView *jukebox_payment_view)
{
  gchar *str;
  
  jukebox_payment_view->pay_box_line_width = 5.0;

  jukebox_payment_view->pay_box_x0 = 840.0;
  jukebox_payment_view->pay_box_y0 = 580.0;

  jukebox_payment_view->pay_box_width = 240.0;
  jukebox_payment_view->pay_box_height = 120.0;

  /* arrow */
  jukebox_payment_view->arrow_box_x0 = 840.0;
  jukebox_payment_view->arrow_box_y0 = 720.0;

  jukebox_payment_view->arrow_box_width = 240.0;
  jukebox_payment_view->arrow_box_height = 120.0;

  jukebox_payment_view->arrow_top_x0 = 720.0;
  jukebox_payment_view->arrow_top_y0 = 820.0;
  
  jukebox_payment_view->arrow_top_width = 480.0;
  jukebox_payment_view->arrow_top_height = 240.0;

  /* images - cards */
  jukebox_payment_view->cards_box_x0 = 100.0;
  jukebox_payment_view->cards_box_y0 = jukebox_payment_view->pay_box_y0;
  
  str = getenv("MONOTHEK_JUKEBOX_PAYMENT_VIEW_CARDS_PNG");

  if(str != NULL){
    jukebox_payment_view->cards_png_filename = g_strdup(str);
  }else{
    jukebox_payment_view->cards_png_filename = g_strdup(MONOTHEK_JUKEBOX_PAYMENT_VIEW_CARDS_PNG);
  }

  /* images - nfc */
  jukebox_payment_view->nfc_box_x0 = jukebox_payment_view->arrow_box_x0;
  jukebox_payment_view->nfc_box_y0 = jukebox_payment_view->arrow_top_y0;

  str = getenv("MONOTHEK_JUKEBOX_PAYMENT_VIEW_NFC_PNG");

  if(str != NULL){
    jukebox_payment_view->nfc_png_filename = g_strdup(str);
  }else{
    jukebox_payment_view->nfc_png_filename = g_strdup(MONOTHEK_JUKEBOX_PAYMENT_VIEW_NFC_PNG);
  }
}

void
monothek_jukebox_payment_view_set_property(GObject *gobject,
					   guint prop_id,
					   const GValue *value,
					   GParamSpec *param_spec)
{
  MonothekJukeboxPaymentView *jukebox_payment_view;

  jukebox_payment_view = MONOTHEK_JUKEBOX_PAYMENT_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_payment_view_get_property(GObject *gobject,
					   guint prop_id,
					   GValue *value,
					   GParamSpec *param_spec)
{
  MonothekJukeboxPaymentView *jukebox_payment_view;

  jukebox_payment_view = MONOTHEK_JUKEBOX_PAYMENT_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_payment_view_finalize(GObject *gobject)
{
  MonothekJukeboxPaymentView *jukebox_payment_view;

  jukebox_payment_view = (MonothekJukeboxPaymentView *) gobject;

  g_free(jukebox_payment_view->cards_png_filename);
  g_free(jukebox_payment_view->nfc_png_filename);
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_payment_view_parent_class)->finalize(gobject);
}

void
monothek_jukebox_payment_view_connect(AgsConnectable *connectable)
{
  MonothekJukeboxPaymentView *jukebox_payment_view;

  GList *list, *list_jukebox_payment;

  jukebox_payment_view = MONOTHEK_JUKEBOX_PAYMENT_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_payment_view)->flags)) != 0){
    return;
  }

  monothek_jukebox_payment_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_payment_view_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxPaymentView *jukebox_payment_view;

  GList *list, *list_jukebox_payment;

  jukebox_payment_view = MONOTHEK_JUKEBOX_PAYMENT_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_payment_view)->flags)) == 0){
    return;
  }

  monothek_jukebox_payment_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_payment_view_draw(MonothekView *view)
{
  MonothekJukeboxPaymentView *jukebox_payment_view;
  
  cairo_t *cr;

  guint width, height;
  guint x_jukebox_payment, y_jukebox_payment;

  static const gdouble white_gc = 65535.0;

  jukebox_payment_view = MONOTHEK_JUKEBOX_PAYMENT_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_jukebox_payment_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_jukebox_payment = 0;
  y_jukebox_payment = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  cairo_set_source_rgb(cr,
		       1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
		       1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
		       1.0 / 255.0 * ((0xff & view->jukebox_gc)));

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    /* message */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TO PAY, PLEASE KEEP YOUR NFC\nCAPABLE CREDIT CARD UNDER\nTHE ARRROW", -1);
    desc = pango_font_description_from_string(jukebox_font);
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
		  (double) 200.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* 5 CHF */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "5 CHF", -1);
    desc = pango_font_description_from_string(jukebox_font);
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
		  (double) 620.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    cairo_set_line_width(cr,
			 jukebox_payment_view->pay_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_payment_view->pay_box_x0, (double) jukebox_payment_view->pay_box_y0,
		    (double) jukebox_payment_view->pay_box_width, (double) jukebox_payment_view->pay_box_height);
    cairo_stroke(cr);

    /* arrow */
    cairo_rectangle(cr,
		    (double) jukebox_payment_view->arrow_box_x0, (double) jukebox_payment_view->arrow_box_y0,
		    (double) jukebox_payment_view->arrow_box_width, (double) jukebox_payment_view->arrow_box_height);
    cairo_fill(cr);

    cairo_move_to(cr,
		  (double) jukebox_payment_view->arrow_top_x0, jukebox_payment_view->arrow_top_y0);
    cairo_line_to(cr,
		  jukebox_payment_view->arrow_top_x0 + jukebox_payment_view->arrow_top_width, jukebox_payment_view->arrow_top_y0);
    cairo_line_to(cr,
		  jukebox_payment_view->arrow_top_x0 + (jukebox_payment_view->arrow_top_width / 2.0), jukebox_payment_view->arrow_top_y0 + jukebox_payment_view->arrow_top_height);
    cairo_close_path(cr);
    cairo_fill(cr);

    /* free font string */
    g_free(jukebox_font);
  }
  
  /* paint */
  cairo_pop_group_to_source(cr);
  cairo_paint(cr);

  /* images */
  if(jukebox_payment_view->cards_png_filename != NULL){
    cairo_surface_t *surface;

    surface = cairo_image_surface_create_from_png(jukebox_payment_view->cards_png_filename);
    cairo_surface_reference(surface);
    
    cairo_set_source_surface(cr,
			     surface,
			     (double) jukebox_payment_view->cards_box_x0, (double) jukebox_payment_view->cards_box_y0);

    cairo_paint(cr);

    cairo_surface_destroy(surface);
  }

  if(jukebox_payment_view->nfc_png_filename != NULL){
    cairo_surface_t *surface;

    surface = cairo_image_surface_create_from_png(jukebox_payment_view->nfc_png_filename);
    cairo_surface_reference(surface);
    
    cairo_set_source_surface(cr,
			     surface,
			     (double) jukebox_payment_view->nfc_box_x0, (double) jukebox_payment_view->nfc_box_y0);

    cairo_paint(cr);

    cairo_surface_destroy(surface);
  }

  cairo_surface_mark_dirty(cairo_get_target(cr));
  cairo_destroy(cr);

#ifndef __APPLE__
  //  pango_fc_font_map_cache_clear(pango_cairo_font_map_get_default());
#endif
}

/**
 * monothek_jukebox_payment_view_new:
 *
 * Creates an #MonothekJukeboxPaymentView
 *
 * Returns: a new #MonothekJukeboxPaymentView
 *
 * Since: 1.0.0
 */
MonothekJukeboxPaymentView*
monothek_jukebox_payment_view_new()
{
  MonothekJukeboxPaymentView *jukebox_payment_view;

  jukebox_payment_view = (MonothekJukeboxPaymentView *) g_object_new(MONOTHEK_TYPE_JUKEBOX_PAYMENT_VIEW,
								     NULL);
  
  return(jukebox_payment_view);
}
