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

#include <monothek/session/monothek_session.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <stdlib.h>

#include <monothek/i18n.h>

void monothek_session_class_init(MonothekSessionClass *session);
void monothek_session_init(MonothekSession *session);
void monothek_session_set_property(GObject *gobject,
				   guint prop_id,
				   const GValue *value,
				   GParamSpec *param_spec);
void monothek_session_get_property(GObject *gobject,
				   guint prop_id,
				   GValue *value,
				   GParamSpec *param_spec);
void monothek_session_finalize(GObject *gobject);

/**
 * SECTION:monothek_session
 * @short_description: The session object
 * @title: MonothekSession
 * @section_id:
 * @include: monothek/session/monothek_session.h
 *
 * #MonothekSession stores data.
 */

enum{
  PROP_0,
  PROP_SESSION_ID,
};

static gpointer monothek_session_parent_class = NULL;

GType
monothek_session_get_type()
{
  static volatile gsize g_define_type_id__volatile = 0;

  if(g_once_init_enter (&g_define_type_id__volatile)){
    GType monothek_type_session = 0;

    static const GTypeInfo monothek_session_info = {
      sizeof (MonothekSessionClass),
      NULL, /* base_init */
      NULL, /* base_finalize */
      (GClassInitFunc) monothek_session_class_init,
      NULL, /* class_finalize */
      NULL, /* class_data */
      sizeof (MonothekSession),
      0,    /* n_preallocs */
      (GInstanceInitFunc) monothek_session_init,
    };

    monothek_type_session = g_type_register_static(G_TYPE_OBJECT,
						   "MonothekSession", &monothek_session_info,
						   0);

    g_once_init_leave(&g_define_type_id__volatile, monothek_type_session);
  }

  return g_define_type_id__volatile;
}

void
monothek_session_class_init(MonothekSessionClass *session)
{
  GObjectClass *gobject;
  GtkWidgetClass *widget;

  GParamSpec *param_spec;

  monothek_session_parent_class = g_type_class_peek_parent(session);

  /* GObjectClass */
  gobject = (GObjectClass *) session;

  gobject->set_property = monothek_session_set_property;
  gobject->get_property = monothek_session_get_property;

  gobject->finalize = monothek_session_finalize;

  /* properties */
  /**
   * MonothekSession:session-id:
   *
   * The assigned session id.
   * 
   * Since: 1.0.0
   */
  param_spec = g_param_spec_string("session-id",
				   i18n_pspec("session id"),
				   i18n_pspec("The assigned session id"),
				   NULL,
				   G_PARAM_READABLE | G_PARAM_WRITABLE);
  g_object_class_install_property(gobject,
				  PROP_SESSION_ID,
				  param_spec);

  /* MonothekModel */
}

void
monothek_session_init(MonothekSession *session)
{
  GValue *value;
  
  session->session_id = NULL;

  session->value = g_hash_table_new_full(g_direct_hash,
					 g_str_equal,
					 NULL,
					 NULL);
  g_hash_table_ref(session->value);

  /* preserve jukebox */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_BOOLEAN);

  g_value_set_boolean(value, TRUE);

  g_hash_table_insert(session->value,
		      "preserve-jukebox", value);

  /* jukebox mode */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_STRING);

  g_value_set_string(value,
		     "test");

  g_hash_table_insert(session->value,
		      "jukebox-mode", value);

  /* jukebox song filename */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_STRING);

  g_value_set_string(value,
		     NULL);

  g_hash_table_insert(session->value,
		      "jukebox-song-filename", value);

  /* jukebox test count */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_UINT);

  g_value_set_uint(value, 0);
  
  g_hash_table_insert(session->value,
		      "jukebox-test-count", value);
  
  /* preserve diskjokey */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_BOOLEAN);

  g_value_set_boolean(value, TRUE);

  g_hash_table_insert(session->value,
		      "preserve-diskjokey", value);
}

void
monothek_session_set_property(GObject *gobject,
			      guint prop_id,
			      const GValue *value,
			      GParamSpec *param_spec)
{
  MonothekSession *session;

  session = MONOTHEK_SESSION(gobject);

  switch(prop_id){
  case PROP_SESSION_ID:
    {
      gchar *session_id;

      session_id = (gchar *) g_value_get_string(value);

      if(session_id == session->session_id){
	return;
      }

      if(session->session_id != NULL){
	g_free(session->session_id);
      }
      
      session->session_id = g_strdup(session_id);
    }
    break; 
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_session_get_property(GObject *gobject,
			      guint prop_id,
			      GValue *value,
			      GParamSpec *param_spec)
{
  MonothekSession *session;

  session = MONOTHEK_SESSION(gobject);

  switch(prop_id){
  case PROP_SESSION_ID:
    {
      g_value_set_string(value, session->session_id);
    }
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, prop_id, param_spec);
    break;
  }
}

void
monothek_session_finalize(GObject *gobject)
{
  MonothekSession *session;

  session = (MonothekSession *) gobject;

  g_free(session->session_id);

  if(session->value != NULL){
    g_hash_table_unref(session->value);
  }
  
  /* call parent */
  G_OBJECT_CLASS(monothek_session_parent_class)->finalize(gobject);
}

/**
 * monothek_session_new:
 *
 * Creates an #MonothekSession
 *
 * Returns: a new #MonothekSession
 *
 * Since: 1.0.0
 */
MonothekSession*
monothek_session_new()
{
  MonothekSession *session;

  session = (MonothekSession *) g_object_new(MONOTHEK_TYPE_SESSION,
					     NULL);
  
  return(session);
}

