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

#include <monothek/ui/view/monothek_jukebox_mode_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/model/monothek_jukebox_mode_model.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_mode_view_class_init(MonothekJukeboxModeViewClass *jukebox_mode_view);
void monothek_jukebox_mode_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_mode_view_init(MonothekJukeboxModeView *jukebox_mode_view);
void monothek_jukebox_mode_view_set_property(GObject *gobject,
					     guint prop_id,
					     const GValue *value,
					     GParamSpec *param_spec);
void monothek_jukebox_mode_view_get_property(GObject *gobject,
					     guint prop_id,
					     GValue *value,
					     GParamSpec *param_spec);
void monothek_jukebox_mode_view_finalize(GObject *gobject);

void monothek_jukebox_mode_view_connect(AgsConnectable *connectable);
void monothek_jukebox_mode_view_disconnect(AgsConnectable *connectable);

void monothek_jukebox_mode_view_draw(MonothekView *view);

/**
 * SECTION:monothek_jukebox_mode_view
 * @short_description: The jukebox_mode view object.
 * @title: MonothekJukeboxModeView
 * @section_id:
 * @include: monothek/ui/view/monothek_jukebox_mode_view.h
 *
 * #MonothekJukeboxModeView is the MVC's jukebox_mode view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_mode_view_parent_class = NULL;
static AgsConnectableInterface *monothek_jukebox_mode_view_parent_connectable_interface;

GType
monothek_jukebox_mode_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_mode_view = 0;

    static const GTypeInfo monothek_jukebox_mode_view_info = {
      sizeof (MonothekJukeboxModeViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_mode_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxModeView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_mode_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_mode_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_mode_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
							     "MonothekJukeboxModeView", &monothek_jukebox_mode_view_info,
							     0);
    
    g_type_add_interface_static(monothek_type_jukebox_mode_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_mode_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_mode_view_class_init(MonothekJukeboxModeViewClass *jukebox_mode_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_jukebox_mode_view_parent_class = g_type_class_peek_parent(jukebox_mode_view);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_mode_view;

  gobject->set_property = monothek_jukebox_mode_view_set_property;
  gobject->get_property = monothek_jukebox_mode_view_get_property;

  gobject->finalize = monothek_jukebox_mode_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) jukebox_mode_view;

  view->draw = monothek_jukebox_mode_view_draw;
}

void
monothek_jukebox_mode_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_jukebox_mode_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_jukebox_mode_view_connect;
  connectable->disconnect = monothek_jukebox_mode_view_disconnect;
}

void
monothek_jukebox_mode_view_init(MonothekJukeboxModeView *jukebox_mode_view)
{  
  /* test */
  jukebox_mode_view->test_box_line_width = 5.0;

  jukebox_mode_view->test_box_x0 = 280.0;
  jukebox_mode_view->test_box_y0 = 580.0;

  jukebox_mode_view->test_box_width = 400.0;
  jukebox_mode_view->test_box_height = 140.0;

  /* play */
  jukebox_mode_view->play_box_line_width = 5.0;

  jukebox_mode_view->play_box_x0 = 760.0;
  jukebox_mode_view->play_box_y0 = 580.0;

  jukebox_mode_view->play_box_width = 400.0;
  jukebox_mode_view->play_box_height = 140.0;

  /* cancel */
  jukebox_mode_view->cancel_box_line_width = 5.0;

  jukebox_mode_view->cancel_box_x0 = 1240.0;
  jukebox_mode_view->cancel_box_y0 = 580.0;

  jukebox_mode_view->cancel_box_width = 400.0;
  jukebox_mode_view->cancel_box_height = 140.0;
}

void
monothek_jukebox_mode_view_set_property(GObject *gobject,
					guint prop_id,
					const GValue *value,
					GParamSpec *param_spec)
{
  MonothekJukeboxModeView *jukebox_mode_view;

  jukebox_mode_view = MONOTHEK_JUKEBOX_MODE_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_mode_view_get_property(GObject *gobject,
					guint prop_id,
					GValue *value,
					GParamSpec *param_spec)
{
  MonothekJukeboxModeView *jukebox_mode_view;

  jukebox_mode_view = MONOTHEK_JUKEBOX_MODE_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_mode_view_finalize(GObject *gobject)
{
  MonothekJukeboxModeView *jukebox_mode_view;

  jukebox_mode_view = (MonothekJukeboxModeView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_mode_view_parent_class)->finalize(gobject);
}

void
monothek_jukebox_mode_view_connect(AgsConnectable *connectable)
{
  MonothekJukeboxModeView *jukebox_mode_view;

  MonothekJukeboxModeModel *jukebox_mode_model;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  guint test_count;
  
  GValue *jukebox_test_count;

  jukebox_mode_view = MONOTHEK_JUKEBOX_MODE_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_mode_view)->flags)) != 0){
    return;
  }

  monothek_jukebox_mode_view_parent_connectable_interface->connect(connectable);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);
  
  /* get jukebox song filename */
  jukebox_test_count = g_hash_table_lookup(session->value,
					   "jukebox-test-count");

  test_count = 0;

  if(jukebox_test_count != NULL){
    test_count = g_value_get_uint(jukebox_test_count);
  }
  
  g_object_get(jukebox_mode_view,
	       "model", &jukebox_mode_model,
	       NULL);

  g_object_set(jukebox_mode_model,
	       "attempts", test_count,
	       NULL);
}

void
monothek_jukebox_mode_view_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxModeView *jukebox_mode_view;

  GList *list, *list_jukebox_mode;

  jukebox_mode_view = MONOTHEK_JUKEBOX_MODE_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_mode_view)->flags)) == 0){
    return;
  }

  monothek_jukebox_mode_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_jukebox_mode_view_draw(MonothekView *view)
{
  MonothekJukeboxModeView *jukebox_mode_view;

  MonothekJukeboxModeModel *jukebox_mode_model;
  
  cairo_t *cr;

  guint width, height;
  guint x_jukebox_mode, y_jukebox_mode;

  static const gdouble white_gc = 65535.0;

  jukebox_mode_view = MONOTHEK_JUKEBOX_MODE_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_jukebox_mode_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  g_object_get(view,
	       "model", &jukebox_mode_model,
	       NULL);
  
  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_jukebox_mode = 0;
  y_jukebox_mode = 0;

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
    pango_layout_set_text(layout, "YOU CAN TEST 3 OUT\nOF 3 TRACKS", -1);
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
  }
  
  {
    PangoLayout *layout, *attempts_layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    gchar *attempts;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);
    
    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_test_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }
    
    cairo_set_line_width(cr,
			 jukebox_mode_view->test_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_mode_view->test_box_x0, (double) jukebox_mode_view->test_box_y0,
		    (double) jukebox_mode_view->test_box_width, (double) jukebox_mode_view->test_box_height);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_test_active){
      cairo_fill(cr);
      
      cairo_set_source_rgb(cr,
			   0.,
			   0.0,
			   0.0);
    }else{
      cairo_stroke(cr);
    }

    /* test - attempts */
    if(jukebox_mode_model != NULL){
      attempts = g_strdup_printf("%d/%d", jukebox_mode_model->attempts, jukebox_mode_model->max_attempts);
    }else{
      attempts = g_strdup("0/0");
    }
    
    attempts_layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(attempts_layout, attempts, -1);
    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(attempts_layout, desc);
    pango_font_description_free(desc);

    cairo_move_to(cr,
		  (double) jukebox_mode_view->test_box_x0 + MONOTHEK_JUKEBOX_MODE_VIEW_PADDING_LEFT,
		  (double) jukebox_mode_view->test_box_y0 + MONOTHEK_JUKEBOX_MODE_VIEW_PADDING_TOP);

    pango_cairo_update_layout(cr, attempts_layout);
    pango_cairo_show_layout(cr, attempts_layout);

    g_object_unref(attempts_layout);

    g_free(attempts);
    
    /* test - label */    
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TEST", -1);
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
		  (double) jukebox_mode_view->test_box_x0 + ((jukebox_mode_view->test_box_width / 2.0) - (logical_rect.width / 2.0)),
		  (double) 630.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_test_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }
    
    /* free font string */
    g_free(jukebox_font);
  }

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_play_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }
    
    cairo_set_line_width(cr,
			 jukebox_mode_view->play_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_mode_view->play_box_x0, (double) jukebox_mode_view->play_box_y0,
		    (double) jukebox_mode_view->play_box_width, (double) jukebox_mode_view->play_box_height);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_play_active){
      cairo_fill(cr);
      
      cairo_set_source_rgb(cr,
			   0.,
			   0.0,
			   0.0);
    }else{
      cairo_stroke(cr);
    }

    /* play */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "PLAY", -1);
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
		  (double) jukebox_mode_view->play_box_x0 + ((jukebox_mode_view->play_box_width / 2.0) - (logical_rect.width / 2.0)),
		  (double) 630.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_play_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }

    /* free font string */
    g_free(jukebox_font);
  }

  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *jukebox_font;
    
    static const guint font_size = 100;

    jukebox_font = g_strdup_printf("%s Bold", view->font);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_cancel_active){
      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->jukebox_gc)));
    }

    cairo_set_line_width(cr,
			 jukebox_mode_view->cancel_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_mode_view->cancel_box_x0, (double) jukebox_mode_view->cancel_box_y0,
		    (double) jukebox_mode_view->cancel_box_width, (double) jukebox_mode_view->cancel_box_height);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_cancel_active){
      cairo_fill(cr);
      
      cairo_set_source_rgb(cr,
			   0.,
			   0.0,
			   0.0);
    }else{
      cairo_stroke(cr);
    }

    /* cancel */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "CANCEL", -1);
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
		  (double) jukebox_mode_view->cancel_box_x0 + ((jukebox_mode_view->cancel_box_width / 2.0) - (logical_rect.width / 2.0)),
		  (double) 630.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    if(jukebox_mode_model != NULL &&
       jukebox_mode_model->jukebox_cancel_active){
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
  pango_fc_font_map_cache_clear(pango_cairo_font_map_get_default());
#endif
}

/**
 * monothek_jukebox_mode_view_new:
 *
 * Creates an #MonothekJukeboxModeView
 *
 * Returns: a new #MonothekJukeboxModeView
 *
 * Since: 1.0.0
 */
MonothekJukeboxModeView*
monothek_jukebox_mode_view_new()
{
  MonothekJukeboxModeView *jukebox_mode_view;

  jukebox_mode_view = (MonothekJukeboxModeView *) g_object_new(MONOTHEK_TYPE_JUKEBOX_MODE_VIEW,
							       NULL);
  
  return(jukebox_mode_view);
}
