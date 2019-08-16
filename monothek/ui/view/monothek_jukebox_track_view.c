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

#include <monothek/ui/view/monothek_jukebox_track_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_jukebox_track_model.h>

#include <monothek/ui/controller/monothek_jukebox_track_controller.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_jukebox_track_view_class_init(MonothekJukeboxTrackViewClass *jukebox_track_view);
void monothek_jukebox_track_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_jukebox_track_view_init(MonothekJukeboxTrackView *jukebox_track_view);
void monothek_jukebox_track_view_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_track_view_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec);
void monothek_jukebox_track_view_finalize(GObject *gobject);

void monothek_jukebox_track_view_connect(AgsConnectable *connectable);
void monothek_jukebox_track_view_disconnect(AgsConnectable *connectable);

void monothek_jukebox_track_view_progress_callback(GtkAdjustment *adjustment,
						   MonothekJukeboxTrackView *jukebox_track_view);

void monothek_jukebox_track_view_draw(MonothekView *view);

void monothek_jukebox_track_view_reset(MonothekView *view,
				       gboolean reset_defaults, gboolean reset_current);
void monothek_jukebox_track_view_clear(MonothekView *view,
				       gboolean clear_all, gboolean clear_hover);

/**
 * SECTION:monothek_jukebox_track_view
 * @short_description: The jukebox_track view object.
 * @title: MonothekJukeboxTrackView
 * @section_id:
 * @include: monothek/ui/view/monothek_jukebox_track_view.h
 *
 * #MonothekJukeboxTrackView is the MVC's jukebox_track view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_jukebox_track_view_parent_class = NULL;
static AgsConnectableInterface *monothek_jukebox_track_view_parent_connectable_interface;

GType
monothek_jukebox_track_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_jukebox_track_view = 0;

    static const GTypeInfo monothek_jukebox_track_view_info = {
      sizeof (MonothekJukeboxTrackViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_jukebox_track_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekJukeboxTrackView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_jukebox_track_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_jukebox_track_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_jukebox_track_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
							      "MonothekJukeboxTrackView", &monothek_jukebox_track_view_info,
							      0);
    
    g_type_add_interface_static(monothek_type_jukebox_track_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_jukebox_track_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_jukebox_track_view_class_init(MonothekJukeboxTrackViewClass *jukebox_track_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_jukebox_track_view_parent_class = g_type_class_peek_parent(jukebox_track_view);

  /* GObjectClass */
  gobject = (GObjectClass *) jukebox_track_view;

  gobject->set_property = monothek_jukebox_track_view_set_property;
  gobject->get_property = monothek_jukebox_track_view_get_property;

  gobject->finalize = monothek_jukebox_track_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) jukebox_track_view;

  view->draw = monothek_jukebox_track_view_draw;

  view->reset = monothek_jukebox_track_view_reset;
  view->clear = monothek_jukebox_track_view_clear;
}

void
monothek_jukebox_track_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_jukebox_track_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_jukebox_track_view_connect;
  connectable->disconnect = monothek_jukebox_track_view_disconnect;
}

void
monothek_jukebox_track_view_init(MonothekJukeboxTrackView *jukebox_track_view)
{
  jukebox_track_view->flags = MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_CONTROLS;
  
  /* cover */
  jukebox_track_view->cover_surface = NULL;
  
  jukebox_track_view->cover_box_line_width = 5.0;

  jukebox_track_view->cover_box_x0 = 740.0;
  jukebox_track_view->cover_box_y0 = 160.0;

  jukebox_track_view->cover_box_width = 440.0;
  jukebox_track_view->cover_box_height = 440.0;

  /* progress */
  jukebox_track_view->progress = gtk_adjustment_new(0.0, 0.0, 1.0, 0.0001, 0.01, 0.01);
  
  jukebox_track_view->progress_box_line_width = 5.0;

  jukebox_track_view->progress_box_x0 = 440.0;
  jukebox_track_view->progress_box_y0 = 900.0;

  jukebox_track_view->progress_box_width = 1040.0;
  jukebox_track_view->progress_box_height = 20.0;

  /* play */
  jukebox_track_view->play_box_line_width = 5.0;

  jukebox_track_view->play_box_x0 = 120.0;
  jukebox_track_view->play_box_y0 = 839.0;

  jukebox_track_view->play_box_width = 520.0;
  jukebox_track_view->play_box_height = 140.0;

  /* test */
  jukebox_track_view->test_box_line_width = 5.0;

  jukebox_track_view->test_box_x0 = 700.0;
  jukebox_track_view->test_box_y0 = 839.0;

  jukebox_track_view->test_box_width = 520.0;
  jukebox_track_view->test_box_height = 140.0;

  /* back */
  jukebox_track_view->back_box_line_width = 5.0;

  jukebox_track_view->back_box_x0 = 1280.0;
  jukebox_track_view->back_box_y0 = 839.0;

  jukebox_track_view->back_box_width = 520.0;
  jukebox_track_view->back_box_height = 140.0;
}

void
monothek_jukebox_track_view_set_property(GObject *gobject,
					 guint prop_id,
					 const GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_track_view_get_property(GObject *gobject,
					 guint prop_id,
					 GValue *value,
					 GParamSpec *param_spec)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_jukebox_track_view_finalize(GObject *gobject)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = (MonothekJukeboxTrackView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_jukebox_track_view_parent_class)->finalize(gobject);
}

void
monothek_jukebox_track_view_connect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_track_view)->flags)) != 0){
    return;
  }

  monothek_jukebox_track_view_parent_connectable_interface->connect(connectable);

  g_signal_connect(jukebox_track_view->progress, "value-changed",
		   G_CALLBACK(monothek_jukebox_track_view_progress_callback), jukebox_track_view);
}

void
monothek_jukebox_track_view_disconnect(AgsConnectable *connectable)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  GList *list, *list_jukebox_track;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(jukebox_track_view)->flags)) == 0){
    return;
  }

  monothek_jukebox_track_view_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(jukebox_track_view->progress,
		      "any_signal::value-changed",
		      G_CALLBACK(monothek_jukebox_track_view_progress_callback),
		      jukebox_track_view,
		      NULL);
}

void
monothek_jukebox_track_view_progress_callback(GtkAdjustment *adjustment,
					      MonothekJukeboxTrackView *jukebox_track_view)
{
  gtk_widget_queue_draw(jukebox_track_view);
}

void
monothek_jukebox_track_view_draw(MonothekView *view)
{
  MonothekJukeboxTrackView *jukebox_track_view;
  
  MonothekJukeboxTrackModel *jukebox_track_model;

  cairo_t *cr;

  guint width, height;
  guint x_jukebox_track, y_jukebox_track;

  static const gdouble white_gc = 65535.0;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_jukebox_track_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  g_object_get(view,
	       "model", &jukebox_track_model,
	       NULL);

  cairo_surface_flush(cairo_get_target(cr));

  if(jukebox_track_model != NULL &&
     jukebox_track_model->cover_filename != NULL){
    cairo_surface_t *surface;

    surface = cairo_image_surface_create_from_png(jukebox_track_model->cover_filename);
    cairo_surface_reference(surface);
    
    cairo_set_source_surface(cr,
			     surface,
			     (double) jukebox_track_view->cover_box_x0, (double) jukebox_track_view->cover_box_y0);

    cairo_paint(cr);

    cairo_surface_destroy(surface);
  }

  cairo_push_group(cr);

  x_jukebox_track = 0;
  y_jukebox_track = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  cairo_set_source_rgb(cr,
		       1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
		       1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
		       1.0 / 255.0 * ((0xff & view->jukebox_gc)));

  /* jukebox - cover box */
  cairo_set_line_width(cr,
		       jukebox_track_view->cover_box_line_width);
  cairo_rectangle(cr,
		  (double) jukebox_track_view->cover_box_x0, (double) jukebox_track_view->cover_box_y0,
		  (double) jukebox_track_view->cover_box_width, (double) jukebox_track_view->cover_box_height);
  cairo_stroke(cr);
  
  /* jukebox - progress box */
  if((MONOTHEK_JUKEBOX_TRACK_VIEW_PLAYBACK_CONTROLS & (jukebox_track_view->flags)) != 0){
    cairo_set_line_width(cr,
			 jukebox_track_view->progress_box_line_width);
    cairo_rectangle(cr,
		    (double) jukebox_track_view->progress_box_x0, (double) jukebox_track_view->progress_box_y0,
		    (double) jukebox_track_view->progress_box_width, (double) jukebox_track_view->progress_box_height);
    cairo_stroke(cr);

    {
      gdouble value;

      value = jukebox_track_view->progress->value;

      if(value > 0.0){
	cairo_rectangle(cr,
			(double) jukebox_track_view->progress_box_x0, (double) jukebox_track_view->progress_box_y0,
			(double) value * jukebox_track_view->progress_box_width, (double) jukebox_track_view->progress_box_height);
	cairo_fill(cr);
      }
    }
  }

  /* buttons */
  if((MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_CONTROLS & (jukebox_track_view->flags)) != 0){
    if((MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_NO_TEST_CONTROL & (jukebox_track_view->flags)) == 0){
      PangoLayout *layout, *attempts_layout;
      PangoFontDescription *desc;

      PangoRectangle ink_rect;
      PangoRectangle logical_rect;
    
      gchar *jukebox_font;
      gchar *attempts;
    
      static const guint font_size = 100;

      jukebox_font = g_strdup_printf("%s Bold", view->font);
    
      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_test_active){
	cairo_set_source_rgb(cr,
			     1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			     1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			     1.0 / 255.0 * ((0xff & view->jukebox_gc)));
      }
    
      cairo_set_line_width(cr,
			   jukebox_track_view->test_box_line_width);
      cairo_rectangle(cr,
		      (double) jukebox_track_view->test_box_x0, (double) jukebox_track_view->test_box_y0,
		      (double) jukebox_track_view->test_box_width, (double) jukebox_track_view->test_box_height);

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_test_active){
	cairo_fill(cr);
      
	cairo_set_source_rgb(cr,
			     0.,
			     0.0,
			     0.0);
      }else{
	cairo_stroke(cr);
      }

      /* test - attempts */
      if(jukebox_track_model != NULL){
	attempts = g_strdup_printf("%d/%d", jukebox_track_model->max_attempts - jukebox_track_model->attempts, jukebox_track_model->max_attempts);
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
		    (double) jukebox_track_view->test_box_x0 + MONOTHEK_JUKEBOX_TRACK_VIEW_PADDING_LEFT,
		    (double) jukebox_track_view->test_box_y0 + MONOTHEK_JUKEBOX_TRACK_VIEW_PADDING_TOP);

      pango_cairo_update_layout(cr, attempts_layout);
      pango_cairo_show_layout(cr, attempts_layout);

      g_object_unref(attempts_layout);

      g_free(attempts);
    
      /* test - label */    
      layout = pango_cairo_create_layout(cr);
      pango_layout_set_text(layout, "TEST IT", -1);
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
		    (double) jukebox_track_view->test_box_x0 + ((jukebox_track_view->test_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 880.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_test_active){
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

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_play_active){
	cairo_set_source_rgb(cr,
			     1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			     1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			     1.0 / 255.0 * ((0xff & view->jukebox_gc)));
      }
    
      cairo_set_line_width(cr,
			   jukebox_track_view->play_box_line_width);
      cairo_rectangle(cr,
		      (double) jukebox_track_view->play_box_x0, (double) jukebox_track_view->play_box_y0,
		      (double) jukebox_track_view->play_box_width, (double) jukebox_track_view->play_box_height);

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_play_active){
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
      pango_layout_set_text(layout, "PLAY THIS ONE", -1);
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
		    (double) jukebox_track_view->play_box_x0 + ((jukebox_track_view->play_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 880.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_play_active){
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

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_back_active){
	cairo_set_source_rgb(cr,
			     1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			     1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			     1.0 / 255.0 * ((0xff & view->jukebox_gc)));
      }

      cairo_set_line_width(cr,
			   jukebox_track_view->back_box_line_width);
      cairo_rectangle(cr,
		      (double) jukebox_track_view->back_box_x0, (double) jukebox_track_view->back_box_y0,
		      (double) jukebox_track_view->back_box_width, (double) jukebox_track_view->back_box_height);

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_back_active){
	cairo_fill(cr);
      
	cairo_set_source_rgb(cr,
			     0.,
			     0.0,
			     0.0);
      }else{
	cairo_stroke(cr);
      }

      /* back */
      layout = pango_cairo_create_layout(cr);
      pango_layout_set_text(layout, "GO BACK", -1);
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
		    (double) jukebox_track_view->back_box_x0 + ((jukebox_track_view->back_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 880.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      if(jukebox_track_model != NULL &&
	 jukebox_track_model->jukebox_back_active){
	cairo_set_source_rgb(cr,
			     1.0 / 255.0 * ((0xff0000 & view->jukebox_gc) >> 16),
			     1.0 / 255.0 * ((0xff00 & view->jukebox_gc) >> 8),
			     1.0 / 255.0 * ((0xff & view->jukebox_gc)));
      }

      /* free font string */
      g_free(jukebox_font);
    }
  }
  
  /* jukebox - time */
  if((MONOTHEK_JUKEBOX_TRACK_VIEW_PLAYBACK_CONTROLS & (jukebox_track_view->flags)) != 0){
    {
      PangoLayout *layout;
      PangoFontDescription *desc;

      PangoRectangle ink_rect;
      PangoRectangle logical_rect;
    
      gchar *jukebox_font;
    
      static const guint font_size = 100;

      jukebox_font = g_strdup_printf("%s Bold", view->font);

      /* jukebox - total */
      layout = pango_cairo_create_layout(cr);

      if(jukebox_track_model != NULL){
	gchar *str;

	str = g_strdup_printf("%d:%.2d",
			      (guint) floor(jukebox_track_model->duration->tv_sec / 60.0),
			      jukebox_track_model->duration->tv_sec % 60);
      
	pango_layout_set_text(layout, str, -1);

	g_free(str);
      }else{
	pango_layout_set_text(layout, "0:00", -1);
      }
    
      desc = pango_font_description_from_string(jukebox_font);
      pango_font_description_set_size(desc,
				      19 * PANGO_SCALE);
      pango_layout_set_font_description(layout, desc);
      pango_font_description_free(desc);

      pango_layout_get_pixel_extents(layout,
				     &ink_rect,
				     &logical_rect);
      cairo_move_to(cr,
		    (double) 340.0,
		    (double) 900.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      /* jukebox - remaining */
      layout = pango_cairo_create_layout(cr);

      if(jukebox_track_model != NULL){
	gchar *str;

	gdouble value;

	value = jukebox_track_view->progress->value;
      
	str = g_strdup_printf("%d:%.2d",
			      (guint) floor(value * jukebox_track_model->duration->tv_sec / 60.0),
			      (guint) (value * jukebox_track_model->duration->tv_sec) % 60);
      
	pango_layout_set_text(layout, str, -1);

	g_free(str);
      }else{
	pango_layout_set_text(layout, "-0:00", -1);
      }

      desc = pango_font_description_from_string(jukebox_font);
      pango_font_description_set_size(desc,
				      19 * PANGO_SCALE);
      pango_layout_set_font_description(layout, desc);
      pango_font_description_free(desc);

      pango_layout_get_pixel_extents(layout,
				     &ink_rect,
				     &logical_rect);
      cairo_move_to(cr,
		    (double) 1500.0,
		    (double) 900.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);
    }
  }
  
  /* jukebox - info */
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

    if(jukebox_track_model != NULL){
      gchar *str;

      str = g_strdup_printf("%s", jukebox_track_model->song_title);
      
      pango_layout_set_text(layout, str, -1);

      g_free(str);
    }else{
      pango_layout_set_text(layout, "(null)", -1);
    }

    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    30 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) ((1920.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 652.9);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* jukebox - artist and album */
    layout = pango_cairo_create_layout(cr);

    if(jukebox_track_model != NULL){
      gchar *str;

      str = g_strdup_printf("%s - %s", jukebox_track_model->artist, jukebox_track_model->album);
      
      pango_layout_set_text(layout, str, -1);

      g_free(str);
    }else{
      pango_layout_set_text(layout, "(null)", -1);
    }

    desc = pango_font_description_from_string(jukebox_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) ((1920.0 / 2.0) - (logical_rect.width / 2.0)),
		  (double) 752.9);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
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
monothek_jukebox_track_view_reset(MonothekView *view,
				  gboolean reset_defaults, gboolean reset_current)
{
  MonothekWindow *window;
  MonothekJukeboxTrackView *jukebox_track_view;
  
  MonothekJukeboxTrackController *jukebox_track_controller;
  
  MonothekJukeboxTrackModel *jukebox_track_model;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  GList *list;
  
  gchar *song_filename;

  guint test_count;
  
  GValue *jukebox_song_filename;
  GValue *jukebox_test_count;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(view);

  monothek_view_clear(view,
		      TRUE, TRUE);

  /* find session */
  session_manager = monothek_session_manager_get_instance();
  session = monothek_session_manager_find_session(session_manager,
						  MONOTHEK_SESSION_DEFAULT_SESSION);
  
  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);
  
  g_object_get(view,
	       "model", &jukebox_track_model,
	       NULL);

  jukebox_track_controller = NULL;
  list = monothek_controller_find_view_type(window->controller,
					    MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW);

  if(list != NULL){
    jukebox_track_controller = list->data;
  }

  /* get jukebox song filename */
  jukebox_song_filename = g_hash_table_lookup(session->value,
					      "jukebox-song-filename");

  song_filename = g_value_get_string(jukebox_song_filename);

  /* load song filename */
  monothek_jukebox_track_model_load_song_filename(MONOTHEK_VIEW(jukebox_track_view)->model,
						  song_filename);

  /* get test count */
  jukebox_test_count = g_hash_table_lookup(session->value,
					   "jukebox-test-count");

  test_count = 0;

  if(jukebox_test_count != NULL){
    test_count = g_value_get_uint(jukebox_test_count);
  }

  g_object_set(jukebox_track_model,
	       "attempts", test_count,
	       NULL);
  
  if(reset_current){
    switch(jukebox_track_model->attempts){
    case 0:
    case 1:
    case 2:
    {
      jukebox_track_view->flags &= (~(MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_NO_TEST_CONTROL));

      /* play */
      jukebox_track_view->play_box_x0 = 120.0;

      jukebox_track_controller->jukebox_play->x0 = 120;

      /* test */
      jukebox_track_view->test_box_x0 = 700.0;

      jukebox_track_controller->jukebox_test->enabled = TRUE;

      /* back */
      jukebox_track_view->back_box_x0 = 1280.0;

      jukebox_track_controller->jukebox_back->x0 = 1280;
    }
    break;
    case 3:
    {
      jukebox_track_view->flags |= (MONOTHEK_JUKEBOX_TRACK_VIEW_CONFIRM_NO_TEST_CONTROL);

      /* play */
      jukebox_track_view->play_box_x0 = 410.0;

      jukebox_track_controller->jukebox_play->x0 = 410;

      /* test */
      jukebox_track_controller->jukebox_test->enabled = FALSE;

      /* back */
      jukebox_track_view->back_box_x0 = 990.0;

      jukebox_track_controller->jukebox_back->x0 = 990;
    }
    break;
    }
  }
}

void
monothek_jukebox_track_view_clear(MonothekView *view,
				  gboolean clear_all, gboolean clear_hover)
{
  MonothekJukeboxTrackView *jukebox_track_view;

  MonothekJukeboxTrackModel *jukebox_track_model;

  jukebox_track_view = MONOTHEK_JUKEBOX_TRACK_VIEW(view);

  g_object_get(view,
	       "model", &jukebox_track_model,
	       NULL);

  if(clear_all || clear_hover){
    jukebox_track_model->jukebox_test_active = FALSE;
    jukebox_track_model->jukebox_play_active = FALSE;
    jukebox_track_model->jukebox_back_active = FALSE;
  }
}

/**
 * monothek_jukebox_track_view_new:
 *
 * Creates an #MonothekJukeboxTrackView
 *
 * Returns: a new #MonothekJukeboxTrackView
 *
 * Since: 1.0.0
 */
MonothekJukeboxTrackView*
monothek_jukebox_track_view_new()
{
  MonothekJukeboxTrackView *jukebox_track_view;

  jukebox_track_view = (MonothekJukeboxTrackView *) g_object_new(MONOTHEK_TYPE_JUKEBOX_TRACK_VIEW,
								 NULL);
  
  return(jukebox_track_view);
}
