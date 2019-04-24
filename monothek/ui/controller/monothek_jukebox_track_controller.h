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

#ifndef __MONOTHEK_JUKEBOX_TRACK_CONTROLLER_H__
#define __MONOTHEK_JUKEBOX_TRACK_CONTROLLER_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/controller/monothek_controller.h>

#include <time.h>

#define MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER                (monothek_jukebox_track_controller_get_type())
#define MONOTHEK_JUKEBOX_TRACK_CONTROLLER(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER, MonothekJukeboxTrackController))
#define MONOTHEK_JUKEBOX_TRACK_CONTROLLER_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER, MonothekJukeboxTrackControllerClass))
#define MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER))
#define MONOTHEK_IS_JUKEBOX_TRACK_CONTROLLER_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER))
#define MONOTHEK_JUKEBOX_TRACK_CONTROLLER_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_JUKEBOX_TRACK_CONTROLLER, MonothekJukeboxTrackControllerClass))

#define MONOTHEK_JUKEBOX_TRACK_CONTROLLER_TEST_TIME_SEC (30)

typedef struct _MonothekJukeboxTrackController MonothekJukeboxTrackController;
typedef struct _MonothekJukeboxTrackControllerClass MonothekJukeboxTrackControllerClass;

struct _MonothekJukeboxTrackController
{
  MonothekController controller;

  MonothekActionBox *jukebox_play;
  MonothekActionBox *jukebox_test;
  MonothekActionBox *jukebox_back;

  struct timespec *start_time;
  struct timespec *timer;
};

struct _MonothekJukeboxTrackControllerClass
{
  MonothekControllerClass controller;

  void (*test)(MonothekJukeboxTrackController *jukebox_track_controller);
  void (*play)(MonothekJukeboxTrackController *jukebox_track_controller);
  void (*back)(MonothekJukeboxTrackController *jukebox_track_controller);

  void (*progress)(MonothekJukeboxTrackController *jukebox_track_controller,
		   gdouble value);

  void (*test_time_expired)(MonothekJukeboxTrackController *jukebox_track_controller);

  void (*run)(MonothekJukeboxTrackController *jukebox_track_controller,
	      gboolean do_run);

  void (*completed)(MonothekJukeboxTrackController *jukebox_track_controller);
};

GType monothek_jukebox_track_controller_get_type(void);

void monothek_jukebox_track_controller_test(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_play(MonothekJukeboxTrackController *jukebox_track_controller);
void monothek_jukebox_track_controller_back(MonothekJukeboxTrackController *jukebox_track_controller);

void monothek_jukebox_track_controller_progress(MonothekJukeboxTrackController *jukebox_track_controller,
						gdouble value);

void monothek_jukebox_track_controller_test_time_expired(MonothekJukeboxTrackController *jukebox_track_controller);

void monothek_jukebox_track_controller_run(MonothekJukeboxTrackController *jukebox_track_controller,
					   gboolean do_run);

void monothek_jukebox_track_controller_completed(MonothekJukeboxTrackController *jukebox_track_controller);

gboolean monothek_jukebox_track_controller_progress_increase_timeout(GObject *gobject);

MonothekJukeboxTrackController* monothek_jukebox_track_controller_new();

#endif /*__MONOTHEK_JUKEBOX_TRACK_CONTROLLER_H__*/
