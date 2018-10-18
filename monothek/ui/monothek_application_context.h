/* Monothek - monoidea's monothek
 * Copyright (C) 2018 Joël Krähemann
 *
 * This file is part of Monothek.
 *
 * GSequencer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GSequencer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GSequencer.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MONOTHEK_APPLICATION_CONTEXT_H__
#define __MONOTHEK_APPLICATION_CONTEXT_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>
#include <ags/libags-gui.h>

#define AGS_TYPE_XORG_APPLICATION_CONTEXT                (monothek_application_context_get_type())
#define MONOTHEK_APPLICATION_CONTEXT(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), AGS_TYPE_XORG_APPLICATION_CONTEXT, MonothekApplicationContext))
#define MONOTHEK_APPLICATION_CONTEXT_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, AGS_TYPE_XORG_APPLICATION_CONTEXT, MonothekApplicationContextClass))
#define AGS_IS_XORG_APPLICATION_CONTEXT(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AGS_TYPE_XORG_APPLICATION_CONTEXT))
#define AGS_IS_XORG_APPLICATION_CONTEXT_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), AGS_TYPE_XORG_APPLICATION_CONTEXT))
#define MONOTHEK_APPLICATION_CONTEXT_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, AGS_TYPE_XORG_APPLICATION_CONTEXT, MonothekApplicationContextClass))

#define MONOTHEK_VERSION "1.0.0"
#define MONOTHEK_BUILD_ID "Wed Oct 17 18:17:09 UTC 2018"

typedef struct _MonothekApplicationContext MonothekApplicationContext;
typedef struct _MonothekApplicationContextClass MonothekApplicationContextClass;

typedef enum{
  MONOTHEK_APPLICATION_CONTEXT_SHOW_GUI      = 1,
}MonothekApplicationContextFlags;

struct _MonothekApplicationContext
{
  AgsApplicationContext application_context;
  
  AgsThreadPool *thread_pool;

  AgsPollingThread *polling_thread;

  GList *worker;
  
  AgsThread *soundcard_thread;
  AgsThread *export_thread;
  
  GList *soundcard;
  GList *sequencer;

  GList *sound_server;
  
  GList *audio;

  MonothekWindow *window;
};

struct _MonothekApplicationContextClass
{
  AgsApplicationContextClass application_context;
};

GType monothek_application_context_get_type();

MonothekApplicationContext* monothek_application_context_new();

#endif /*__MONOTHEK_APPLICATION_CONTEXT_H__*/
