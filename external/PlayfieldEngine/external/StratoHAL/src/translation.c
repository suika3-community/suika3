/* -*- tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * StratoHAL
 * Translation routine.
 */

/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2025-2026 Awe Morris
 * Copyright (c) 1996-2024 Keiichi Tabata
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *	claim that you wrote the original software. If you use this software
 *	in a product, an acknowledgment in the product documentation would be
 *	appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *	misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <string.h>

const char *hal_get_system_language(void);

const char *hal_gettext(const char *msg)
{
    const char *lang_code = hal_get_system_language();
    if (strcmp(msg, "Out of memory.") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "Out of memory.";
        if (strncmp(lang_code, "es", 2) == 0) return "Memoria insuficiente.";
        if (strncmp(lang_code, "fr", 2) == 0) return "Mémoire insuffisante.";
        if (strncmp(lang_code, "it", 2) == 0) return "Memoria esaurita.";
        if (strncmp(lang_code, "de", 2) == 0) return "Nicht genügend Speicher.";
        if (strncmp(lang_code, "el", 2) == 0) return "Εκτός μνήμης.";
        if (strncmp(lang_code, "ru", 2) == 0) return "Недостаточно памяти.";
        if (strncmp(lang_code, "zh", 2) == 0) return "内存不足。";
        if (strncmp(lang_code, "tw", 2) == 0) return "記憶體不足。";
        if (strncmp(lang_code, "ja", 2) == 0) return "メモリが不足しています。";
        return "Out of memory.";
    }
    if (strcmp(msg, "Cannot open file \"%s\".") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "Cannot open file \"%s\".";
        if (strncmp(lang_code, "es", 2) == 0) return "No se puede abrir el archivo \"%s\".";
        if (strncmp(lang_code, "fr", 2) == 0) return "Impossible d'ouvrir le fichier \"%s\".";
        if (strncmp(lang_code, "it", 2) == 0) return "Impossibile aprire il file \"%s\".";
        if (strncmp(lang_code, "de", 2) == 0) return "Datei \"%s\" kann nicht geöffnet werden.";
        if (strncmp(lang_code, "el", 2) == 0) return "Δεν είναι δυνατό το άνοιγμα του αρχείου \"%s\".";
        if (strncmp(lang_code, "ru", 2) == 0) return "Невозможно открыть файл \"%s\".";
        if (strncmp(lang_code, "zh", 2) == 0) return "无法打开文件 \"%s\"。";
        if (strncmp(lang_code, "tw", 2) == 0) return "無法打開檔案 \"%s\"。";
        if (strncmp(lang_code, "ja", 2) == 0) return "ファイル \"%s\" を開けません。";
        return "Cannot open file \"%s\".";
    }
    if (strcmp(msg, "Cannot read file \"%s\".") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "Cannot read file \"%s\".";
        if (strncmp(lang_code, "es", 2) == 0) return "No se puede leer el archivo \"%s\".";
        if (strncmp(lang_code, "fr", 2) == 0) return "Impossible de lire le fichier \"%s\".";
        if (strncmp(lang_code, "it", 2) == 0) return "Impossibile leggere il file \"%s\".";
        if (strncmp(lang_code, "de", 2) == 0) return "Datei \"%s\" kann nicht gelesen werden.";
        if (strncmp(lang_code, "el", 2) == 0) return "Δεν είναι δυνατή η ανάγνωση του αρχείου \"%s\".";
        if (strncmp(lang_code, "ru", 2) == 0) return "Невозможно прочитать файл \"%s\".";
        if (strncmp(lang_code, "zh", 2) == 0) return "无法读取文件 \"%s\"。";
        if (strncmp(lang_code, "tw", 2) == 0) return "無法讀取檔案 \"%s\"。";
        if (strncmp(lang_code, "ja", 2) == 0) return "ファイル \"%s\" を読み込めません。";
        return "Cannot read file \"%s\".";
    }
    if (strcmp(msg, "No startup file.") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "No startup file.";
        if (strncmp(lang_code, "es", 2) == 0) return "No hay archivo de inicio.";
        if (strncmp(lang_code, "fr", 2) == 0) return "Aucun fichier de démarrage.";
        if (strncmp(lang_code, "it", 2) == 0) return "Nessun file di avvio.";
        if (strncmp(lang_code, "de", 2) == 0) return "Keine Startdatei vorhanden.";
        if (strncmp(lang_code, "el", 2) == 0) return "Δεν υπάρχει αρχείο εκκίνησης.";
        if (strncmp(lang_code, "ru", 2) == 0) return "Файл запуска отсутствует.";
        if (strncmp(lang_code, "zh", 2) == 0) return "没有启动文件。";
        if (strncmp(lang_code, "tw", 2) == 0) return "沒有啟動檔案。";
        if (strncmp(lang_code, "ja", 2) == 0) return "スタートアップファイルがありません。";
        return "No startup file.";
    }
    if (strcmp(msg, "The app is crashed.") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "The app is crashed.";
        if (strncmp(lang_code, "es", 2) == 0) return "La aplicación se ha bloqueado.";
        if (strncmp(lang_code, "fr", 2) == 0) return "L'application a planté.";
        if (strncmp(lang_code, "it", 2) == 0) return "L'applicazione è andata in crash.";
        if (strncmp(lang_code, "de", 2) == 0) return "Die App ist abgestürzt.";
        if (strncmp(lang_code, "el", 2) == 0) return "Η εφαρμογή κατέρρευσε.";
        if (strncmp(lang_code, "ru", 2) == 0) return "Приложение завершилось с ошибкой.";
        if (strncmp(lang_code, "zh", 2) == 0) return "应用程序已崩溃。";
        if (strncmp(lang_code, "tw", 2) == 0) return "應用程式已當機。";
        if (strncmp(lang_code, "ja", 2) == 0) return "アプリケーションがクラッシュしました。";
        return "The app is crashed.";
    }
    if (strcmp(msg, "Failed to initialize the sound.") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "Failed to initialize the sound.";
        if (strncmp(lang_code, "es", 2) == 0) return "Error al inicializar el sonido.";
        if (strncmp(lang_code, "fr", 2) == 0) return "Échec de l'initialisation du son.";
        if (strncmp(lang_code, "it", 2) == 0) return "Impossibile inizializzare l'audio.";
        if (strncmp(lang_code, "de", 2) == 0) return "Initialisierung des Sounds fehlgeschlagen.";
        if (strncmp(lang_code, "el", 2) == 0) return "Αποτυχία αρχικοποίησης του ήχου.";
        if (strncmp(lang_code, "ru", 2) == 0) return "Не удалось инициализировать звук.";
        if (strncmp(lang_code, "zh", 2) == 0) return "声音初始化失败。";
        if (strncmp(lang_code, "tw", 2) == 0) return "聲音初始化失敗。";
        if (strncmp(lang_code, "ja", 2) == 0) return "サウンドの初期化に失敗しました。";
        return "Failed to initialize the sound.";
    }
    if (strcmp(msg, "Failed to initialize the graphics.") == 0) {
        if (strncmp(lang_code, "en", 2) == 0) return "Failed to initialize the graphics.";
        if (strncmp(lang_code, "es", 2) == 0) return "Error al inicializar los gráficos.";
        if (strncmp(lang_code, "fr", 2) == 0) return "Echec de linitialisation des graphiques.";
        if (strncmp(lang_code, "it", 2) == 0) return "Impossibile inizializzare la grafica.";
        if (strncmp(lang_code, "de", 2) == 0) return "Initialisierung der Grafik fehlgeschlagen.";
        if (strncmp(lang_code, "el", 2) == 0) return "Αποτυχία αρχικοποίησης των γραφικών.";
        if (strncmp(lang_code, "ru", 2) == 0) return "Не удалось инициализировать графику.";
        if (strncmp(lang_code, "zh", 2) == 0) return "图形初始化失败。";
        if (strncmp(lang_code, "tw", 2) == 0) return "圖形初始化失敗。";
        if (strncmp(lang_code, "ja", 2) == 0) return "グラフィックスの初期化に失敗しました。";
        return "Failed to initialize the graphics.";
    }
    return msg;
}
