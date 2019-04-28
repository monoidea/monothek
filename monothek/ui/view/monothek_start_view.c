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

#include <monothek/ui/view/monothek_start_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/model/monothek_start_model.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_start_view_class_init(MonothekStartViewClass *start_view);
void monothek_start_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_start_view_init(MonothekStartView *start_view);
void monothek_start_view_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec);
void monothek_start_view_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec);
void monothek_start_view_finalize(GObject *gobject);

void monothek_start_view_connect(AgsConnectable *connectable);
void monothek_start_view_disconnect(AgsConnectable *connectable);

void monothek_start_view_draw(MonothekView *view);

void monothek_start_view_reset(MonothekView *view,
			       gboolean reset_defaults, gboolean reset_current);
void monothek_start_view_clear(MonothekView *view,
			       gboolean clear_all, gboolean clear_hover);

/**
 * SECTION:monothek_start_view
 * @short_description: The start view object.
 * @title: MonothekStartView
 * @section_id:
 * @include: monothek/ui/view/monothek_start_view.h
 *
 * #MonothekStartView is the MVC's start view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_start_view_parent_class = NULL;
static AgsConnectableInterface *monothek_start_view_parent_connectable_interface;

GType
monothek_start_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_start_view = 0;

    static const GTypeInfo monothek_start_view_info = {
      sizeof (MonothekStartViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_start_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekStartView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_start_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_start_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_start_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
						      "MonothekStartView", &monothek_start_view_info,
						      0);
    
    g_type_add_interface_static(monothek_type_start_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_start_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_start_view_class_init(MonothekStartViewClass *start_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_start_view_parent_class = g_type_class_peek_parent(start_view);

  /* GObjectClass */
  gobject = (GObjectClass *) start_view;

  gobject->set_property = monothek_start_view_set_property;
  gobject->get_property = monothek_start_view_get_property;

  gobject->finalize = monothek_start_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) start_view;

  view->draw = monothek_start_view_draw;

  view->reset = monothek_start_view_reset;
  view->clear = monothek_start_view_clear;
}

void
monothek_start_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_start_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_start_view_connect;
  connectable->disconnect = monothek_start_view_disconnect;
}

void
monothek_start_view_init(MonothekStartView *start_view)
{
  start_view->outer_box_line_width = 5.0;

  /* jukebox */
  start_view->jukebox_x0 = 80.0;
  start_view->jukebox_y0 = 80.0;

  start_view->jukebox_width = 840.0;
  start_view->jukebox_height = 920.0;

  start_view->jukebox_start_box_line_width = 5.0;

  start_view->jukebox_start_box_x0 = 320.0;
  start_view->jukebox_start_box_y0 = 600.0;

  start_view->jukebox_start_box_width = 360.0;
  start_view->jukebox_start_box_height = 120.0;
  
  /* diskjokey */
  start_view->diskjokey_x0 = 1000.0;
  start_view->diskjokey_y0 = 80.0;

  start_view->diskjokey_width = 840.0;
  start_view->diskjokey_height = 920.0;

  start_view->diskjokey_start_box_line_width = 5.0;

  start_view->diskjokey_start_box_x0 = 1240.0;
  start_view->diskjokey_start_box_y0 = 600.0;

  start_view->diskjokey_start_box_width = 360.0;
  start_view->diskjokey_start_box_height = 120.0;
}

void
monothek_start_view_set_property(GObject *gobject,
				 guint prop_id,
				 const GValue *value,
				 GParamSpec *param_spec)
{
  MonothekStartView *start_view;

  start_view = MONOTHEK_START_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_start_view_get_property(GObject *gobject,
				 guint prop_id,
				 GValue *value,
				 GParamSpec *param_spec)
{
  MonothekStartView *start_view;

  start_view = MONOTHEK_START_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_start_view_finalize(GObject *gobject)
{
  MonothekStartView *start_view;

  start_view = (MonothekStartView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_start_view_parent_class)->finalize(gobject);
}

void
monothek_start_view_connect(AgsConnectable *connectable)
{
  MonothekStartView *start_view;

  GList *list, *list_start;

  start_view = MONOTHEK_START_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(start_view)->flags)) != 0){
    return;
  }

  monothek_start_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_start_view_disconnect(AgsConnectable *connectable)
{
  MonothekStartView *start_view;

  GList *list, *list_start;

  start_view = MONOTHEK_START_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(start_view)->flags)) == 0){
    return;
  }

  monothek_start_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_start_view_draw(MonothekView *view)
{
  MonothekStartView *start_view;

  MonothekStartModel *start_model;
  
  cairo_t *cr;

  guint width, height;
  guint x_start, y_start;

  static const gdouble white_gc = 65535.0;

  start_view = MONOTHEK_START_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_start_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  g_object_get(view,
	       "model", &start_model,
	       NULL);
  
  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_start = 0;
  y_start = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  /* jukebox - draw box */
  cairo_set_source_rgb(cr,
		       1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
		       1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
		       1.0 / 255.0 * ((0xff & view->jukebox_gc)));
  cairo_set_line_width(cr,
		       start_view->outer_box_line_width);
  cairo_rectangle(cr,
		  (double) start_view->jukebox_x0, (double) start_view->jukebox_y0,
		  (double) start_view->jukebox_width, (double) start_view->jukebox_height);
  cairo_stroke(cr);

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    /* jukebox - head */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "JUKEBOX", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    96 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 80.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 228.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - pricing */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "5 CHF", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    60 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 80.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 438.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
    
    /* jukebox - start */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "START", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    38 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    
    if(start_model == NULL ||
       !start_model->jukebox_start_active){
      cairo_move_to(cr,
		    (double) 80.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		    (double) 639.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   start_view->jukebox_start_box_line_width);
      cairo_rectangle(cr,
		      (double) start_view->jukebox_start_box_x0, (double) start_view->jukebox_start_box_y0,
		      (double) start_view->jukebox_start_box_width, (double) start_view->jukebox_start_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   start_view->jukebox_start_box_line_width);
      cairo_rectangle(cr,
		      (double) start_view->jukebox_start_box_x0, (double) start_view->jukebox_start_box_y0,
		      (double) start_view->jukebox_start_box_width, (double) start_view->jukebox_start_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);
      
      cairo_move_to(cr,
		    (double) 80.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		    (double) 639.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }
    
    /* jukebox - description */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "CHOOSE A TITLE AND\nDANCE TO IT TILL IT ENDS", -1);
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
		  (double) 80.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 820.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    /* free font string */
    g_free(jukebox_font);
  }
  
  /* diskjokey - draw box */
  cairo_set_source_rgb(cr,
		       1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
		       1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
		       1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
  cairo_set_line_width(cr,
		       start_view->outer_box_line_width);
  cairo_rectangle(cr,
		  (double) start_view->diskjokey_x0, (double) start_view->diskjokey_y0,
		  (double) start_view->diskjokey_width, (double) start_view->diskjokey_height);
  cairo_stroke(cr);

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    /* diskjokey - head */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "DISKJOKEY", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    96 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 1000.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 228.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* diskjokey - pricing */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "5 CHF", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    60 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 1000.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 438.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
    
    /* diskjokey - start */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "START", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    38 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    
    if(start_model == NULL ||
       !start_model->diskjokey_start_active){
      cairo_move_to(cr,
		    (double) 1000.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		    (double) 639.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   start_view->diskjokey_start_box_line_width);
      cairo_rectangle(cr,
		      (double) start_view->diskjokey_start_box_x0, (double) start_view->diskjokey_start_box_y0,
		      (double) start_view->diskjokey_start_box_width, (double) start_view->diskjokey_start_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   start_view->diskjokey_start_box_line_width);
      cairo_rectangle(cr,
		      (double) start_view->diskjokey_start_box_x0, (double) start_view->diskjokey_start_box_y0,
		      (double) start_view->diskjokey_start_box_width, (double) start_view->diskjokey_start_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);
      
      cairo_move_to(cr,
		    (double) 1000.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		    (double) 639.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* diskjokey - description */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "BE THE DJ AND PLAY 10\nMINUTES FREESTYLE", -1);
    desc = pango_font_description_from_string(diskjokey_font);
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
		  (double) 1000.0 + ((840.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 820.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    /* free font string */
    g_free(diskjokey_font);
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
monothek_start_view_reset(MonothekView *view,
			  gboolean reset_defaults, gboolean reset_current)
{
  //TODO:JK: implement me
}

void
monothek_start_view_clear(MonothekView *view,
			  gboolean clear_all, gboolean clear_hover)
{
  MonothekStartModel *start_model;

  g_object_get(view,
	       "model", &start_model,
	       NULL);

  if(clear_all || clear_hover){
    start_model->jukebox_start_active = FALSE;
    start_model->diskjokey_start_active = FALSE;
  }
  
  //TODO:JK: implement me
}

/**
 * monothek_start_view_new:
 *
 * Creates an #MonothekStartView
 *
 * Returns: a new #MonothekStartView
 *
 * Since: 1.0.0
 */
MonothekStartView*
monothek_start_view_new()
{
  MonothekStartView *start_view;

  start_view = (MonothekStartView *) g_object_new(MONOTHEK_TYPE_START_VIEW,
						  NULL);
  
  return(start_view);
}
