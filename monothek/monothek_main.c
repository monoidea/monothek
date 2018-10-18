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

#include <glib.h>
#include <glib-object.h>

#include <gdk/gdk.h>
#include <pango/pangocairo.h>

#include <gtk/gtk.h>

#define _GNU_SOURCE
#include <locale.h>

#include <monothek/ui/monothek_application_context.h>

#include "monothek_main.h"

#include "config.h"

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
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  gtk_init(&argc, &argv);

  monothek_setup(argc, argv);

  return(0);
}
