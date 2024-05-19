#include <gtk/gtk.h>

// ���� ���� ���̾�α׿��� ������ �������� ���� �ݹ� �Լ�
void select_file(GtkWidget* widget, gpointer data) {
    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new("���� ����",
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "���", GTK_RESPONSE_CANCEL,
        "����", GTK_RESPONSE_ACCEPT,
        NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename;
        GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        // ���� ��θ� ���
        g_print("Selected file: %s\n", filename);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

// ���� �Լ�
int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* button;
    gtk_init(&argc, &argv);

    // ������ ����
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "�α� �м� ����");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // ���� ���� ��ư ����
    button = gtk_button_new_with_label("���� ����");
    g_signal_connect(button, "clicked", G_CALLBACK(select_file), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
