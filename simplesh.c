/*
 *	画面にプロンプトを表示させ、キーボードからコマンド入力を実行するプログラム simplesh.c
    "help"コマンドを使用すると、ヘルプメッセージを表示
    "kyorochan" コマンドを使用すると、キョロちゃんのアスキーアートを表示
    "masuo" コマンドを使用すると、マスオさんのアスキーアートを表示
    "yonezu" コマンドを使用すると、米津玄師の曲がランダムで再生
 *  使用例) ./simplesh
 */

#include <stdio.h>   // 標準入力出力ライブラリ
#include <stdlib.h>  // 標準ライブラリ（例：ランダム、システムコマンドなど）
#include <unistd.h>  // POSIX API（例：fork, chdirなど）
#include <string.h>  // 文字列操作ライブラリ
#include <sys/wait.h> // プロセスの終了待ちをするためのライブラリ
#include <time.h>    // 時間ライブラリ（例：randのシード設定）
#include <errno.h>   // エラーハンドリングのためのライブラリ

#define MAX_LINE 80 // コマンドの最大長を定義

// ヘルプメッセージを表示する関数
void print_help() {
    printf(
        "利用可能なコマンド例:\n"
        "  help      - ヘルプメッセージを表示\n"
        "  ls        - ディレクトリ内のファイルやフォルダの一覧を表示\n"
        "  cd        - 指定したディレクトリに移動\n"
        "  kyorochan - キョロちゃんのAAを表示\n"
        "  masuo     - マスオさんのAAを表示\n"
        "  yonezu    - ランダムに選ばれた米津玄師のYouTube動画を再生\n"
        "  exit      - シェルを終了\n"
    );
}

int main(int argc, char *argv[]) {

    // 引数がある場合はエラーを出力して終了
    if (argc > 1) {
        fprintf(stderr, "使用方法: %s\n", argv[0]);
        return 1;
    }
    
    char *args[MAX_LINE / 2 + 1]; // コマンドライン引数の配列
    char input[MAX_LINE];         // ユーザーからの入力を格納するバッファ
    int should_run = 1;           // シェルを動作させるかどうかのフラグ

    // ランダムなURLを格納する配列（米津玄師のYouTube動画のURL）
    const char *urls[] = {
        "https://www.youtube.com/watch?v=-wb2PAx6aEs",
        "https://www.youtube.com/watch?v=BYGo5t6rdA8",
        "https://www.youtube.com/watch?v=Zhx1n6uvgUE",
        "https://www.youtube.com/watch?v=7WZ1Kt3zraY",
        "https://www.youtube.com/watch?v=UFQEttrn6CQ",
        "https://www.youtube.com/watch?v=XeFQJ6-XoD0",
        "https://www.youtube.com/watch?v=ptnYBctoexk",
        "https://www.youtube.com/watch?v=1s84rIhPuhk",
        "https://www.youtube.com/watch?v=SX_ViT4Ra7k",
        "https://www.youtube.com/watch?v=zkNzxsaCunU",
        "https://www.youtube.com/watch?v=Dx_fKPBPYUI",
        "https://www.youtube.com/watch?v=lzAyrgSqeeE",
        "https://www.youtube.com/watch?v=2PqxOytUjz0"
    };
    int num_urls = sizeof(urls) / sizeof(urls[0]); // URL配列の要素数

    // ランダムなシードを現在の時間で初期化
    srand(time(NULL));

    // メインのシェルのループ
    while (should_run) {
        printf("PROMPT> ");     // プロンプトを表示
        fflush(stdout);         // 出力を即時に反映

        // ユーザーからの入力を読み込む
        if (fgets(input, MAX_LINE, stdin) == NULL) {
            // 入力の読み込みに失敗した場合
            fprintf(stderr, "入力の読み込みに失敗しました: %s\n", strerror(errno));
            return 1; // プログラムを終了
        }

        // 入力の最後の改行文字を取り除く
        size_t length = strlen(input);
        if (length > 0 && input[length - 1] == '\n') {
            input[length - 1] = '\0';
        }

        // コマンドをスペースで区切って解析
        int arg_count = 0;  // 引数の数を初期化
        char *token = strtok(input, " "); // 最初のトークンを取得
        while (token != NULL) {
            args[arg_count] = token; // トークンを引数リストに追加
            arg_count++;
            token = strtok(NULL, " "); // 次のトークンを取得
        }
        args[arg_count] = NULL; // 引数リストの終わりをNULLで示す

        // "exit" コマンドの処理
        if (arg_count > 0 && strcmp(args[0], "exit") == 0) {
            should_run = 0; // ループを終了
            continue; // 次のループへ
        }

        // "cd" コマンドの処理（親プロセスで実行）
        if (arg_count > 0 && strcmp(args[0], "cd") == 0) {
            // 引数がない場合はホームディレクトリへ移動
            const char *dir = (arg_count < 2) ? getenv("HOME") : args[1];
            if (dir == NULL) {
                // ホームディレクトリが見つからない場合のエラーメッセージ
                fprintf(stderr, "cd: ホームディレクトリが見つかりません\n");
            } else {
                // 指定したディレクトリへ移動
                if (chdir(dir) != 0) {
                    // ディレクトリ変更に失敗した場合のエラーメッセージ
                    fprintf(stderr, "cd: ディレクトリの変更に失敗しました: %s\n", strerror(errno));
                }
            }
            continue; // 次のループへ
        }

        // "kyorochan" コマンドの処理
        if (arg_count == 1 && strcmp(args[0], "kyorochan") == 0) {
            // キョロちゃんのアスキーアートを表示
            printf(
                "\n"
                "　　　　 ＿＿＿＿\n"
                "　　　 ／👁️     ＼\n"
                "　　　/　　 👁️   ヽ    クエー\n"
                "　　／⌒＼　　　|  \n"
                "　 /　　　L＿＿ノ|  < \n"
                "　｜　　 / L `　　 | \n"
                "　｜　　/ / LГL／ |\n"
                "　｜　 / /　　　　|\n"
                "　 ＼／ｲ　　　　 ﾉ\n"
                "　　　 ＼＿＿＿／\n"
                "　　　 ＿|_　_|_\n"
                "\n"
            );
            continue; // 次のループへ
        }

        // "masuo" コマンドの処理
        if (arg_count == 1 && strcmp(args[0], "masuo") == 0) {
            // マスオさんのアスキーアートを表示
            printf(
                "\n"
                "    / ＞　⌒　⌒ |\n"
                    "｜/ 　 (･) (･)　|\n"
                    "(６    ――○-○-   |    ええええ？ホントかい？カツオくん\n"
                    "｜　　　　つ   |\n"
                    "｜　 　 ＿＿_)/\n"
                    "＼ 　 (＿／ /\n"
                    "／＼＿＿／\n"
                "\n"
            );

            continue; // 次のループへ
        }

        // "yonezu" コマンドの処理
        if (arg_count == 1 && strcmp(args[0], "yonezu") == 0) {
            // ランダムな米津玄師のYouTube動画のURLを選択
            int random_index = rand() % num_urls;
            // 選択したURLを開くためのコマンドを作成
            char command[MAX_LINE];
            snprintf(command, sizeof(command), "open %s", urls[random_index]);
            // システムコマンドとして実行
            int result = system(command);
            if (result == -1) {
                // コマンドの実行に失敗した場合のエラーメッセージ
                fprintf(stderr, "system: コマンドの実行に失敗しました: %s\n", strerror(errno));
            }
            continue; // 次のループへ
        }

        // "help" コマンドの処理
        if (arg_count == 1 && strcmp(args[0], "help") == 0) {
            // ヘルプメッセージを表示
            print_help();
            continue; // 次のループへ
        }

        // その他のコマンドを子プロセスで実行
        pid_t pid = fork(); // 新しいプロセスを作成
        if (pid < 0) {
            // forkの作成に失敗した場合のエラーメッセージ
            fprintf(stderr, "forkの作成に失敗しました: %s\n", strerror(errno));
            return 1; // プログラムを終了
        } else if (pid == 0) {
            // 子プロセスで実行
            if (execvp(args[0], args) < 0) {
                // コマンドの実行に失敗した場合のエラーメッセージ
                fprintf(stderr, "execvpの実行に失敗しました: %s\n", strerror(errno));
                return 1; // 子プロセスを終了
            }
        } else {
            // 親プロセスで子プロセスの終了を待つ
            wait(NULL);
        }
    }

    // シェルを終了するメッセージ
    printf("Good Bye!\n");

    return 0; // プログラムを正常終了
}
