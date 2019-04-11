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

#ifndef __MONOTHEK_EXPORT_THREAD_H__
#define __MONOTHEK_EXPORT_THREAD_H__

#include <glib.h>
#include <glib-object.h>

#include <ags/libags.h>
#include <ags/libags-audio.h>

#define MONOTHEK_TYPE_EXPORT_THREAD                (monothek_export_thread_get_type())
#define MONOTHEK_EXPORT_THREAD(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_EXPORT_THREAD, MonothekExportThread))
#define MONOTHEK_EXPORT_THREAD_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST(class, MONOTHEK_TYPE_EXPORT_THREAD, MonothekExportThreadClass))
#define MONOTHEK_IS_EXPORT_THREAD(obj)             (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MONOTHEK_TYPE_EXPORT_THREAD))
#define MONOTHEK_IS_EXPORT_THREAD_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE ((class), MONOTHEK_TYPE_EXPORT_THREAD))
#define MONOTHEK_EXPORT_THREAD_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS(obj, MONOTHEK_TYPE_EXPORT_THREAD, MonothekExportThreadClass))

typedef struct _MonothekExportThread MonothekExportThread;
typedef struct _MonothekExportThreadClass MonothekExportThreadClass;

struct _MonothekExportThread
{
  AgsExportThreadClass export_thread;

  struct timespec *start_time;
  struct timespec *duration;
};

struct _MonothekExportThreadClass
{
  AgsExportThreadClass export_thread;
};

GType monothek_export_thread_get_type();

MonothekExportThread* monothek_export_thread_new(GObject *soundcard, AgsAudioFile *audio_file);

#endif /*__MONOTHEK_EXPORT_THREAD_H__*/
