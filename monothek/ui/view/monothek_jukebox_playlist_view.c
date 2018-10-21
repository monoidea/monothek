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

#include <monothek/ui/view/monothek_jukebox_playlist_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/model/monothek_jukebox_playlist_model.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_playlist_view_class_init(MonothekJukeboxPlaylistViewClass *jukebox_playlist_view);
void monothek_jukebox_playlist_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_playlist_view_init(MonothekJukeboxPlaylistView *jukebox_playlist_view);
void monothek_jukebox_playlist_view_set_property(GObject *gobject,
						 guint prop_id,
						 const GValue *value,
						 GParamSpec *param_spec);
void monothek_jukebox_playlist_view_get_property(GObject *gobject,
						 guint prop_id,
						 GValue *value,
						 GParamSpec *param_spec);
void monothek_jukebox_playlist_view_finalize(GObject *gobject);

void monothek_jukebox_playlist_view_connect(AgsConnectable *connectable);
void monothek_jukebox_playlist_view_disconnect(AgsConnectable *connectable);

void monothek_jukebox_playlist_view_draw(MonothekView *view);

/**
 * SECTION:monothek_jukebox_playlist_view
 * @short_description: The jukebox_playlist view object.
 * @title: MonothekJukeboxPlaylistView
 * @section_id:
 * @include: monothek/ui/view/monothek_jukebox_playlist_view.h
 *
 * #MonothekJukeboxPlaylistView is the MVC's jukebox_playlist view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_playlist_view_parent_class = NULL;
static AgsConnectableInterface *monothek_jukebox_playlist_view_parent_connectable_interface;

GType
monothek_jukebox_playlist_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_playlist_view = 0;

    static const GTypeInfo monothek_jukebox_playlist_view_info = {
      sizeof (MonothekJukeboxPlaylistViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_playlist_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxPlaylistView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_playlist_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_playlist_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_playlist_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
								 "MonothekJukeboxPlaylistView", &monothek_jukebox_playlist_view_info,
								 0);
    
    g_type_add_interface_static(monothek_type_jukebox_playlist_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_playlist_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_playlist_view_class_init(MonothekJukeboxPlaylistViewClass *jukebox_playlist_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_jukebox_playlist_view_parent_class = g_type_class_peek_parent(jukebox_playlist_view);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_playlist_view;

  gobject->set_property = monothek_jukebox_playlist_view_set_property;
  gobject->get_property = monothek_jukebox_playlist_view_get_property;

  gobject->finalize = monothek_jukebox_playlist_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) jukebox_playlist_view;

  view->draw = monothek_jukebox_playlist_view_draw;
}

void
monothek_jukebox_playlist_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_jukebox_playlist_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_jukebox_playlist_view_connect;
  connectable->disconnect = monothek_jukebox_playlist_view_disconnect;
}

void
monothek_jukebox_playlist_view_init(MonothekJukeboxPlaylistView *jukebox_playlist_view)
{
  jukebox_playlist_view->title_x0 = 100.0;
  jukebox_playlist_view->artist_x0 = 600.0;
  jukebox_playlist_view->album_x0 = 1100.0;
  jukebox_playlist_view->duration_x0 = 1720.0;

  jukebox_playlist_view->song_box_line_width = 5.0;
  
  jukebox_playlist_view->table_x0 = 80.0;
  jukebox_playlist_view->table_y0 = 80.0;

  jukebox_playlist_view->row_width = 1760.0;
  jukebox_playlist_view->row_height = 60.0;

  jukebox_playlist_view->row_spacing = 20.0;
  jukebox_playlist_view->text_spacing = 20.0;
}

void
monothek_jukebox_playlist_view_set_property(GObject *gobject,
					    guint prop_id,
					    const GValue *value,
					    GParamSpec *param_spec)
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;

  jukebox_playlist_view = MONOTHEK_JUKEBOX_PLAYLIST_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_playlist_view_get_property(GObject *gobject,
					    guint prop_id,
					    GValue *value,
					    GParamSpec *param_spec)
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;

  jukebox_playlist_view = MONOTHEK_JUKEBOX_PLAYLIST_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_playlist_view_finalize(GObject *gobject)
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;

  jukebox_playlist_view = (MonothekJukeboxPlaylistView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_playlist_view_parent_class)->finalize(gobject);
}

void
monothek_jukebox_playlist_view_connect(AgsConnectable *connectable)
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;

  GList *list, *list_jukebox_playlist;

  jukebox_playlist_view = MONOTHEK_JUKEBOX_PLAYLIST_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_playlist_view)->flags)) != 0){
    return;
  }

  monothek_jukebox_playlist_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_playlist_view_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;

  GList *list, *list_jukebox_playlist;

  jukebox_playlist_view = MONOTHEK_JUKEBOX_PLAYLIST_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_playlist_view)->flags)) == 0){
    return;
  }

  monothek_jukebox_playlist_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_playlist_view_draw(MonothekView *view)
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;
  
  MonothekJukeboxPlaylistModel *jukebox_playlist_model;

  cairo_t *cr;

  guint width, height;
  guint x_jukebox_playlist, y_jukebox_playlist;
  guint i;
  
  static const gdouble white_gc = 65535.0;

  jukebox_playlist_view = MONOTHEK_JUKEBOX_PLAYLIST_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_jukebox_playlist_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  g_object_get(view,
	       "model", &jukebox_playlist_model,
	       NULL);

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_jukebox_playlist = 0;
  y_jukebox_playlist = 0;

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

    /* jukebox - titel */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TITEL", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->title_x0,
		  (double) 40.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - artist */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "KÜNSTLER", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->artist_x0,
		  (double) 40.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - album */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "ALBUM", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->album_x0,
		  (double) 40.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - duration */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "LÄNGE", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->duration_x0,
		  (double) 40.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    g_free(jukebox_font);
  }
  
  for(i = 0; i < MONOTHEK_JUKEBOX_PLAYLIST_VIEW_SONG_ROW_COUNT; i++){
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;

    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    if(jukebox_playlist_model != NULL &&
       jukebox_playlist_model->song_select_active[i]){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }

    /* jukebox - song row */
    cairo_set_line_width(cr,
			 jukebox_playlist_view->song_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_playlist_view->table_x0, (double) jukebox_playlist_view->table_y0 + (i * (jukebox_playlist_view->row_height + jukebox_playlist_view->row_spacing)),
		    (double) jukebox_playlist_view->row_width, (double) jukebox_playlist_view->row_height);

    if(jukebox_playlist_model != NULL &&
       jukebox_playlist_model->song_select_active[i]){
      cairo_fill(cr);
      
      cairo_set_source_rgb(cr,
			   0.,
			   0.0,
			   0.0);
    }else{
      cairo_stroke(cr);
    }

    /* jukebox - titel */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "(null)", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->title_x0,
		  (double) jukebox_playlist_view->table_y0 + (i * (jukebox_playlist_view->row_height + jukebox_playlist_view->row_spacing)) + jukebox_playlist_view->text_spacing);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - artist */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "(null)", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->artist_x0,
		  (double) jukebox_playlist_view->table_y0 + (i * (jukebox_playlist_view->row_height + jukebox_playlist_view->row_spacing)) + jukebox_playlist_view->text_spacing);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - album */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "(null)", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->album_x0,
		  (double) jukebox_playlist_view->table_y0 + (i * (jukebox_playlist_view->row_height + jukebox_playlist_view->row_spacing)) + jukebox_playlist_view->text_spacing);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - duration */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "0:00", -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    18 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) jukebox_playlist_view->duration_x0,
		  (double) jukebox_playlist_view->table_y0 + (i * (jukebox_playlist_view->row_height + jukebox_playlist_view->row_spacing)) + jukebox_playlist_view->text_spacing);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    if(jukebox_playlist_model != NULL &&
       jukebox_playlist_model->song_select_active[i]){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }
    
    g_free(jukebox_font);
  }
  
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
 * monothek_jukebox_playlist_view_new:
 *
 * Creates an #MonothekJukeboxPlaylistView
 *
 * Returns: a new #MonothekJukeboxPlaylistView
 *
 * Since: 1.0.0
 */
MonothekJukeboxPlaylistView*
monothek_jukebox_playlist_view_new()
{
  MonothekJukeboxPlaylistView *jukebox_playlist_view;

  jukebox_playlist_view = (MonothekJukeboxPlaylistView *) g_object_new(MONOTHEK_TYPE_JUKEBOX_PLAYLIST_VIEW,
								       NULL);
  
  return(jukebox_playlist_view);
}
