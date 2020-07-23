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

#ifndef __MONOTHEK_APPLICATION_CONTEXT_H__
#define __MONOTHEK_APPLICATION_CONTEXT_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#include <monothek/ui/monothek_window.h>

#define MONOTHEK_TYPE_APPLICATION_CONTEXT                (monothek_application_context_get_type())
#define MONOTHEK_APPLICATION_CONTEXT(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_APPLICATION_CONTEXT, MonothekApplicationContext))
#define MONOTHEK_APPLICATION_CONTEXT_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_APPLICATION_CONTEXT, MonothekApplicationContextClass))
#define MONOTHEK_IS_APPLICATION_CONTEXT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_APPLICATION_CONTEXT))
#define MONOTHEK_IS_APPLICATION_CONTEXT_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_APPLICATION_CONTEXT))
#define MONOTHEK_APPLICATION_CONTEXT_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_APPLICATION_CONTEXT, MonothekApplicationContextClass))

#define MONOTHEK_VERSION "2.0.0"
#define MONOTHEK_BUILD_ID "Thu Jul 23 22:23:31 UTC 2020"

#define MONOTHEK_DEFAULT_DIRECTORY ".monothek"
#define MONOTHEK_DEFAULT_CONFIG "monothek.conf"
  
typedef struct _MonothekApplicationContext MonothekApplicationContext;
typedef struct _MonothekApplicationContextClass MonothekApplicationContextClass;

typedef enum{
  MONOTHEK_APPLICATION_CONTEXT_SHOW_GUI      = 1,
}MonothekApplicationContextFlags;

struct _MonothekApplicationContext
{
  AgsApplicationContext application_context;

  guint flags;
  
  AgsThreadPool *thread_pool;

  GList *worker;

  GMainContext *server_main_context;

  gboolean is_operating;

  AgsServerStatus *server_status;
  
  AgsRegistry *registry;
  
  GList *server;

  GMainContext *audio_main_context;
  GMainContext *osc_server_main_context;
  
  GObject *default_soundcard;

  AgsThread *default_soundcard_thread;
  AgsThread *default_export_thread;
  
  GList *soundcard;
  GList *sequencer;

  GList *audio;

  GList *sound_server;

  GList *osc_server;

  gboolean gui_ready;
  gboolean show_animation;
  gboolean file_ready;

  gdouble gui_scale_factor;
  
  GList *task;
  
  MonothekWindow *window;
};

struct _MonothekApplicationContextClass
{
  AgsApplicationContextClass application_context;
};

GType monothek_application_context_get_type();

gboolean monothek_application_context_message_monitor_timeout(MonothekApplicationContext *monothek_application_context);
gboolean monothek_application_context_task_timeout(MonothekApplicationContext *monothek_application_context);

MonothekApplicationContext* monothek_application_context_new();

#endif /*__MONOTHEK_APPLICATION_CONTEXT_H__*/
