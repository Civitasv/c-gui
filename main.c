#include <gtk/gtk.h>
#include <glib/gstdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sudo.h"
#include "shuffle.h"

#define BLOCK_SIZE 70

GtkWidget *entries[N][N];
GtkWidget *grid;
GtkFixed *fixed;
int activate_entry_row, activate_entry_col;
bool is_label[N][N];

static void
gtk_css_set(GtkWidget *g_widget);

static void
hint(GtkWidget *widget,
     gpointer user_data);

static void
check(GtkWidget *widget,
      gpointer user_data);

static void
new_game(GtkWidget *widget,
         gpointer user_data);

static void
entry_activate(GtkWidget *widget,
               GdkEvent *event,
               gpointer user_data);

static void
entry_changed(GtkWidget *widget,
              gpointer user_data);

static void
entry_insert_text(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data);

static void
activate(GtkApplication *app,
         gpointer user_data);

static void
init_game();

static void
gtk_css_set(GtkWidget *g_widget)
{
    GtkCssProvider *provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(provider,
                                    "./sudo.css", NULL);
    GtkStyleContext *context = gtk_widget_get_style_context(g_widget);

    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(provider);
}

static void
hint(GtkWidget *widget,
     gpointer user_data)
{
    if (!is_label[activate_entry_row][activate_entry_col])
    {
        // g_print("%d %d\n", activate_entry_row, activate_entry_col);

        int answer = sudos_answer[activate_entry_row][activate_entry_col];
        char str[2];
        sprintf(str, "%d", answer);
        GtkWidget *entry = entries[activate_entry_row][activate_entry_col];

        // g_print("%s\n", str);
        gtk_entry_set_text(GTK_ENTRY(entry), str);
    }
}

static void
check(GtkWidget *widget,
      gpointer user_data)
{
    bool if_success = sudos_check();
    GtkWidget *dialog;
    if (if_success)
    {
        dialog = gtk_message_dialog_new(NULL,
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_INFO,
                                        GTK_BUTTONS_OK,
                                        "\nYOU WIN!\n");
        gtk_window_set_title(GTK_WINDOW(dialog), "success");
        gtk_widget_set_size_request(GTK_WIDGET(dialog), 200, 200);
    }
    else
    {
        dialog = gtk_message_dialog_new(NULL,
                                        GTK_DIALOG_MODAL,
                                        GTK_MESSAGE_ERROR,
                                        GTK_BUTTONS_OK,
                                        "\nKEEP IT UP!\n");
        gtk_window_set_title(GTK_WINDOW(dialog), "fail");
        gtk_widget_set_size_request(GTK_WIDGET(dialog), 200, 200);
    }
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

static void
new_game(GtkWidget *widget,
         gpointer user_data)
{
    init_game();
}

static void
entry_activate(GtkWidget *widget,
               GdkEvent *event,
               gpointer user_data)
{
    int *data = user_data;
    int i = *data, j = *(data + 1);

    activate_entry_row = i;
    activate_entry_col = j;
}

static void
entry_changed(GtkWidget *widget,
              gpointer user_data)
{
    gint number = atoi(gtk_entry_get_text(GTK_ENTRY(widget)));
    sudos_add(number, activate_entry_row, activate_entry_col);
}

static void
entry_insert_text(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data)
{
    int i;

    for (i = 0; i < length; i++)
    {
        if (!(text[i] >= '1' && text[i] <= '9'))
        {
            g_signal_stop_emission_by_name(G_OBJECT(editable), "insert-text");
            return;
        }
    }
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
    GtkBuilder *builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "./builder.glade", NULL);

    GObject *window = gtk_builder_get_object(builder, "window");
    gtk_window_set_application(GTK_WINDOW(window), app);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

    // set label content
    GObject *label = gtk_builder_get_object(builder, "title");
    gtk_label_set_label(GTK_LABEL(label), "数独 Cr.Civitasv");

    // fixed
    fixed = GTK_FIXED(gtk_builder_get_object(builder, "fixed"));

    // check button
    GObject *check_btn = gtk_builder_get_object(builder, "check");
    g_signal_connect(check_btn, "clicked", G_CALLBACK(check), NULL);

    // new game button
    GObject *new_game_btn = gtk_builder_get_object(builder, "new_game");
    g_signal_connect(new_game_btn, "clicked", G_CALLBACK(new_game), NULL);

    // hint button
    GObject *hint_btn = gtk_builder_get_object(builder, "hint");
    g_signal_connect(hint_btn, "clicked", G_CALLBACK(hint), NULL);

    // grid
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 0);
    gtk_css_set(GTK_WIDGET(grid));
    gtk_fixed_put(fixed, grid, 0, 0);

    init_game();

    gtk_widget_show_all(GTK_WIDGET(window));

    /* We do not need the builder any more */
    g_object_unref(builder);
}

static void
init_game()
{
    // clear
    gtk_container_remove(GTK_CONTAINER(fixed), grid);
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 0);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 0);
    gtk_css_set(GTK_WIDGET(grid));

    gtk_fixed_put(GTK_FIXED(fixed), GTK_WIDGET(grid), 0, 0);
    gtk_widget_show(grid);
    // generate sudos
    sudos_generate_real();
    // add entry to grid
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (sudos[i][j] == 0)
            {
                is_label[i][j] = false;
                entries[i][j] = gtk_entry_new();
                gtk_widget_set_size_request(entries[i][j], BLOCK_SIZE, BLOCK_SIZE);
                gtk_entry_set_alignment(GTK_ENTRY(entries[i][j]), 0.5);
                gtk_entry_set_width_chars(GTK_ENTRY(entries[i][j]), 1);
                gtk_entry_set_max_length(GTK_ENTRY(entries[i][j]), 1);
                gtk_css_set(GTK_WIDGET(entries[i][j]));
                gtk_grid_attach(GTK_GRID(grid), entries[i][j], j, i, 1, 1);
                int *data = malloc(sizeof(int) * 2);
                *data = i;
                *(data + 1) = j;
                g_signal_connect(entries[i][j], "focus-in-event", G_CALLBACK(entry_activate), (gpointer)data);
                g_signal_connect(entries[i][j], "changed", G_CALLBACK(entry_changed), NULL);
                g_signal_connect(entries[i][j], "insert-text", G_CALLBACK(entry_insert_text), NULL);
                gtk_widget_show(entries[i][j]);
            }
            else
            {
                is_label[i][j] = true;
                char title[2];
                sprintf(title, "%d", sudos[i][j]);
                GtkWidget *label = gtk_label_new_with_mnemonic(title);
                gtk_widget_set_size_request(label, BLOCK_SIZE, BLOCK_SIZE);
                gtk_css_set(label);
                gtk_grid_attach(GTK_GRID(grid), label, j, i, 1, 1);
                gtk_widget_show(label);
            }
        }
    }
}

int main(int argc,
         char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.civitasv.sudoku", G_APPLICATION_FLAGS_NONE);
    // add callback on active application
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    // free memory
    g_object_unref(app);
    return 0;
}
