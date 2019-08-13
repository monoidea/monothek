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
  /* ok */
  jukebox_info_view->ok_box_line_width = 5.0;

  jukebox_info_view->ok_box_x0 = 800.0;
  jukebox_info_view->ok_box_y0 = 760.0;

  jukebox_info_view->ok_box_width = 320.0;
  jukebox_info_view->ok_box_height = 140.0;
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

  MonothekJukeboxInfoModel *jukebox_info_model;
  
  cairo_t *cr;

  guint width, height;
  guint x_jukebox_info, y_jukebox_info;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(view);

  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_jukebox_info_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  g_object_get(view,
	       "model", &jukebox_info_model,
	       NULL);

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_jukebox_info = 0;
  y_jukebox_info = 0;

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
    pango_layout_set_text(layout, "YOU PLAYED FOLLOWING TRACK:", -1);
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
		  (double) 230.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
  }
  
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    /* artist */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, jukebox_info_model->artist, -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    78 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 0.0 + ((1920.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 350.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* title */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, jukebox_info_model->song_title, -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    78 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 0.0 + ((1920.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 510.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
  }

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    if(jukebox_info_model != NULL &&
       jukebox_info_model->jukebox_ok_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }
    
    cairo_set_line_width(cr,
			 jukebox_info_view->ok_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_info_view->ok_box_x0, (double) jukebox_info_view->ok_box_y0,
		    (double) jukebox_info_view->ok_box_width, (double) jukebox_info_view->ok_box_height);

    if(jukebox_info_model != NULL &&
       jukebox_info_model->jukebox_ok_active){
      cairo_fill(cr);
      
      cairo_set_source_rgb(cr,
			   0.,
			   0.0,
			   0.0);
    }else{
      cairo_stroke(cr);
    }

    /* ok */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "OK", -1);
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
		  (double) jukebox_info_view->ok_box_x0 + ((jukebox_info_view->ok_box_width / 2.0) - (logical_rect.width / 2.0)),
		  (double) 812.5);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    if(jukebox_info_model != NULL &&
       jukebox_info_model->jukebox_ok_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }

    /* free font string */
    g_free(jukebox_font);
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
  MonothekJukeboxInfoView *jukebox_info_view;

  MonothekJukeboxInfoModel *jukebox_info_model;

  MonothekSessionManager *session_manager;
  MonothekSession *session;
  
  gchar *song_filename;

  GValue *jukebox_song_filename;

  jukebox_info_view = MONOTHEK_JUKEBOX_INFO_VIEW(view);

  monothek_view_clear(view,
		      TRUE, TRUE);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);

  g_object_get(view,
	       "model", &jukebox_info_model,
	       NULL);

  /* get jukebox song filename */
  jukebox_song_filename = g_hash_table_lookup(session->value,
					      "jukebox-song-filename");

  song_filename = g_value_get_string(jukebox_song_filename);

  /* load song filename */
  monothek_jukebox_info_model_load_song_filename(MONOTHEK_VIEW(jukebox_info_view)->model,
						 song_filename);

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
