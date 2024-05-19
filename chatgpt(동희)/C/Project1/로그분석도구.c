#include <gtk/gtk.h>

// 파일 선택 다이얼로그에서 파일을 선택했을 때의 콜백 함수
void select_file(GtkWidget* widget, gpointer data) {
    GtkWidget* dialog;
    dialog = gtk_file_chooser_dialog_new("파일 선택",
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "취소", GTK_RESPONSE_CANCEL,
        "선택", GTK_RESPONSE_ACCEPT,
        NULL);
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* filename;
        GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        // 파일 경로를 출력
        g_print("Selected file: %s\n", filename);
        g_free(filename);
    }
    gtk_widget_destroy(dialog);
}

// 메인 함수
int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* button;
    gtk_init(&argc, &argv);

    // 윈도우 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "로그 분석 도구");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 파일 선택 버튼 생성
    button = gtk_button_new_with_label("파일 선택");
    g_signal_connect(button, "clicked", G_CALLBACK(select_file), NULL);
    gtk_container_add(GTK_CONTAINER(window), button);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
