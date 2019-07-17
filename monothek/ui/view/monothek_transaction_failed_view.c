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

#include <monothek/ui/view/monothek_transaction_failed_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_transaction_failed_view_class_init(MonothekTransactionFailedViewClass *transaction_failed_view);
void monothek_transaction_failed_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_transaction_failed_view_init(MonothekTransactionFailedView *transaction_failed_view);
void monothek_transaction_failed_view_set_property(GObject *gobject,
						   guint prop_id,
						   const GValue *value,
						   GParamSpec *param_spec);
void monothek_transaction_failed_view_get_property(GObject *gobject,
						   guint prop_id,
						   GValue *value,
						   GParamSpec *param_spec);
void monothek_transaction_failed_view_finalize(GObject *gobject);

void monothek_transaction_failed_view_connect(AgsConnectable *connectable);
void monothek_transaction_failed_view_disconnect(AgsConnectable *connectable);

void monothek_transaction_failed_view_draw(MonothekView *view);

/**
 * SECTION:monothek_transaction_failed_view
 * @short_description: The transaction failed view object.
 * @title: MonothekTransactionFailedView
 * @section_id:
 * @include: monothek/ui/view/monothek_transaction_failed_view.h
 *
 * #MonothekTransactionFailedView is the MVC's transaction failed view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_transaction_failed_view_parent_class = NULL;
static AgsConnectableInterface *monothek_transaction_failed_view_parent_connectable_interface;

GType
monothek_transaction_failed_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_transaction_failed_view = 0;

    static const GTypeInfo monothek_transaction_failed_view_info = {
      sizeof (MonothekTransactionFailedViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_transaction_failed_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekTransactionFailedView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_transaction_failed_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_transaction_failed_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_transaction_failed_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
								   "MonothekTransactionFailedView", &monothek_transaction_failed_view_info,
								   0);
    
    g_type_add_interface_static(monothek_type_transaction_failed_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_transaction_failed_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_transaction_failed_view_class_init(MonothekTransactionFailedViewClass *transaction_failed_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_transaction_failed_view_parent_class = g_type_class_peek_parent(transaction_failed_view);

  /* GObjectClass */
  gobject = (GObjectClass *) transaction_failed_view;

  gobject->set_property = monothek_transaction_failed_view_set_property;
  gobject->get_property = monothek_transaction_failed_view_get_property;

  gobject->finalize = monothek_transaction_failed_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) transaction_failed_view;

  view->draw = monothek_transaction_failed_view_draw;
}

void
monothek_transaction_failed_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_transaction_failed_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_transaction_failed_view_connect;
  connectable->disconnect = monothek_transaction_failed_view_disconnect;
}

void
monothek_transaction_failed_view_init(MonothekTransactionFailedView *transaction_failed_view)
{
  //empty
}

void
monothek_transaction_failed_view_set_property(GObject *gobject,
					      guint prop_id,
					      const GValue *value,
					      GParamSpec *param_spec)
{
  MonothekTransactionFailedView *transaction_failed_view;

  transaction_failed_view = MONOTHEK_TRANSACTION_FAILED_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_transaction_failed_view_get_property(GObject *gobject,
					      guint prop_id,
					      GValue *value,
					      GParamSpec *param_spec)
{
  MonothekTransactionFailedView *transaction_failed_view;

  transaction_failed_view = MONOTHEK_TRANSACTION_FAILED_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_transaction_failed_view_finalize(GObject *gobject)
{
  MonothekTransactionFailedView *transaction_failed_view;

  transaction_failed_view = (MonothekTransactionFailedView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_transaction_failed_view_parent_class)->finalize(gobject);
}

void
monothek_transaction_failed_view_connect(AgsConnectable *connectable)
{
  MonothekTransactionFailedView *transaction_failed_view;

  GList *list, *list_transaction_failed;

  transaction_failed_view = MONOTHEK_TRANSACTION_FAILED_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(transaction_failed_view)->flags)) != 0){
    return;
  }

  monothek_transaction_failed_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_transaction_failed_view_disconnect(AgsConnectable *connectable)
{
  MonothekTransactionFailedView *transaction_failed_view;

  GList *list, *list_transaction_failed;

  transaction_failed_view = MONOTHEK_TRANSACTION_FAILED_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(transaction_failed_view)->flags)) == 0){
    return;
  }

  monothek_transaction_failed_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_transaction_failed_view_draw(MonothekView *view)
{
  MonothekTransactionFailedView *transaction_failed_view;
  
  cairo_t *cr;
  
  static const gdouble white_gc = 65535.0;

  transaction_failed_view = MONOTHEK_TRANSACTION_FAILED_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_transaction_failed_view_parent_class)->draw(view);

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
    pango_layout_set_text(layout, "PROCESS COULD NOT BE\nCOMPLETED", -1);
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
    pango_layout_set_text(layout, "PLEASE TRY AGAIN", -1);
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
 * monothek_transaction_failed_view_new:
 *
 * Creates an #MonothekTransactionFailedView
 *
 * Returns: a new #MonothekTransactionFailedView
 *
 * Since: 1.0.0
 */
MonothekTransactionFailedView*
monothek_transaction_failed_view_new()
{
  MonothekTransactionFailedView *transaction_failed_view;

  transaction_failed_view = (MonothekTransactionFailedView *) g_object_new(MONOTHEK_TYPE_TRANSACTION_FAILED_VIEW,
									   NULL);
  
  return(transaction_failed_view);
}
