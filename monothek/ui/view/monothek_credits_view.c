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

#include <monothek/ui/view/monothek_credits_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_credits_view_class_init(MonothekCreditsViewClass *credits_view);
void monothek_credits_view_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_credits_view_init(MonothekCreditsView *credits_view);
void monothek_credits_view_set_property(GObject *gobject,
				       guint prop_id,
				       const GValue *value,
				       GParamSpec *param_spec);
void monothek_credits_view_get_property(GObject *gobject,
				       guint prop_id,
				       GValue *value,
				       GParamSpec *param_spec);
void monothek_credits_view_finalize(GObject *gobject);

void monothek_credits_view_connect(AgsConnectable *connectable);
void monothek_credits_view_disconnect(AgsConnectable *connectable);

void monothek_credits_view_draw(MonothekView *view);

/**
 * SECTION:monothek_credits_view
 * @short_description: The credits view object.
 * @title: MonothekCreditsView
 * @section_id:
 * @include: monothek/ui/view/monothek_credits_view.h
 *
 * #MonothekCreditsView is the MVC's credits view widget.
 */

enum{
  PROP_0,
};

static gpointer monothek_credits_view_parent_class = NULL;
static AgsConnectableInterface *monothek_credits_view_parent_connectable_interface;

GType
monothek_credits_view_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_credits_view = 0;

    static const GTypeInfo monothek_credits_view_info = {
      sizeof (MonothekCreditsViewClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_credits_view_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekCreditsView),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_credits_view_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_credits_view_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_credits_view = g_type_register_static(MONOTHEK_TYPE_VIEW,
						       "MonothekCreditsView", &monothek_credits_view_info,
						       0);
    
    g_type_add_interface_static(monothek_type_credits_view,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_credits_view);
  }

  return g_define_type_id__volatile;
}

void
monothek_credits_view_class_init(MonothekCreditsViewClass *credits_view)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;
  MonothekViewClass *view;
  
  GParamSpec *param_spec;

  monothek_credits_view_parent_class = g_type_class_peek_parent(credits_view);

  /* GObjectClass */
  gobject = (GObjectClass *) credits_view;

  gobject->set_property = monothek_credits_view_set_property;
  gobject->get_property = monothek_credits_view_get_property;

  gobject->finalize = monothek_credits_view_finalize;

  /* properties */

  /* MonothekView */
  view = (MonothekViewClass *) credits_view;

  view->draw = monothek_credits_view_draw;
}

void
monothek_credits_view_connectable_interface_init(AgsConnectableInterface *connectable)
{
  monothek_credits_view_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->is_ready = NULL;
  connectable->is_connected = NULL;
  connectable->connect = monothek_credits_view_connect;
  connectable->disconnect = monothek_credits_view_disconnect;
}

void
monothek_credits_view_init(MonothekCreditsView *credits_view)
{
}

void
monothek_credits_view_set_property(GObject *gobject,
				  guint prop_id,
				  const GValue *value,
				  GParamSpec *param_spec)
{
  MonothekCreditsView *credits_view;

  credits_view = MONOTHEK_CREDITS_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_credits_view_get_property(GObject *gobject,
				  guint prop_id,
				  GValue *value,
				  GParamSpec *param_spec)
{
  MonothekCreditsView *credits_view;

  credits_view = MONOTHEK_CREDITS_VIEW(gobject);

  switch(prop_id){
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_credits_view_finalize(GObject *gobject)
{
  MonothekCreditsView *credits_view;

  credits_view = (MonothekCreditsView *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_credits_view_parent_class)->finalize(gobject);
}

void
monothek_credits_view_connect(AgsConnectable *connectable)
{
  MonothekCreditsView *credits_view;

  GList *list, *list_credits;

  credits_view = MONOTHEK_CREDITS_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(credits_view)->flags)) != 0){
    return;
  }

  monothek_credits_view_parent_connectable_interface->connect(connectable);

  //TODO:JK: implement me
}

void
monothek_credits_view_disconnect(AgsConnectable *connectable)
{
  MonothekCreditsView *credits_view;

  GList *list, *list_credits;

  credits_view = MONOTHEK_CREDITS_VIEW(connectable);

  if((MONOTHEK_VIEW_CONNECTED & (MONOTHEK_VIEW(credits_view)->flags)) == 0){
    return;
  }

  monothek_credits_view_parent_connectable_interface->disconnect(connectable);

  //TODO:JK: implement me
}

void
monothek_credits_view_draw(MonothekView *view)
{
  MonothekCreditsView *credits_view;
  
  cairo_t *cr;

  static const gdouble white_gc = 65535.0;

  credits_view = MONOTHEK_CREDITS_VIEW(view);
  
  /* call parent */
  MONOTHEK_VIEW_CLASS(monothek_credits_view_parent_class)->draw(view);

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

  //TODO:JK implement me
  
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
 * monothek_credits_view_new:
 *
 * Creates an #MonothekCreditsView
 *
 * Returns: a new #MonothekCreditsView
 *
 * Since: 1.0.0
 */
MonothekCreditsView*
monothek_credits_view_new()
{
  MonothekCreditsView *credits_view;

  credits_view = (MonothekCreditsView *) g_object_new(MONOTHEK_TYPE_CREDITS_VIEW,
						    NULL);
  
  return(credits_view);
}
