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

#include <monothek/session/monothek_session_manager.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/session/monothek_session.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_session_manager_class_init(MonothekSessionManagerClass *session_manager);
void monothek_session_manager_init(MonothekSessionManager *session_manager);
void monothek_session_manager_set_property(GObject *gobject,
					   guint prop_id,
					   const GValue *value,
					   GParamSpec *param_spec);
void monothek_session_manager_get_property(GObject *gobject,
					   guint prop_id,
					   GValue *value,
					   GParamSpec *param_spec);
void monothek_session_manager_finalize(GObject *gobject);

/**
 * SECTION:monothek_session_manager
 * @short_description: The session manager
 * @title: MonothekSessionManager
 * @section_id:
 * @include: monothek/session/monothek_session_manager.h
 *
 * #MonothekSessionManager stores the sessions.
 */

enum{
  PROP_0,
  PROP_SESSION,
};

static gpointer monothek_session_manager_parent_class = NULL;

MonothekSessionManager *monothek_session_manager = NULL;

GType
monothek_session_manager_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_session_manager = 0;

    static const GTypeInfo monothek_session_manager_info = {
      sizeof (MonothekSessionManagerClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_session_manager_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekSessionManager),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_session_manager_init,
    };

    monothek_type_session_manager = g_type_register_static(G_TYPE_OBJECT,
							   "MonothekSessionManager", &monothek_session_manager_info,
							   0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_session_manager);
  }

  return g_define_type_id__volatile;
}

void
monothek_session_manager_class_init(MonothekSessionManagerClass *session_manager)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_session_manager_parent_class = g_type_class_peek_parent(session_manager);

  /* GObjectClass */
  gobject = (GObjectClass *) session_manager;

  gobject->set_property = monothek_session_manager_set_property;
  gobject->get_property = monothek_session_manager_get_property;

  gobject->finalize = monothek_session_manager_finalize;

  /* properties */
  /**
   * MonothekSessionManager:session:
   *
   * The containing #MonothekSession.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_pointer("session",
				    i18n_pspec("containing session"),
				    i18n_pspec("The session it contains"),
				    G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_SESSION,
				  param_spec);

  /* MonothekModel */
}

void
monothek_session_manager_init(MonothekSessionManager *session_manager)
{
  session_manager->session = NULL;
}

void
monothek_session_manager_set_property(GObject *gobject,
				      guint prop_id,
				      const GValue *value,
				      GParamSpec *param_spec)
{
  MonothekSessionManager *session_manager;

  session_manager = MONOTHEK_SESSION_MANAGER(gobject);

  switch(prop_id){
  case PROP_SESSION:
    {
      MonothekSession *session;
      
      /*  */
      session = (MonothekSession *) g_value_get_pointer(value);

      monothek_session_manager_add_session(session_manager,
					   (GObject *) session);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_session_manager_get_property(GObject *gobject,
				      guint prop_id,
				      GValue *value,
				      GParamSpec *param_spec)
{
  MonothekSessionManager *session_manager;

  session_manager = MONOTHEK_SESSION_MANAGER(gobject);

  switch(prop_id){
  case PROP_SESSION:
    {      
      g_value_set_pointer(value,
			  g_list_copy(session_manager->session));
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_session_manager_finalize(GObject *gobject)
{
  MonothekSessionManager *session_manager;

  session_manager = (MonothekSessionManager *) gobject;
  
  /* call parent */
  G_OBJECT_CLASS(monothek_session_manager_parent_class)->finalize(gobject);
}

/**
 * monothek_session_manager_add_session:
 * @session_manager: the #MonothekSessionManager
 * @session: the #MonothekSession
 * 
 * Add @session to @session_manager.
 * 
 * Since: 1.0.0
 */
void
monothek_session_manager_add_session(MonothekSessionManager *session_manager,
				     GObject *session)
{
  if(!MONOTHEK_IS_SESSION_MANAGER(session_manager) ||
     !MONOTHEK_IS_SESSION(session)){
    return;
  }

  if(g_list_find(session_manager->session,
		 session) == NULL){
    g_object_ref(session);
    session_manager->session = g_list_prepend(session_manager->session,
					      session);
  }
}

/**
 * monothek_session_manager_remove_session:
 * @session_manager: the #MonothekSessionManager
 * @session: the #MonothekSession
 * 
 * Remove @session from @session_manager.
 * 
 * Since: 1.0.0
 */
void
monothek_session_manager_remove_session(MonothekSessionManager *session_manager,
					GObject *session)
{
  if(!MONOTHEK_IS_SESSION_MANAGER(session_manager) ||
     !MONOTHEK_IS_SESSION(session)){
    return;
  }

  if(g_list_find(session_manager->session,
		 session) != NULL){
    g_object_unref(session);
    session_manager->session = g_list_remove(session_manager->session,
					     session);
  }
}

/**
 * monothek_session_manager_find_session:
 * @session_manager: the #MonothekSessionManager
 * @session_id: the string identifier
 * 
 * Find session by @session_id.
 *
 * Returns: the matching #MonothekSession if found, otherwise %NULL
 * 
 * Since: 1.0.0
 */
GObject*
monothek_session_manager_find_session(MonothekSessionManager *session_manager,
				      gchar *session_id)
{
  MonothekSession *session;
  
  GList *start_list, *list;
  
  if(!MONOTHEK_IS_SESSION_MANAGER(session_manager) ||
     session_id == NULL){
    return(NULL);
  }

  g_object_get(session_manager,
	       "session", &start_list,
	       NULL);

  session = NULL;
  
  list = start_list;
  
  while(list != NULL){
    if(!g_ascii_strcasecmp(MONOTHEK_SESSION(list->data)->session_id,
			   session_id)){
      session = list->data;
      
      break;
    }
    
    list = list->next;
  }

  g_list_free(start_list);
  
  return(session);
}

/**
 * monothek_session_manager_get_instance:
 * 
 * Get monothek session manager instance.
 *
 * Returns: the #MonothekSessionManager singleton
 * 
 * Since: 1.0.0
 */
MonothekSessionManager*
monothek_session_manager_get_instance()
{
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock(&mutex);

  if(monothek_session_manager == NULL){
    monothek_session_manager = monothek_session_manager_new();
  }
  
  pthread_mutex_unlock(&mutex);

  return(monothek_session_manager);
}

/**
 * monothek_session_manager_new:
 *
 * Creates an #MonothekSessionManager
 *
 * Returns: a new #MonothekSessionManager
 *
 * Since: 1.0.0
 */
MonothekSessionManager*
monothek_session_manager_new()
{
  MonothekSessionManager *session_manager;

  session_manager = (MonothekSessionManager *) g_object_new(MONOTHEK_TYPE_SESSION_MANAGER,
							    NULL);
  
  return(session_manager);
}
