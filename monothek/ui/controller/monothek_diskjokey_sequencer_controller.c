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

#include <monothek/ui/controller/monothek_diskjokey_sequencer_controller.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#include <monothek/ui/model/monothek_diskjokey_sequencer_model.h>

#include <monothek/ui/view/monothek_diskjokey_sequencer_view.h>
#include <monothek/ui/view/monothek_diskjokey_end_view.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_diskjokey_sequencer_controller_class_init(MonothekDiskjokeySequencerControllerClass *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_connectable_interface_init(AgsConnectableInterface *connectable);
void monothek_diskjokey_sequencer_controller_init(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller);
void monothek_diskjokey_sequencer_controller_finalize(GObject *gobject);

void monothek_diskjokey_sequencer_controller_connect(AgsConnectable *connectable);
void monothek_diskjokey_sequencer_controller_disconnect(AgsConnectable *connectable);

/**
 * SECTION:monothek_diskjokey_sequencer_controller
 * @short_description: The diskjokey_sequencer controller object.
 * @title: MonothekDiskjokeySequencerController
 * @section_id:
 * @include: monothek/ui/controller/monothek_diskjokey_sequencer_controller.h
 *
 * #MonothekDiskjokeySequencerController is the MVC's diskjokey_sequencer controller.
 */

enum{
  LAST_SIGNAL,
};

static gpointer monothek_diskjokey_sequencer_controller_parent_class = NULL;
static AgsConnectableInterface* monothek_diskjokey_sequencer_controller_parent_connectable_interface;

static guint diskjokey_sequencer_controller_signals[LAST_SIGNAL];

GType
monothek_diskjokey_sequencer_controller_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_diskjokey_sequencer_controller = 0;

    static const GTypeInfo monothek_diskjokey_sequencer_controller_info = {
      sizeof (MonothekDiskjokeySequencerControllerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_diskjokey_sequencer_controller_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekDiskjokeySequencerController),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_diskjokey_sequencer_controller_init,
    };

    static const GInterfaceInfo monothek_connectable_interface_info = {
      (GInterfaceInitFunc) monothek_diskjokey_sequencer_controller_connectable_interface_init,
      NULL, /* interface_finalize */
      NULL, /* interface_data */
    };

    monothek_type_diskjokey_sequencer_controller = g_type_register_static(MONOTHEK_TYPE_CONTROLLER,
									"MonothekDiskjokeySequencerController", &monothek_diskjokey_sequencer_controller_info,
									0);

    g_type_add_interface_static(monothek_type_diskjokey_sequencer_controller,
				AGS_TYPE_CONNECTABLE,
				&monothek_connectable_interface_info);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_diskjokey_sequencer_controller);
  }

  return g_define_type_id__volatile;
}

void
monothek_diskjokey_sequencer_controller_class_init(MonothekDiskjokeySequencerControllerClass *diskjokey_sequencer_controller)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  monothek_diskjokey_sequencer_controller_parent_class = g_type_class_peek_parent(diskjokey_sequencer_controller);

  /* GObjectClass */
  gobject = (GObjectClass *) diskjokey_sequencer_controller;

  gobject->finalize = monothek_diskjokey_sequencer_controller_finalize;

  /* MonothekDiskjokeySequencerController */

  /* signals */
}

void
monothek_diskjokey_sequencer_controller_connectable_interface_init(AgsConnectableInterface *connectable)
{  
  monothek_diskjokey_sequencer_controller_parent_connectable_interface = g_type_interface_peek_parent(connectable);

  connectable->connect = monothek_diskjokey_sequencer_controller_connect;
  connectable->disconnect = monothek_diskjokey_sequencer_controller_disconnect;
}

void
monothek_diskjokey_sequencer_controller_init(MonothekDiskjokeySequencerController *diskjokey_sequencer_controller)
{
  //empty
}

void
monothek_diskjokey_sequencer_controller_finalize(GObject *gobject)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = (MonothekDiskjokeySequencerController *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_diskjokey_sequencer_controller_parent_class)->finalize(gobject);
}

void
monothek_diskjokey_sequencer_controller_connect(AgsConnectable *connectable)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER(connectable);

  if(monothek_controller_test_flags(diskjokey_sequencer_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_sequencer_controller_parent_connectable_interface->connect(connectable);
}

void
monothek_diskjokey_sequencer_controller_disconnect(AgsConnectable *connectable)
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = MONOTHEK_DISKJOKEY_SEQUENCER_CONTROLLER(connectable);

  if(!monothek_controller_test_flags(diskjokey_sequencer_controller, MONOTHEK_CONTROLLER_CONNECTED)){
    return;
  }

  monothek_diskjokey_sequencer_controller_parent_connectable_interface->disconnect(connectable);
}

/**
 * monothek_diskjokey_sequencer_controller_new:
 *
 * Creates an #MonothekDiskjokeySequencerController
 *
 * Returns: a new #MonothekDiskjokeySequencerController
 *
 * Since: 1.0.0
 */
MonothekDiskjokeySequencerController*
monothek_diskjokey_sequencer_controller_new()
{
  MonothekDiskjokeySequencerController *diskjokey_sequencer_controller;

  diskjokey_sequencer_controller = (MonothekDiskjokeySequencerController *) g_object_new(MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_CONTROLLER,
										     NULL);
  
  return(diskjokey_sequencer_controller);
}
