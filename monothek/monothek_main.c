/* Monothek - monoidea's monothek
 * Copyright (C) 2018-2020 Joël Krähemann
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

#include <glib.h>
#include <glib-object.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <gdk/gdk.h>
#include <pango/pangocairo.h>

#include <gtk/gtk.h>

#include <stdlib.h>

#define _GNU_SOURCE
#include <locale.h>

#include <unistd.h>
#include <sys/types.h>

#if !defined(AGS_W32API)
#include <pwd.h>
#endif

#include <monothek/session/monothek_session_manager.h>
#include <monothek/session/monothek_session.h>

#include <monothek/audio/monothek_rack.h>

#include <monothek/ui/monothek_application_context.h>

#include "config.h"
#include "monothek_main.h"

void*
monothek_setup_thread(void *ptr)
{
  MonothekApplicationContext *application_context;
  
  application_context = (MonothekApplicationContext *) ptr;

  while(g_atomic_int_get(&(application_context->gui_ready)) == 0){
    usleep(500000);
  }

  //  pthread_mutex_lock(monothek_gui_thread_get_dispatch_mutex());
  
  ags_application_context_setup(application_context);

  //  pthread_mutex_unlock(monothek_gui_thread_get_dispatch_mutex());
  
  pthread_exit(NULL);

  return(NULL);
}

void
monothek_setup(int argc, char **argv)
{
  MonothekApplicationContext *application_context;
  AgsLog *log;

  pthread_t thread;

  /* application context */
  application_context = (MonothekApplicationContext *) monothek_application_context_new();
  g_object_ref(application_context);
  
  AGS_APPLICATION_CONTEXT(application_context)->argc = argc;
  AGS_APPLICATION_CONTEXT(application_context)->argv = argv;

  log = ags_log_get_instance();

  ags_log_add_message(log,
		      "Welcome to Monothek");
  
  /* application context */
  pthread_create(&thread, NULL,
		 monothek_setup_thread, application_context);
  
  ags_application_context_prepare(application_context);
}

int
main(int argc, char **argv)
{
  MonothekRack *rack;

  MonothekSessionManager *session_manager;
  MonothekSession *session;

  AgsConfig *config;

#if defined AGS_W32API
  gchar *app_dir;
  gchar *path;
#endif

#if !defined(AGS_W32API)
  struct passwd *pw;

  uid_t uid;
#endif

  gchar *wdir, *config_file;

  gboolean has_file;

  GValue *value;

  putenv("TZ=UTC");
  
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  config = NULL;

  gtk_init(&argc, &argv);

  /* find session */
  session_manager = monothek_session_manager_get_instance();

  session = monothek_session_new();
  g_object_set(session,
	       "session-id", MONOTHEK_SESSION_DEFAULT_SESSION,
	       NULL);
  monothek_session_manager_add_session(session_manager,
				       session);

  /* rack */
  rack = monothek_rack_new(NULL);
  
  value = g_new0(GValue,
		      1);
  g_value_init(value,
	       G_TYPE_OBJECT);

  g_value_set_object(value,
		     rack);
  
  g_hash_table_insert(session->value,
		      "rack",
		      value);
  
  /* preserve jukebox */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_BOOLEAN);

  g_value_set_boolean(value,
		      TRUE);

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

  g_value_set_uint(value,
		   0);
  
  g_hash_table_insert(session->value,
		      "jukebox-test-count", value);
  
  /* preserve diskjokey */
  value = g_new0(GValue,
		 1);
  g_value_init(value,
	       G_TYPE_BOOLEAN);

  g_value_set_boolean(value,
		      TRUE);

  g_hash_table_insert(session->value,
		      "preserve-diskjokey", value);
  
  has_file = FALSE;

  /* setup */
  if(!has_file){
#ifdef MONOTHEK_W32API
    app_dir = NULL;

    if(strlen(argv[0]) > strlen("\\monothek-ui.exe")){
      app_dir = g_strndup(argv[0],
			  strlen(argv[0]) - strlen("\\monothek-ui.exe"));
    }
  
    path = g_strdup_printf("%s\\etc\\monothek",
			   g_get_current_dir());
    
    if(!g_file_test(path,
		    G_FILE_TEST_IS_DIR)){
      g_free(path);

      if(g_path_is_absolute(app_dir)){
	path = g_strdup_printf("%s\\%s",
			       app_dir,
			       "\\etc\\monothek");
      }else{
	path = g_strdup_printf("%s\\%s\\%s",
			       g_get_current_dir(),
			       app_dir,
			       "\\etc\\monothek");
      }
    }
    
    config_file = g_strdup_printf("%s\\%s",
				  path,
				  "monothek.conf");

    g_free(path);
#else
    uid = getuid();
    pw = getpwuid(uid);

    wdir = g_strdup_printf("%s/%s",
			   pw->pw_dir,
			   AGS_DEFAULT_DIRECTORY);
    
    config_file = g_strdup_printf("%s/%s",
				  wdir,
				  "monothek.conf");

    g_free(wdir);
#endif

    config = ags_config_get_instance();

    ags_config_load_from_file(config,
			      config_file);

    g_free(config_file);
  }
  
  monothek_setup(argc, argv);

  return(0);
}
