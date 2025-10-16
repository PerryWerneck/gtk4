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

 #include <config.h>
 #include <gtk4/progress.h>
 #include <gtk/gtk.h>

 #if !GTK_CHECK_VERSION(2,74,0)
 	#define G_APPLICATION_DEFAULT_FLAGS 0
 #endif 

 /*---[ Implement ]----------------------------------------------------------------------------------*/


 static void activate(GtkApplication *app, gpointer user_data) {

	GtkWidget *window;

    // Create a new window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Hello World");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 150);

    // Create a label widget
    PwProgress *progress = pw_progress_new();

    // Add the label to the window
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(progress));

    // Show the window
    gtk_window_present(GTK_WINDOW(window));

 }

 int main (int argc, char **argv) {

	GtkApplication *app;
    int status;

    // Create a new GtkApplication instance
    app = gtk_application_new("br.eti.werneck.widgets", G_APPLICATION_DEFAULT_FLAGS);

    // Connect the "activate" signal to the activate function
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Run the application
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Release the application instance
    g_object_unref(app);

    return status;

 }


