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

#include <monothek/ui/view/monothek_diskjokey_sequencer_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_sequencer_model.h>

#include <monothek/ui/controller/monothek_diskjokey_sequencer_controller.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_sequencer_view_class_init(MonothekDiskjokeySequencerViewClass *diskjokey_sequencer_view);
void monothek_diskjokey_sequencer_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_sequencer_view_init(MonothekDiskjokeySequencerView *diskjokey_sequencer_view);
void monothek_diskjokey_sequencer_view_set_property(GObject *gobject,
						    guint prop_id,
						    const GValue *value,
						    GParamSpec *param_spec);
void monothek_diskjokey_sequencer_view_get_property(GObject *gobject,
						    guint prop_id,
						    GValue *value,
						    GParamSpec *param_spec);
void monothek_diskjokey_sequencer_view_finalize(GObject *gobject);

void monothek_diskjokey_sequencer_view_connect(AgsConnectable *connectable);
void monothek_diskjokey_sequencer_view_disconnect(AgsConnectable *connectable);

void monothek_diskjokey_sequencer_view_progress_callback(GtkAdjustment *adjustment,
							 MonothekDiskjokeySequencerView *diskjokey_sequencer_view);

void monothek_diskjokey_sequencer_view_draw(MonothekView *view);

void monothek_diskjokey_sequencer_view_reset(MonothekView *view,
					     gboolean reset_defaults, gboolean reset_current);
void monothek_diskjokey_sequencer_view_clear(MonothekView *view,
					     gboolean clear_all, gboolean clear_hover);

/**
 * SECTION:monothek_diskjokey_sequencer_view
 * @short_description: The diskjokey sequencer view object.
 * @title: MonothekDiskjokeySequencerView
 * @section_id:
 * @include: monothek/ui/view/monothek_diskjokey_sequencer_view.h
 *
 * #MonothekDiskjokeySequencerView is the MVC's diskjokey sequencer view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_diskjokey_sequencer_view_parent_class = NULL;
static AgsConnectableInterface *monothek_diskjokey_sequencer_view_parent_connectable_interface;

GType
monothek_diskjokey_sequencer_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_sequencer_view = 0;

    static const GTypeInfo monothek_diskjokey_sequencer_view_info = {
      sizeof (MonothekDiskjokeySequencerViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_sequencer_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeySequencerView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_sequencer_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_diskjokey_sequencer_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_diskjokey_sequencer_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
								    "MonothekDiskjokeySequencerView", &monothek_diskjokey_sequencer_view_info,
								    0);
    
    g_type_add_interface_static(monothek_type_diskjokey_sequencer_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_sequencer_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_sequencer_view_class_init(MonothekDiskjokeySequencerViewClass *diskjokey_sequencer_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_diskjokey_sequencer_view_parent_class = g_type_class_peek_parent(diskjokey_sequencer_view);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_sequencer_view;

  gobject->set_property = monothek_diskjokey_sequencer_view_set_property;
  gobject->get_property = monothek_diskjokey_sequencer_view_get_property;

  gobject->finalize = monothek_diskjokey_sequencer_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) diskjokey_sequencer_view;

  view->draw = monothek_diskjokey_sequencer_view_draw;

  view->reset = monothek_diskjokey_sequencer_view_reset;
  view->clear = monothek_diskjokey_sequencer_view_clear;
}

void
monothek_diskjokey_sequencer_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_diskjokey_sequencer_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_diskjokey_sequencer_view_connect;
  connectable->disconnect = monothek_diskjokey_sequencer_view_disconnect;
}

void
monothek_diskjokey_sequencer_view_init(MonothekDiskjokeySequencerView *diskjokey_sequencer_view)
{
  diskjokey_sequencer_view->active_column_line_width = 6.0;
  
  /* pattern */
  diskjokey_sequencer_view->pattern_line_width = 5.0;

  diskjokey_sequencer_view->pattern_x0 = 160.0;
  diskjokey_sequencer_view->pattern_y0 = 80.0;

  diskjokey_sequencer_view->pattern_column_spacing = 20.0;
  diskjokey_sequencer_view->pattern_row_spacing = 20.0;

  diskjokey_sequencer_view->pad_width = 60.0;
  diskjokey_sequencer_view->pad_height = 60.0;

  /* tab - 0 */
  diskjokey_sequencer_view->tab_box_line_width[0] = 5.0;

  diskjokey_sequencer_view->tab_box_x0[0] = 160.0;
  diskjokey_sequencer_view->tab_box_y0[0] = 800.0;

  diskjokey_sequencer_view->tab_box_width[0] = 300.0;
  diskjokey_sequencer_view->tab_box_height[0] = 60.0;

  /* tab - 1 */
  diskjokey_sequencer_view->tab_box_line_width[1] = 5.0;

  diskjokey_sequencer_view->tab_box_x0[1] = 480.0;
  diskjokey_sequencer_view->tab_box_y0[1] = 800.0;

  diskjokey_sequencer_view->tab_box_width[1] = 300.0;
  diskjokey_sequencer_view->tab_box_height[1] = 60.0;

  /* tab - 2 */
  diskjokey_sequencer_view->tab_box_line_width[2] = 5.0;

  diskjokey_sequencer_view->tab_box_x0[2] = 800.0;
  diskjokey_sequencer_view->tab_box_y0[2] = 800.0;

  diskjokey_sequencer_view->tab_box_width[2] = 300.0;
  diskjokey_sequencer_view->tab_box_height[2] = 60.0;

  /* tab - 3 */
  diskjokey_sequencer_view->tab_box_line_width[3] = 5.0;

  diskjokey_sequencer_view->tab_box_x0[3] = 1120.0;
  diskjokey_sequencer_view->tab_box_y0[3] = 800.0;

  diskjokey_sequencer_view->tab_box_width[3] = 300.0;
  diskjokey_sequencer_view->tab_box_height[3] = 60.0;
  
  /* bpm */
  diskjokey_sequencer_view->bpm_box_line_width = 5.0;

  diskjokey_sequencer_view->bpm_box_x0 = 160.0;
  diskjokey_sequencer_view->bpm_box_y0 = 880.0;

  diskjokey_sequencer_view->bpm_box_width = 140.0;
  diskjokey_sequencer_view->bpm_box_height = 60.0;

  diskjokey_sequencer_view->bpm_control_line_width = 5.0;

  diskjokey_sequencer_view->bpm_control_x0 = 160.0;
  diskjokey_sequencer_view->bpm_control_y0 = 990.0;

  diskjokey_sequencer_view->bpm_control_width = 620.0;
  diskjokey_sequencer_view->bpm_control_height = 20.0;

  /* swing */
  diskjokey_sequencer_view->swing_box_line_width = 5.0;

  diskjokey_sequencer_view->swing_box_x0 = 800.0;
  diskjokey_sequencer_view->swing_box_y0 = 880.0;

  diskjokey_sequencer_view->swing_box_width = 60.0;
  diskjokey_sequencer_view->swing_box_height = 60.0;

  diskjokey_sequencer_view->swing_control_line_width = 5.0;

  diskjokey_sequencer_view->swing_control_x0 = 800.0;
  diskjokey_sequencer_view->swing_control_y0 = 990.0;

  diskjokey_sequencer_view->swing_control_width = 620.0;
  diskjokey_sequencer_view->swing_control_height = 20.0;
  
  /* techno */
  diskjokey_sequencer_view->techno_box_line_width = 5.0;

  diskjokey_sequencer_view->techno_box_x0 = 1460.0;
  diskjokey_sequencer_view->techno_box_y0 = 80.0;

  diskjokey_sequencer_view->techno_box_width = 420.0;
  diskjokey_sequencer_view->techno_box_height = 140.0;

  /* house */
  diskjokey_sequencer_view->house_box_line_width = 5.0;

  diskjokey_sequencer_view->house_box_x0 = 1460.0;
  diskjokey_sequencer_view->house_box_y0 = 240.0;

  diskjokey_sequencer_view->house_box_width = 420.0;
  diskjokey_sequencer_view->house_box_height = 140.0;

  /* hiphop */
  diskjokey_sequencer_view->hiphop_box_line_width = 5.0;

  diskjokey_sequencer_view->hiphop_box_x0 = 1460.0;
  diskjokey_sequencer_view->hiphop_box_y0 = 400.0;

  diskjokey_sequencer_view->hiphop_box_width = 420.0;
  diskjokey_sequencer_view->hiphop_box_height = 140.0;

  /* random */
  diskjokey_sequencer_view->random_box_line_width = 5.0;

  diskjokey_sequencer_view->random_box_x0 = 1460.0;
  diskjokey_sequencer_view->random_box_y0 = 560.0;

  diskjokey_sequencer_view->random_box_width = 200.0;
  diskjokey_sequencer_view->random_box_height = 60.0;

  /* clear */
  diskjokey_sequencer_view->clear_box_line_width = 5.0;

  diskjokey_sequencer_view->clear_box_x0 = 1680.0;
  diskjokey_sequencer_view->clear_box_y0 = 560.0;

  diskjokey_sequencer_view->clear_box_width = 200.0;
  diskjokey_sequencer_view->clear_box_height = 60.0;

  /* timer */
  diskjokey_sequencer_view->progress = gtk_adjustment_new(0.0, 0.0, 1.0, 0.0001, 0.01, 0.01);

  diskjokey_sequencer_view->timer_box_line_width = 5.0;

  diskjokey_sequencer_view->timer_box_x0 = 1460.0;
  diskjokey_sequencer_view->timer_box_y0 = 720.0;

  diskjokey_sequencer_view->timer_box_width = 420.0;
  diskjokey_sequencer_view->timer_box_height = 140.0;

  /* stop */
  diskjokey_sequencer_view->stop_box_line_width = 5.0;

  diskjokey_sequencer_view->stop_box_x0 = 1460.0;
  diskjokey_sequencer_view->stop_box_y0 = 880.0;

  diskjokey_sequencer_view->stop_box_width = 420.0;
  diskjokey_sequencer_view->stop_box_height = 140.0;
}

void
monothek_diskjokey_sequencer_view_set_property(GObject *gobject,
					       guint prop_id,
					       const GValue *value,
					       GParamSpec *param_spec)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;

  diskjokey_sequencer_view = MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_sequencer_view_get_property(GObject *gobject,
					       guint prop_id,
					       GValue *value,
					       GParamSpec *param_spec)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;

  diskjokey_sequencer_view = MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_diskjokey_sequencer_view_finalize(GObject *gobject)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;

  diskjokey_sequencer_view = (MonothekDiskjokeySequencerView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_sequencer_view_parent_class)->finalize(gobject);
}

void
monothek_diskjokey_sequencer_view_connect(AgsConnectable *connectable)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;

  GList *list, *list_diskjokey_sequencer;

  diskjokey_sequencer_view = MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(diskjokey_sequencer_view)->flags)) != 0){
    return;
  }

  monothek_diskjokey_sequencer_view_parent_connectable_interface->connect(connectable);

  g_signal_connect(diskjokey_sequencer_view->progress, "value-changed",
		   G_CALLBACK(monothek_diskjokey_sequencer_view_progress_callback), diskjokey_sequencer_view);
}

void
monothek_diskjokey_sequencer_view_disconnect(AgsConnectable *connectable)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;

  GList *list, *list_diskjokey_sequencer;

  diskjokey_sequencer_view = MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(diskjokey_sequencer_view)->flags)) == 0){
    return;
  }

  monothek_diskjokey_sequencer_view_parent_connectable_interface->disconnect(connectable);

  g_object_disconnect(diskjokey_sequencer_view->progress,
		      "any_signal::value-changed",
		      G_CALLBACK(monothek_diskjokey_sequencer_view_progress_callback),
		      diskjokey_sequencer_view,
		      NULL);
}

void
monothek_diskjokey_sequencer_view_progress_callback(GtkAdjustment *adjustment,
						    MonothekDiskjokeySequencerView *diskjokey_sequencer_view)
{
  gtk_widget_queue_draw(diskjokey_sequencer_view);
}

void
monothek_diskjokey_sequencer_view_draw(MonothekView *view)
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;
  
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  cairo_t *cr;

  guint width, height;
  guint x_diskjokey_sequencer, y_diskjokey_sequencer;
  guint i, j;
  
  static const gdouble white_gc = 65535.0;

  diskjokey_sequencer_view = MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_diskjokey_sequencer_view_parent_class)->draw(view);

  /* create cr */
  cr = gdk_cairo_create(GTK_WIDGET(view)->window);
  
  if(cr == NULL){
    return;
  }

  g_object_get(view,
	       "model", &diskjokey_sequencer_model,
	       NULL);

  cairo_surface_flush(cairo_get_target(cr));
  cairo_push_group(cr);

  x_diskjokey_sequencer = 0;
  y_diskjokey_sequencer = 0;

  width = GTK_WIDGET(view)->allocation.width;
  height = GTK_WIDGET(view)->allocation.height;

  cairo_set_source_rgb(cr,
		       1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
		       1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
		       1.0 / 255.0 * ((0xff & view->diskjokey_gc)));

  /* columns numeration */
  for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT; j++){
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    gchar *label;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    label = g_strdup_printf("%d", (diskjokey_sequencer_model->current_tab * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT) + j + 1);
    
    /* columns */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, label, -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) diskjokey_sequencer_view->pattern_x0 + (j * (diskjokey_sequencer_view->pad_width + diskjokey_sequencer_view->pattern_column_spacing)) + ((diskjokey_sequencer_view->pad_width / 2.0) - (logical_rect.width / 2.0)),
		  (double) 40.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    g_free(diskjokey_font);
    g_free(label);
  }

  /* row sound */
  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_ROW_COUNT; i++){
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    gchar *label;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    if(diskjokey_sequencer_model->current_genre == MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO){
      label = g_strdup(diskjokey_sequencer_model->techno_label[i]);
    }else if(diskjokey_sequencer_model->current_genre == MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE){
      label = g_strdup(diskjokey_sequencer_model->house_label[i]);
    }else if(diskjokey_sequencer_model->current_genre == MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP){
      label = g_strdup(diskjokey_sequencer_model->hiphop_label[i]);
    }else{
      label = g_strdup("(null)");
    }
    
    /* rows */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, label, -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 140.0 - (logical_rect.width),
		  (double) diskjokey_sequencer_view->pattern_y0 + 20.0 + (i * (diskjokey_sequencer_view->pad_height + diskjokey_sequencer_view->pattern_row_spacing)));

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    g_free(diskjokey_font);
    g_free(label);
  }
  
  /* pattern */
  if(diskjokey_sequencer_model->active_column != -1){
    if(diskjokey_sequencer_model->active_column >= diskjokey_sequencer_model->current_tab * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT &&
       diskjokey_sequencer_model->active_column < (diskjokey_sequencer_model->current_tab + 1) * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT){
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->active_column_line_width);

      cairo_rectangle(cr,
		      diskjokey_sequencer_view->pattern_x0 - 4 + (diskjokey_sequencer_model->active_column % MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT) * (diskjokey_sequencer_view->pad_width + diskjokey_sequencer_view->pattern_column_spacing), diskjokey_sequencer_view->pattern_y0 - 4,
		      (double) diskjokey_sequencer_view->pad_width + 8, (double) MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_ROW_COUNT * (diskjokey_sequencer_view->pad_height + diskjokey_sequencer_view->pattern_row_spacing) - diskjokey_sequencer_view->pattern_row_spacing + 8);
      
      cairo_stroke(cr);
      
      for(i = 1; i < MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_ROW_COUNT; i++){
	cairo_rectangle(cr,
			diskjokey_sequencer_view->pattern_x0 - 5 + (diskjokey_sequencer_model->active_column % MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT) * (diskjokey_sequencer_view->pad_width + diskjokey_sequencer_view->pattern_column_spacing), (double) diskjokey_sequencer_view->pattern_y0 + (i * (diskjokey_sequencer_view->pad_height + diskjokey_sequencer_view->pattern_row_spacing)) - diskjokey_sequencer_view->pattern_row_spacing,
			(double) diskjokey_sequencer_view->pad_width + 10, (double) diskjokey_sequencer_view->pattern_row_spacing);

	cairo_fill(cr);
      }
    }
  }
  
  cairo_set_line_width(cr,
		       diskjokey_sequencer_view->pattern_line_width);

  for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_ROW_COUNT; i++){
    for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT; j++){
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->pattern_x0 + (j * (diskjokey_sequencer_view->pad_width + diskjokey_sequencer_view->pattern_column_spacing)), (double) diskjokey_sequencer_view->pattern_y0 + (i * (diskjokey_sequencer_view->pad_height + diskjokey_sequencer_view->pattern_row_spacing)),
		      (double) diskjokey_sequencer_view->pad_width, (double) diskjokey_sequencer_view->pad_height);

      if(diskjokey_sequencer_model->pad_active[i][(diskjokey_sequencer_model->current_tab * MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT) + j]){
	cairo_fill(cr);
      }else{
	cairo_stroke(cr);
      }
    }
  }

  /* tabs */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    /* tab 1 */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TAB 1", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (diskjokey_sequencer_model->current_tab != 0 &&
	!diskjokey_sequencer_model->tab_active[0])){
      cairo_move_to(cr,
		    (double) 270.0,
		    (double) 820.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[0]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[0], (double) diskjokey_sequencer_view->tab_box_y0[0],
		      (double) diskjokey_sequencer_view->tab_box_width[0], (double) diskjokey_sequencer_view->tab_box_height[0]);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[0]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[0], (double) diskjokey_sequencer_view->tab_box_y0[0],
		      (double) diskjokey_sequencer_view->tab_box_width[0], (double) diskjokey_sequencer_view->tab_box_height[0]);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);

      cairo_move_to(cr,
		    (double) 270.0,
		    (double) 820.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* tab 2 */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TAB 2", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (diskjokey_sequencer_model->current_tab != 1 &&
	!diskjokey_sequencer_model->tab_active[1])){
      cairo_move_to(cr,
		    (double) 590.0,
		    (double) 820.0);
      
      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);
      
      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[1]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[1], (double) diskjokey_sequencer_view->tab_box_y0[1],
		      (double) diskjokey_sequencer_view->tab_box_width[1], (double) diskjokey_sequencer_view->tab_box_height[1]);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[1]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[1], (double) diskjokey_sequencer_view->tab_box_y0[1],
		      (double) diskjokey_sequencer_view->tab_box_width[1], (double) diskjokey_sequencer_view->tab_box_height[1]);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);
      
      cairo_move_to(cr,
		    (double) 590.0,
		    (double) 820.0);
      
      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);
      
      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* tab 3 */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TAB 3", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (diskjokey_sequencer_model->current_tab != 2 &&
	!diskjokey_sequencer_model->tab_active[2])){
      cairo_move_to(cr,
		    (double) 910.0,
		    (double) 820.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[2]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[2], (double) diskjokey_sequencer_view->tab_box_y0[2],
		      (double) diskjokey_sequencer_view->tab_box_width[2], (double) diskjokey_sequencer_view->tab_box_height[2]);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[2]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[2], (double) diskjokey_sequencer_view->tab_box_y0[2],
		      (double) diskjokey_sequencer_view->tab_box_width[2], (double) diskjokey_sequencer_view->tab_box_height[2]);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);

      cairo_move_to(cr,
		    (double) 910.0,
		    (double) 820.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* tab 4 */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TAB 4", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (diskjokey_sequencer_model->current_tab != 3 &&
	!diskjokey_sequencer_model->tab_active[3])){
      cairo_move_to(cr,
		    (double) 1230.0,
		    (double) 820.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);
  
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[3]);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[3], (double) diskjokey_sequencer_view->tab_box_y0[3],
		      (double) diskjokey_sequencer_view->tab_box_width[3], (double) diskjokey_sequencer_view->tab_box_height[3]);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->tab_box_line_width[3]);  
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->tab_box_x0[3], (double) diskjokey_sequencer_view->tab_box_y0[3],
		      (double) diskjokey_sequencer_view->tab_box_width[3], (double) diskjokey_sequencer_view->tab_box_height[3]);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);

      cairo_move_to(cr,
		    (double) 1230.0,
		    (double) 820.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* free font string */
    g_free(diskjokey_font);
  }

  /* bpm */
  cairo_rectangle(cr,
		  (double) diskjokey_sequencer_view->bpm_box_x0, (double) diskjokey_sequencer_view->bpm_box_y0,
		  (double) diskjokey_sequencer_view->bpm_box_width, (double) diskjokey_sequencer_view->bpm_box_height);
  cairo_stroke(cr);
  
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    gchar *bpm;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    bpm = g_strdup_printf("%.0f", diskjokey_sequencer_model->bpm);
    
    /* bpm */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, bpm, -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 200.0,
		  (double) 900.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* label */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "BPM", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 320.0,
		  (double) 900.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* free font string */
    g_free(diskjokey_font);
  }

  cairo_rectangle(cr,
		  (double) diskjokey_sequencer_view->bpm_control_x0, (double) diskjokey_sequencer_view->bpm_control_y0,
		  (double) diskjokey_sequencer_view->bpm_control_width, (double) diskjokey_sequencer_view->bpm_control_height);
  cairo_fill(cr);


  if((diskjokey_sequencer_model->bpm - diskjokey_sequencer_model->bpm_lower) == (diskjokey_sequencer_model->bpm_upper - diskjokey_sequencer_model->bpm_lower) / 2.0){
    cairo_rectangle(cr,
		    (double) diskjokey_sequencer_view->bpm_control_x0 + ((gdouble) diskjokey_sequencer_view->bpm_control_width / 2.0), (double) diskjokey_sequencer_view->bpm_control_y0 - 10.0,
		    (double) 40.0, (double) 40.0);
  }else{
    cairo_rectangle(cr,
		    (double) diskjokey_sequencer_view->bpm_control_x0 + (((gdouble) diskjokey_sequencer_view->bpm_control_width - 40.0) / (diskjokey_sequencer_model->bpm_upper - diskjokey_sequencer_model->bpm_lower) * (diskjokey_sequencer_model->bpm - diskjokey_sequencer_model->bpm_lower)), (double) diskjokey_sequencer_view->bpm_control_y0 - 10.0,
		    (double) 40.0, (double) 40.0);
  }
  cairo_fill(cr);

  /* swing */
  cairo_rectangle(cr,
		  (double) diskjokey_sequencer_view->swing_box_x0, (double) diskjokey_sequencer_view->swing_box_y0,
		  (double) diskjokey_sequencer_view->swing_box_width, (double) diskjokey_sequencer_view->swing_box_height);
  cairo_stroke(cr);
  
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    gchar *swing;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    swing = g_strdup_printf("6");
    
    /* swing */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, swing, -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 820.0,
		  (double) 900.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* label */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "SWING", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 880.0,
		  (double) 900.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    /* free font string */
    g_free(diskjokey_font);
  }

  cairo_rectangle(cr,
		  (double) diskjokey_sequencer_view->swing_control_x0, (double) diskjokey_sequencer_view->swing_control_y0,
		  (double) diskjokey_sequencer_view->swing_control_width, (double) diskjokey_sequencer_view->swing_control_height);
  cairo_fill(cr);

  cairo_rectangle(cr,
		  (double) diskjokey_sequencer_view->swing_control_x0, (double) diskjokey_sequencer_view->swing_control_y0 - 10.0,
		  (double) 40.0, (double) 40.0);
  cairo_fill(cr);

  /* techno */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);
    
    /* techno */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TECHNO", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    40 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (!diskjokey_sequencer_model->techno_active &&
	diskjokey_sequencer_model->current_genre != MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO)){
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->techno_box_x0 + ((diskjokey_sequencer_view->techno_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 132.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->techno_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->techno_box_x0, (double) diskjokey_sequencer_view->techno_box_y0,
		      (double) diskjokey_sequencer_view->techno_box_width, (double) diskjokey_sequencer_view->techno_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->techno_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->techno_box_x0, (double) diskjokey_sequencer_view->techno_box_y0,
		      (double) diskjokey_sequencer_view->techno_box_width, (double) diskjokey_sequencer_view->techno_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);
      
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->techno_box_x0 + ((diskjokey_sequencer_view->techno_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 132.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* free font string */
    g_free(diskjokey_font);
  }
  
  /* house */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);
    
    /* house */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "HOUSE", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    40 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (!diskjokey_sequencer_model->house_active &&
	diskjokey_sequencer_model->current_genre != MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE)){
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->house_box_x0 + ((diskjokey_sequencer_view->house_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 292.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->house_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->house_box_x0, (double) diskjokey_sequencer_view->house_box_y0,
		      (double) diskjokey_sequencer_view->house_box_width, (double) diskjokey_sequencer_view->house_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->house_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->house_box_x0, (double) diskjokey_sequencer_view->house_box_y0,
		      (double) diskjokey_sequencer_view->house_box_width, (double) diskjokey_sequencer_view->house_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);

      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->house_box_x0 + ((diskjokey_sequencer_view->house_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 292.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* free font string */
    g_free(diskjokey_font);
  }

  /* hiphop */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);
    
    /* hiphop */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "HIPHOP", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    40 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       (!diskjokey_sequencer_model->hiphop_active &&
	diskjokey_sequencer_model->current_genre != MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP)){
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->hiphop_box_x0 + ((diskjokey_sequencer_view->hiphop_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 452.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->hiphop_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->hiphop_box_x0, (double) diskjokey_sequencer_view->hiphop_box_y0,
		      (double) diskjokey_sequencer_view->hiphop_box_width, (double) diskjokey_sequencer_view->hiphop_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->hiphop_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->hiphop_box_x0, (double) diskjokey_sequencer_view->hiphop_box_y0,
		      (double) diskjokey_sequencer_view->hiphop_box_width, (double) diskjokey_sequencer_view->hiphop_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);

      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->hiphop_box_x0 + ((diskjokey_sequencer_view->hiphop_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 452.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }

    /* free font string */
    g_free(diskjokey_font);
  }

  /* random */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);
    
    /* random */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "RANDOM", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       !diskjokey_sequencer_model->random_active){
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->random_box_x0 + ((diskjokey_sequencer_view->random_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 580.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->random_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->random_box_x0, (double) diskjokey_sequencer_view->random_box_y0,
		      (double) diskjokey_sequencer_view->random_box_width, (double) diskjokey_sequencer_view->random_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->random_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->random_box_x0, (double) diskjokey_sequencer_view->random_box_y0,
		      (double) diskjokey_sequencer_view->random_box_width, (double) diskjokey_sequencer_view->random_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);

      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->random_box_x0 + ((diskjokey_sequencer_view->random_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 580.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* free font string */
    g_free(diskjokey_font);
  }

  /* clear */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);
    
    /* clear */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "CLEAR", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       !diskjokey_sequencer_model->clear_active){
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->clear_box_x0 + ((diskjokey_sequencer_view->clear_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 580.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->clear_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->clear_box_x0, (double) diskjokey_sequencer_view->clear_box_y0,
		      (double) diskjokey_sequencer_view->clear_box_width, (double) diskjokey_sequencer_view->clear_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->clear_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->clear_box_x0, (double) diskjokey_sequencer_view->clear_box_y0,
		      (double) diskjokey_sequencer_view->clear_box_width, (double) diskjokey_sequencer_view->clear_box_height);
      cairo_fill(cr);
      
      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);
      
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->clear_box_x0 + ((diskjokey_sequencer_view->clear_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 580.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
    /* free font string */
    g_free(diskjokey_font);
  }

  /* timer */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);

    /* label */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "TIMER", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    19 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) 1460.0,
		  (double) 680.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);
    
    
    /* timer */
    layout = pango_cairo_create_layout(cr);

    if(diskjokey_sequencer_model != NULL){
      gchar *str;

      gdouble value;

      value = diskjokey_sequencer_view->progress->value;
      
      str = g_strdup_printf("%d:%.2d",
			    9 - (guint) floor(value * diskjokey_sequencer_model->duration->tv_sec / 60.0),
			    59 - (guint) (value * diskjokey_sequencer_model->duration->tv_sec) % 60);
      
      pango_layout_set_text(layout, str, -1);

      g_free(str);
    }else{
      pango_layout_set_text(layout, "10:00", -1);
    }

    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    40 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);
    cairo_move_to(cr,
		  (double) diskjokey_sequencer_view->timer_box_x0 + ((diskjokey_sequencer_view->timer_box_width / 2.0) - (logical_rect.width / 2.0)),
		  (double) 772.0);

    pango_cairo_update_layout(cr, layout);
    pango_cairo_show_layout(cr, layout);

    g_object_unref(layout);

    cairo_set_line_width(cr,
			 diskjokey_sequencer_view->timer_box_line_width);
    cairo_rectangle(cr,
		    (double) diskjokey_sequencer_view->timer_box_x0, (double) diskjokey_sequencer_view->timer_box_y0,
		    (double) diskjokey_sequencer_view->timer_box_width, (double) diskjokey_sequencer_view->timer_box_height);
    cairo_stroke(cr);

    /* free font string */
    g_free(diskjokey_font);
  }

  /* stop */
  {
    PangoLayout *layout;
    PangoFontDescription *desc;

    PangoRectangle ink_rect;
    PangoRectangle logical_rect;
    
    gchar *diskjokey_font;
    
    static const guint font_size = 100;

    diskjokey_font = g_strdup_printf("%s Bold", view->font);
    
    /* stop */
    layout = pango_cairo_create_layout(cr);
    pango_layout_set_text(layout, "STOP", -1);
    desc = pango_font_description_from_string(diskjokey_font);
    pango_font_description_set_size(desc,
				    40 * PANGO_SCALE);
    pango_layout_set_font_description(layout, desc);
    pango_layout_set_alignment(layout,
			       PANGO_ALIGN_CENTER);
    pango_font_description_free(desc);

    pango_layout_get_pixel_extents(layout,
				   &ink_rect,
				   &logical_rect);

    if(diskjokey_sequencer_model == NULL ||
       !diskjokey_sequencer_model->run_active){
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->stop_box_x0 + ((diskjokey_sequencer_view->stop_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 932.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->stop_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->stop_box_x0, (double) diskjokey_sequencer_view->stop_box_y0,
		      (double) diskjokey_sequencer_view->stop_box_width, (double) diskjokey_sequencer_view->stop_box_height);
      cairo_stroke(cr);
    }else{
      cairo_set_line_width(cr,
			   diskjokey_sequencer_view->stop_box_line_width);
      cairo_rectangle(cr,
		      (double) diskjokey_sequencer_view->stop_box_x0, (double) diskjokey_sequencer_view->stop_box_y0,
		      (double) diskjokey_sequencer_view->stop_box_width, (double) diskjokey_sequencer_view->stop_box_height);
      cairo_fill(cr);

      cairo_set_source_rgb(cr,
			   0.0,
			   0.0,
			   0.0);
      
      cairo_move_to(cr,
		    (double) diskjokey_sequencer_view->stop_box_x0 + ((diskjokey_sequencer_view->stop_box_width / 2.0) - (logical_rect.width / 2.0)),
		    (double) 932.0);

      pango_cairo_update_layout(cr, layout);
      pango_cairo_show_layout(cr, layout);

      g_object_unref(layout);

      cairo_set_source_rgb(cr,
			   1.0 / 255.0 * ((0xff0000 & view->diskjokey_gc) >> 16),
			   1.0 / 255.0 * ((0xff00 & view->diskjokey_gc) >> 8),
			   1.0 / 255.0 * ((0xff & view->diskjokey_gc)));
    }
    
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
monothek_diskjokey_sequencer_view_reset(MonothekView *view,
					gboolean reset_defaults, gboolean reset_current)
{
  MonothekWindow *window;
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;
  
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  GList *list;

  gdouble bpm;
  gdouble swing;

#ifdef __APPLE__
  clock_serv_t cclock;
  mach_timespec_t mts;
#endif

  diskjokey_sequencer_view = MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(view);

  window = gtk_widget_get_ancestor(view,
				   MONOTHEK_TYPE_WINDOW);

  g_object_get(view,
	       "model", &diskjokey_sequencer_model,
	       NULL);

  diskjokey_sequencer_controller = NULL;
  list = monothek_controller_find_view_type(window->controller,
					    MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW);

  if(list != NULL){
    diskjokey_sequencer_controller = list->data;
  }

  if(reset_current){
    //TODO:JK: implement me
  }

  if(reset_defaults){
    diskjokey_sequencer_model->current_genre = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO;

    diskjokey_sequencer_model->techno_active = TRUE;
    diskjokey_sequencer_model->house_active = FALSE;
    diskjokey_sequencer_model->hiphop_active = FALSE;

    diskjokey_sequencer_model->random_active = FALSE;
    diskjokey_sequencer_model->clear_active = FALSE;

    diskjokey_sequencer_model->run_active = FALSE;

    diskjokey_sequencer_model->active_column = -1;

    diskjokey_sequencer_model->current_tab = 0;

    diskjokey_sequencer_model->tab_active[0] = TRUE;
    diskjokey_sequencer_model->tab_active[1] = FALSE;
    diskjokey_sequencer_model->tab_active[2] = FALSE;
    diskjokey_sequencer_model->tab_active[3] = FALSE;

    /* bpm */
    bpm = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_BPM_DEFAULT;
  
    diskjokey_sequencer_model->bpm = bpm;
    monothek_diskjokey_sequencer_controller_change_bpm(diskjokey_sequencer_controller,
						       bpm);

    /* swing */
    swing = MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_SWING_DEFAULT;
  
    diskjokey_sequencer_model->swing = swing;
    monothek_diskjokey_sequencer_controller_change_swing(diskjokey_sequencer_controller,
							 swing);
  
    /* load drum kit */
    monothek_diskjokey_sequencer_controller_load_drum_kit(diskjokey_sequencer_controller,
							  MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO_FILENAME);

    /* reset timer */
#ifdef __APPLE__
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    diskjokey_sequencer_controller->start_time->tv_sec = mts.tv_sec;
    diskjokey_sequencer_controller->start_time->tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_MONOTONIC, diskjokey_sequencer_controller->start_time);
#endif

    diskjokey_sequencer_controller->timer->tv_sec = 0;
    diskjokey_sequencer_controller->timer->tv_nsec = 0;
  }
}
  
void
monothek_diskjokey_sequencer_view_clear(MonothekView *view,
					gboolean clear_all, gboolean clear_hover)
{
  MonothekDiskjokeySequencerModel *diskjokey_sequencer_model;

  g_object_get(view,
	       "model", &diskjokey_sequencer_model,
	       NULL);
  
  if(clear_all || clear_hover){
    if(diskjokey_sequencer_model->current_genre != MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_TECHNO){
      diskjokey_sequencer_model->techno_active = FALSE;
    }

    if(diskjokey_sequencer_model->current_genre != MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HOUSE){
      diskjokey_sequencer_model->house_active = FALSE;
    }

    if(diskjokey_sequencer_model->current_genre != MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_HIPHOP){
      diskjokey_sequencer_model->hiphop_active = FALSE;
    }
    
    diskjokey_sequencer_model->random_active = FALSE;
    diskjokey_sequencer_model->clear_active = FALSE;

    if(diskjokey_sequencer_model->current_tab != 0){
      diskjokey_sequencer_model->tab_active[0] = FALSE;
    }

    if(diskjokey_sequencer_model->current_tab != 1){
      diskjokey_sequencer_model->tab_active[1] = FALSE;
    }

    if(diskjokey_sequencer_model->current_tab != 2){
      diskjokey_sequencer_model->tab_active[2] = FALSE;
    }

    if(diskjokey_sequencer_model->current_tab != 3){
      diskjokey_sequencer_model->tab_active[3] = FALSE;
    }
  }
  
  if(clear_all){
    guint i, j;

    for(i = 0; i < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_ROW_COUNT; i++){
      for(j = 0; j < MONOTHEK_DISKJOKEY_SEQUENCER_MODEL_COLUMN_COUNT; j++){
	diskjokey_sequencer_model->pad_active[i][j] = FALSE;
      }
    }
  }
}

/**
 * monothek_diskjokey_sequencer_view_new:
 *
 * Creates an #MonothekDiskjokeySequencerView
 *
 * Returns: a new #MonothekDiskjokeySequencerView
 *
 * Since: 1.0.0
 */
MonothekDiskjokeySequencerView*
monothek_diskjokey_sequencer_view_new()
{
  MonothekDiskjokeySequencerView *diskjokey_sequencer_view;

  diskjokey_sequencer_view = (MonothekDiskjokeySequencerView *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW,
									     NULL);
  
  return(diskjokey_sequencer_view);
}
