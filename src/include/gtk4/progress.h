/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * Copyright (C) 2025 Perry Werneck <perry.werneck@gmail.com>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

 // References: https://docs.gtk.org/gobject/tutorial.html

 #pragma once

 #include <gtk/gtk.h>

 G_BEGIN_DECLS

 #define PW_TYPE_PROGRESS pw_progress_get_type()
 G_DECLARE_FINAL_TYPE (PwProgress, pw_progress, PW, PROGRESS, GtkGrid)

 PwProgress * pw_progress_new();

 void pw_progress_set_progress(PwProgress *progress, uint64_t current, uint64_t total);
 void pw_progress_set_text(PwProgress *progress, const char *text);
 void pw_progress_set_pulse(PwProgress *progress, gboolean pulse);
 void pw_progress_set_step(PwProgress *progress, unsigned int current, unsigned int total);
 void pw_progress_set_steps(PwProgress *progress, unsigned int steps);
 void pw_progress_next_step(PwProgress *progress);
 
 G_END_DECLS
 