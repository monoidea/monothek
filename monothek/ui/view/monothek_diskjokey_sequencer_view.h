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

#ifndef __MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_H__
#define __MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_H__

#include <glib.h>
#include <glib-object.h>

#include <gtk/gtk.h>

#include <monothek/ui/view/monothek_view.h>

#define MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW                (monothek_diskjokey_sequencer_view_get_type())
#define MONOTHEK_DISKJOKEY_SEQUENCER_VIEW(obj)                (G_TYPE_CHECK_INSTANCE_CAST((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW, MonothekDiskjokeySequencerView))
#define MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_CLASS(class)        (G_TYPE_CHECK_CLASS_CAST((class), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW, MonothekDiskjokeySequencerViewClass))
#define MONOTHEK_IS_DISKJOKEY_SEQUENCER_VIEW(obj)             (G_TYPE_CHECK_INSTANCE_TYPE((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW))
#define MONOTHEK_IS_DISKJOKEY_SEQUENCER_VIEW_CLASS(class)     (G_TYPE_CHECK_CLASS_TYPE((class), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW))
#define MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS((obj), MONOTHEK_TYPE_DISKJOKEY_SEQUENCER_VIEW, MonothekDiskjokeySequencerViewClass))

#define MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_COLUMN_COUNT (16)
#define MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_PATTERN_ROW_COUNT (9)

typedef struct _MonothekDiskjokeySequencerView MonothekDiskjokeySequencerView;
typedef struct _MonothekDiskjokeySequencerViewClass MonothekDiskjokeySequencerViewClass;

struct _MonothekDiskjokeySequencerView
{
  MonothekView monothek_view;

  gdouble active_column_line_width;

  //pattern
  gdouble pattern_line_width;

  gdouble pattern_x0;
  gdouble pattern_y0;

  gdouble pattern_column_spacing;
  gdouble pattern_row_spacing;

  gdouble pad_width;
  gdouble pad_height;

  //tab
  gdouble tab_box_line_width[4];

  gdouble tab_box_x0[4];
  gdouble tab_box_y0[4];

  gdouble tab_box_width[4];
  gdouble tab_box_height[4];

  //bpm
  gdouble bpm_box_line_width;

  gdouble bpm_box_x0;
  gdouble bpm_box_y0;

  gdouble bpm_box_width;
  gdouble bpm_box_height;

  gdouble bpm_control_line_width;

  gdouble bpm_control_x0;
  gdouble bpm_control_y0;

  gdouble bpm_control_width;
  gdouble bpm_control_height;
  
  //swing
  gdouble swing_box_line_width;

  gdouble swing_box_x0;
  gdouble swing_box_y0;

  gdouble swing_box_width;
  gdouble swing_box_height;

  gdouble swing_control_line_width;

  gdouble swing_control_x0;
  gdouble swing_control_y0;

  gdouble swing_control_width;
  gdouble swing_control_height;
  
  //techno
  gdouble techno_box_line_width;

  gdouble techno_box_x0;
  gdouble techno_box_y0;

  gdouble techno_box_width;
  gdouble techno_box_height;

  //house
  gdouble house_box_line_width;

  gdouble house_box_x0;
  gdouble house_box_y0;

  gdouble house_box_width;
  gdouble house_box_height;

  //hiphop
  gdouble hiphop_box_line_width;

  gdouble hiphop_box_x0;
  gdouble hiphop_box_y0;

  gdouble hiphop_box_width;
  gdouble hiphop_box_height;

  //random
  gdouble random_box_line_width;

  gdouble random_box_x0;
  gdouble random_box_y0;

  gdouble random_box_width;
  gdouble random_box_height;

  //clear
  gdouble clear_box_line_width;

  gdouble clear_box_x0;
  gdouble clear_box_y0;

  gdouble clear_box_width;
  gdouble clear_box_height;

  //timer
  gdouble timer_box_line_width;

  gdouble timer_box_x0;
  gdouble timer_box_y0;

  gdouble timer_box_width;
  gdouble timer_box_height;

  //stop
  gdouble stop_box_line_width;

  gdouble stop_box_x0;
  gdouble stop_box_y0;

  gdouble stop_box_width;
  gdouble stop_box_height;
};

struct _MonothekDiskjokeySequencerViewClass
{
  MonothekViewClass monothek_view;
};

GType monothek_diskjokey_sequencer_view_get_type(void);

MonothekDiskjokeySequencerView* monothek_diskjokey_sequencer_view_new();

#endif /*__MONOTHEK_DISKJOKEY_SEQUENCER_VIEW_H__*/
