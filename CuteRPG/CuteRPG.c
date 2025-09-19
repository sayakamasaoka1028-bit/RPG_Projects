#include <stdio.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#define WIDTH 8
#define HEIGHT 6

// マップ
wchar_t map[HEIGHT][WIDTH+1] = {
    L"🟫🟫🟫🟫🟫🟫🟫🟫",
    L"🟫🐱⬜👧⬜⬜🎁🟫",
    L"🟫⬜⬜⬜⬜⬜⬜🟫",
    L"🟫⬜⬜👧⬜⬜⬜🟫",
    L"🟫⬜⬜⬜⬜⬜⬜🟫",
    L"🟫🟫🟫🟫🟫🟫🟫🟫"
};

// プレイヤー座標
int px = 1, py = 1;

// マップ描画
void drawMap() {
    for (int y = 0; y < HEIGHT; y++) {
        wprintf(L"%ls\n", map[y]);
    }
}

// 会話イベント
void talk(int x, int y) {
    if (x == 3 && y == 1) {
        wprintf(L"\n👧: 「こんにちはにゃん！今日はお散歩日和だね♪」\n\n");
    }
    if (x == 3 && y == 3) {
        wprintf(L"\n👧: 「宝箱は森の中にあるよ。気をつけてね☆」\n\n");
    }
}

// 宝箱イベント
void treasure(int x, int y) {
    if (x == 6 && y == 1) {
        wprintf(L"\n🎁 宝箱発見！中にはかわいいお菓子が入ってたよ🍬\n\n");
        map[y][x] = L'⬜'; // 宝箱を取ったら道に戻す
    }
}

int main() {
    setlocale(LC_ALL, ""); // UTF-8対応
    char command;

    wprintf(L"=== かわいいマップRPG ===\n");
    wprintf(L"操作: w(上) a(左) s(下) d(右) q(終了)\n\n");

    while (1) {
        drawMap();
        wprintf(L"移動コマンド: ");
        scanf(" %c", &command);

        int nx = px, ny = py;
        if (command == 'w') ny--;
        if (command == 's') ny++;
        if (command == 'a') nx--;
        if (command == 'd') nx++;
        if (command == 'q') break;

        // 壁じゃなければ移動
        if (map[ny][nx] != L'🟫') {
            map[py][px] = L'⬜'; // 道に戻す
            px = nx;
            py = ny;

            talk(px, py);
            treasure(px, py);

            map[py][px] = L'🐱'; // プレイヤーを置く
        }

        system("clear"); // Linux/macOS
        // Windowsなら system("cls");
    }

    wprintf(L"ゲーム終了！また遊んでね☆\n");
    return 0;
}
