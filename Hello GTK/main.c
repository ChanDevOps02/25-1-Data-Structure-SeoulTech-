#include <gtk/gtk.h>
#include <libpq-fe.h>

// 친구 정보를 저장하는 구조체
// name: 친구 이름, count: 카톡 횟수
typedef struct {
    char name[100];
    int count;
} FriendInfo;

// 전역 위젯 및 데이터
GtkWidget *list_box;       // 친구 목록을 표시할 리스트 박스
GtkWidget *entry_name;     // 이름 입력용 엔트리
GtkWidget *entry_count;    // 카톡 횟수 입력용 엔트리
GtkWidget *time_label;     // 처리 시간 표시용 라벨
GList *friend_list = NULL; // 메모리 내에 저장된 친구 리스트
PGconn *conn = NULL;       // PostgreSQL 연결 객체

// 위젯에 폰트를 적용하는 헬퍼 함수
void apply_font(GtkWidget *widget, const gchar *font_str) {
    PangoFontDescription *font_desc = pango_font_description_from_string(font_str);
    gtk_widget_override_font(widget, font_desc);
    pango_font_description_free(font_desc);
}

// 입력 텍스트에서 아스키코드 이외 문자를 필터링하여 삽입하도록 제어
// editable: 입력 위젯, text: 입력 문자열, length: 문자열 길이, position: 삽입 위치
gboolean on_insert_text_only_ascii(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer user_data) {
    GString *filtered = g_string_new(NULL);

    const gchar *p = text;
    const gchar *end = text + length;

    // 입력 문자열을 UTF-8 문자 단위로 순회
    while (p < end) {
        gunichar c = g_utf8_get_char(p);
        // 영숫자 또는 공백인 경우에만 필터링 문자열에 추가
        if (g_unichar_isalnum(c) || g_unichar_isspace(c)) {
            gchar buf[7]; // UTF-8 문자 최대 6바이트 + null
            gint bytes = g_unichar_to_utf8(c, buf);
            buf[bytes] = '\0';
            g_string_append(filtered, buf);
        }
        p = g_utf8_next_char(p);
    }

    // 필터링 후 길이가 달라졌으면 원본 입력을 중단하고 필터된 문자열 삽입
    if (g_utf8_strlen(filtered->str, -1) < g_utf8_strlen(text, -1)) {
        g_signal_stop_emission_by_name(editable, "insert-text");
        gtk_editable_insert_text(editable, filtered->str, -1, position);
    }

    g_string_free(filtered, TRUE);
    return TRUE;
}

// 시간 측정 결과(마이크로초)를 밀리초로 변환
double measure_duration_us(gint64 start, gint64 end) {
    return (end - start) / 1000.0;
}

// friend_list 데이터를 기반으로 GTK 리스트 박스에 항목을 렌더링
void render_list() {
    // 기존 리스트 박스 항목 없애기
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *iter = children; iter; iter = iter->next) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // friend_list를 돌면서 각 FriendInfo를 화면에 추가
    for (GList *iter = friend_list; iter; iter = iter->next) {
        FriendInfo *info = (FriendInfo *)iter->data;

        // "이름 (횟수)" 형태의 라벨 생성
        gchar *label_text = g_strdup_printf("%s (%d)", info->name, info->count);
        GtkWidget *label = gtk_label_new(label_text);
        gtk_widget_set_halign(label, GTK_ALIGN_START);
        apply_font(label, "Apple SD Gothic Neo 14");

        // 라벨을 프레임 안에 넣고, 프레임을 리스트 박스 행에 추가
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *frame = gtk_frame_new(NULL);
        gtk_container_add(GTK_CONTAINER(frame), label);
        gtk_container_add(GTK_CONTAINER(row), frame);

        // CSS 스타일을 적용하기 위한 이름 설정
        gtk_widget_set_name(row, "friend_row");
        gtk_list_box_insert(GTK_LIST_BOX(list_box), row, -1);

        g_free(label_text);
    }

    gtk_widget_show_all(list_box); // 변경된 내용 표시
}

// "추가" 버튼 클릭 시 호출되는 콜백
static void on_add_clicked(GtkButton *button, gpointer user_data) {
    gint64 start = g_get_real_time(); // 시작 시간 기록

    const gchar *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const gchar *count_str = gtk_entry_get_text(GTK_ENTRY(entry_count));
    // 입력값 확인해보기: 비어있으면 무시
    if (g_strcmp0(name, "") == 0 || g_strcmp0(count_str, "") == 0) return;

    int count = atoi(count_str);

    // DB에 INSERT 쿼리 실행 (직접 sql문 작성해야됌)
    gchar *query = g_strdup_printf("INSERT INTO friends (name, count) VALUES ('%s', %d);", name, count);
    PGresult *res = PQexec(conn, query);
    g_free(query);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DB INSERT 실패: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }
    PQclear(res);

    // 새로운 FriendInfo 구조체 생성 및 초기화
    FriendInfo *new_info = g_new(FriendInfo, 1);
    strncpy(new_info->name, name, sizeof(new_info->name));
    new_info->count = count;

    // friend_list에 카운트 내림차순으로 삽입
    GList *iter = friend_list;
    int inserted = 0, index = 0;
    while (iter) {
        FriendInfo *current = (FriendInfo *)iter->data;
        if (new_info->count > current->count) {
            friend_list = g_list_insert(friend_list, new_info, index);
            inserted = 1;
            break;
        }
        iter = iter->next;
        index++;
    }
    if (!inserted) {
        friend_list = g_list_append(friend_list, new_info);
    }

    // 입력 필드 초기화
    gtk_entry_set_text(GTK_ENTRY(entry_name), "");
    gtk_entry_set_text(GTK_ENTRY(entry_count), "");

    render_list(); // 리스트 갱신

    // 처리 시간 계산 및 라벨에 표시
    gint64 end = g_get_real_time();
    double duration = measure_duration_us(start, end);
    gchar *time_text = g_strdup_printf("\u23F1 처리 시간: %.2f ms", duration);
    gtk_label_set_text(GTK_LABEL(time_label), time_text);
    g_free(time_text);
}

// "삭제" 버튼 클릭 시 호출되는 콜백
static void on_delete_clicked(GtkButton *button, gpointer user_data) {
    gint64 start = g_get_real_time();

    // 선택된 행 가져오기
    GtkListBoxRow *selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(list_box));
    if (!selected_row) return;

    int index = gtk_list_box_row_get_index(selected_row);
    if (index < 0 || index >= g_list_length(friend_list)) return;

    // friend_list에서 해당 인덱스의 항목 찾기
    GList *to_remove = g_list_nth(friend_list, index);
    if (to_remove) {
        FriendInfo *info = (FriendInfo *)to_remove->data;

        // DB에 DELETE 쿼리 실행
        gchar *query = g_strdup_printf("DELETE FROM friends WHERE name = '%s';", info->name);
        PGresult *res = PQexec(conn, query);
        g_free(query);

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            fprintf(stderr, "DB DELETE 실패: %s\n", PQerrorMessage(conn));
            PQclear(res);
            return;
        }
        PQclear(res);

        // 메모리 해제 및 리스트에서 제거
        g_free(info);
        friend_list = g_list_delete_link(friend_list, to_remove);
    }

    render_list(); // 리스트 갱신

    // 처리 시간 계산 및 표시
    gint64 end = g_get_real_time();
    double duration = measure_duration_us(start, end);
    gchar *time_text = g_strdup_printf("\u23F1 처리 시간: %.2f ms", duration);
    gtk_label_set_text(GTK_LABEL(time_label), time_text);
    g_free(time_text);
}

// DB에서 친구 데이터를 로드하여 friend_list 초기화
void load_friends_from_db() {
    const char *conninfo = "host=localhost port=5432 dbname=frienddb user=kmc password=1234";

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "DB 연결 실패: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return;
    }

    // count 내림차순으로 조회
    PGresult *res = PQexec(conn, "SELECT name, count FROM friends ORDER BY count DESC");
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT 실패: %s\n", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        const char *name = PQgetvalue(res, i, 0);
        int count = atoi(PQgetvalue(res, i, 1));

        FriendInfo *info = g_new(FriendInfo, 1);
        strncpy(info->name, name, sizeof(info->name));
        info->count = count;
        friend_list = g_list_append(friend_list, info);
    }

    PQclear(res);
    render_list(); // 초기 렌더링
}

// GTK 애플리케이션 활성화 시 UI 구성 및 시그널 연결
static void on_activate(GtkApplication *app, gpointer user_data) {
    // CSS 스타일 정의 및 적용
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "#friend_row {"
        "  background-color: #dbeafe;"
        "  color: #1e3a8a;"
        "  border-radius: 8px;"
        "  padding: 10px;"
        "  margin-bottom: 5px;"
        "}"
        "window { background-color: #f0f4f8; }"
        , -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // 메인 윈도우 생성
    GtkWidget *window = gtk_application_window_new(app);
    gtk_widget_set_name(window, "window");
    gtk_window_set_title(GTK_WINDOW(window), "친구 목록 관리");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 500);

    // 수직 박스 레이아웃 생성
    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_vbox), 15);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);

    // 리스트 박스 생성 및 추가
    list_box = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(list_box), GTK_SELECTION_SINGLE);
    gtk_box_pack_start(GTK_BOX(main_vbox), list_box, TRUE, TRUE, 0);

    // 입력 필드 박스 생성
    GtkWidget *input_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_vbox), input_box, FALSE, FALSE, 0);

    // 이름 입력 엔트리
    entry_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_name), "이름 입력");
    apply_font(entry_name, "Apple SD Gothic Neo 14");
    gtk_box_pack_start(GTK_BOX(input_box), entry_name, TRUE, TRUE, 0);
    g_signal_connect(entry_name, "insert-text", G_CALLBACK(on_insert_text_only_ascii), NULL);

    // 카운트 입력 엔트리
    entry_count = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_count), "카톡 횟수 입력");
    apply_font(entry_count, "Apple SD Gothic Neo 14");
    gtk_box_pack_start(GTK_BOX(input_box), entry_count, TRUE, TRUE, 0);
    g_signal_connect(entry_count, "insert-text", G_CALLBACK(on_insert_text_only_ascii), NULL);

    // 버튼 박스 및 버튼 생성
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(main_vbox), button_box, FALSE, FALSE, 0);

    GtkWidget *btn_add = gtk_button_new_with_label("\u2795 추가");
    apply_font(btn_add, "Apple SD Gothic Neo 14");
    gtk_box_pack_start(GTK_BOX(button_box), btn_add, TRUE, TRUE, 0);
    g_signal_connect(btn_add, "clicked", G_CALLBACK(on_add_clicked), NULL);

    GtkWidget *btn_delete = gtk_button_new_with_label("\u1F5D1 삭제");
    apply_font(btn_delete, "Apple SD Gothic Neo 14");
    gtk_box_pack_start(GTK_BOX(button_box), btn_delete, TRUE, TRUE, 0);
    g_signal_connect(btn_delete, "clicked", G_CALLBACK(on_delete_clicked), NULL);

    // 처리 시간 라벨 생성
    time_label = gtk_label_new("\u23F1 처리 시간: 0.00 ms");
    apply_font(time_label, "Apple SD Gothic Neo 13");
    gtk_box_pack_start(GTK_BOX(main_vbox), time_label, FALSE, FALSE, 0);

    // DB에서 초기 데이터 로드
    load_friends_from_db();

    gtk_widget_show_all(window); // UI 표시
}

// GTK 실행
int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.friendsapp", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
