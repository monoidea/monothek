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

#include <monothek/ui/monothek_start_view.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

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

/**
 * SECTION:monothek_start_view
 * @short_description: The start_view object.
 * @title: MonothekStartView
 * @section_id:
 * @include: monothek/ui/monothek_start_view.h
 *
 * #MonothekStartView is a composite toplevel widget. It contains the
 * menubar, the machine rack and the notation editor.
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

  GParamSpec *param_spec;

  monothek_start_view_parent_class = g_type_class_peek_parent(start_view);

  /* GObjectClass */
  gobject = (GObjectClass *) start_view;

  gobject->set_property = monothek_start_view_set_property;
  gobject->get_property = monothek_start_view_get_property;

  gobject->finalize = monothek_start_view_finalize;

  /* properties */

  /* MonothekView */
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
  //TODO:JK: implement me
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
  MONOTHEK_VIEW_CLASS(monothek_start_view_parent_class)->draw(view);
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
