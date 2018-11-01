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

#ifndef __MONOTHEK_SESSION_MANAGER_H__
#define __MONOTHEK_SESSION_MANAGER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define MONOTHEK_TYPE_SESSION_MANAGER                (monothek_session_manager_get_type())
#define MONOTHEK_SESSION_MANAGER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_SESSION_MANAGER, MonothekSessionManager))
#define MONOTHEK_SESSION_MANAGER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_SESSION_MANAGER, MonothekSessionManagerClass))
#define MONOTHEK_IS_SESSION_MANAGER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_SESSION_MANAGER))
#define MONOTHEK_IS_SESSION_MANAGER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_SESSION_MANAGER))
#define MONOTHEK_SESSION_MANAGER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_SESSION_MANAGER, MonothekSessionManagerClass))

typedef struct _MonothekSessionManager MonothekSessionManager;
typedef struct _MonothekSessionManagerClass MonothekSessionManagerClass;

struct _MonothekSessionManager
{
  GObject gobject;
  
  GList *session;
};

struct _MonothekSessionManagerClass
{
  GObjectClass gobject;
};

GType monothek_session_manager_get_type();

void monothek_session_manager_add_session(MonothekSessionManager *session_manager,
					  GObject *session);
void monothek_session_manager_remove_session(MonothekSessionManager *session_manager,
					     GObject *session);

GObject* monothek_session_manager_find_session(MonothekSessionManager *session_manager,
					       gchar *session_id);

/* instance */
MonothekSessionManager* monothek_session_manager_get_instance();

MonothekSessionManager* monothek_session_manager_new();

#endif /*__MONOTHEK_SESSION_MANAGER_H__*/
