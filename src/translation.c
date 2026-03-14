/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Translation
 */

/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2026 The Suika3 Community
 * Copyright (c) 2025-2026 The Playfield Engine Project
 * Copyright (c) 2025-2026 The NoctVM Project
 * Copyright (c) 2025-2026 Awe Morris
 * Copyright (c) 2016-2024 The Suika2 Development Team
 * Copyright (c) 1996-2024 Keiichi Tabata
 *
 * This software is derived from the codebase of Playfield Engine, NoctLang,
 * Suika2, Suika Studio, Wind Game Lib, and 98/AT Game Lib.
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
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <string.h>

const char *s3_get_system_language(void);

const char *s3_gettext(const char *msg)
{
    const char *lang_code = s3_get_system_language();
    if (strcmp(msg, "File %s: Line %d: %s") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Archivo %s: Línea %d: %s";
        if (strcmp(lang_code, "fr") == 0) return "Fichier %s: Ligne %d: %s";
        if (strcmp(lang_code, "it") == 0) return "File %s: Linea %d: %s";
        if (strcmp(lang_code, "de") == 0) return "Datei %s: Zeile %d: %s";
        if (strcmp(lang_code, "el") == 0) return "Αρχείο %s: Γραμμή %d: %s";
        if (strcmp(lang_code, "ru") == 0) return "Файл %s: Строка %d: %s";
        if (strcmp(lang_code, "zh") == 0) return "文件 %s: 第 %d 行: %s";
        if (strcmp(lang_code, "tw") == 0) return "檔案 %s: 第 %d 行: %s";
        if (strcmp(lang_code, "ja") == 0) return "ファイル %s: %d 行目: %s";
        return "File %s: Line %d: %s";
    }
    if (strcmp(msg, "File %s: Line %d: Tag %s: ") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Archivo %s: Línea %d: Etiqueta %s: ";
        if (strcmp(lang_code, "fr") == 0) return "Fichier %s: Ligne %d: Balise %s: ";
        if (strcmp(lang_code, "it") == 0) return "File %s: Linea %d: Tag %s: ";
        if (strcmp(lang_code, "de") == 0) return "Datei %s: Zeile %d: Tag %s: ";
        if (strcmp(lang_code, "el") == 0) return "Αρχείο %s: Γραμμή %d: Ετικέτα %s: ";
        if (strcmp(lang_code, "ru") == 0) return "Файл %s: Строка %d: Тег %s: ";
        if (strcmp(lang_code, "zh") == 0) return "文件 %s: 第 %d 行: 标签 %s: ";
        if (strcmp(lang_code, "tw") == 0) return "檔案 %s: 第 %d 行: 標籤 %s: ";
        if (strcmp(lang_code, "ja") == 0) return "ファイル %s: %d 行目: タグ %s: ";
        return "File %s: Line %d: Tag %s: ";
    }
    if (strcmp(msg, "File %s: Line %d: Runtime error.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Archivo %s: Línea %d: Error de ejecución.";
        if (strcmp(lang_code, "fr") == 0) return "Fichier %s: Ligne %d: Erreur d'exécution.";
        if (strcmp(lang_code, "it") == 0) return "File %s: Linea %d: Erreur d'esecuzione.";
        if (strcmp(lang_code, "de") == 0) return "Datei %s: Zeile %d: Laufzeitfehler.";
        if (strcmp(lang_code, "el") == 0) return "Αρχείο %s: Γραμμή %d: Σφάλμα κατά την εκτέλεση.";
        if (strcmp(lang_code, "ru") == 0) return "Файл %s: Строка %d: Ошибка времени выполнения.";
        if (strcmp(lang_code, "zh") == 0) return "文件 %s: 第 %d 行: 运行时错误。";
        if (strcmp(lang_code, "tw") == 0) return "檔案 %s: 第 %d 行: 執行階段錯誤。";
        if (strcmp(lang_code, "ja") == 0) return "ファイル %s: %d 行目: 実行時エラー。";
        return "File %s: Line %d: Runtime error.";
    }
    if (strcmp(msg, "%s:%d: Tag \"%s\" not found.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "%s:%d: No se encontró la etiqueta \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "%s:%d : Balise \"%s\" introuvable.";
        if (strcmp(lang_code, "it") == 0) return "%s:%d: Tag \"%s\" non trovato.";
        if (strcmp(lang_code, "de") == 0) return "%s:%d: Tag \"%s\" wurde nicht gefunden.";
        if (strcmp(lang_code, "el") == 0) return "%s:%d: Η ετικέτα \"%s\" δεν βρέθηκε.";
        if (strcmp(lang_code, "ru") == 0) return "%s:%d: Тег \"%s\" не найден.";
        if (strcmp(lang_code, "zh") == 0) return "%s:%d: 未找到标签 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "%s:%d: 找不到標籤 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "%s:%d: タグ \"%s\" がみつかりません。";
        return "%s:%d: Tag \"%s\" not found.";
    }
    if (strcmp(msg, "%s:%d: \"Tag_%s\" is not a function.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "%s:%d: \"Tag_%s\" no es una función.";
        if (strcmp(lang_code, "fr") == 0) return "%s:%d : \"Tag_%s\" n'est pas une fonction.";
        if (strcmp(lang_code, "it") == 0) return "%s:%d: \"Tag_%s\" non è una funzione.";
        if (strcmp(lang_code, "de") == 0) return "%s:%d: \"Tag_%s\" ist keine Funktion.";
        if (strcmp(lang_code, "el") == 0) return "%s:%d: Το \"Tag_%s\" δεν είναι συνάρτηση.";
        if (strcmp(lang_code, "ru") == 0) return "%s:%d: «Tag_%s» не является функцией.";
        if (strcmp(lang_code, "zh") == 0) return "%s:%d: \"Tag_%s\" 不是函数。";
        if (strcmp(lang_code, "tw") == 0) return "%s:%d: \"Tag_%s\" 不是函式。";
        if (strcmp(lang_code, "ja") == 0) return "%s:%d: \"Tag_%s\" は関数ではありません。";
        return "%s:%d: \"Tag_%s\" is not a function.";
    }
    if (strcmp(msg, "Argument \"%s\" not specified.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se ha especificado el argumento \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Argument \"%s\" non spécifié.";
        if (strcmp(lang_code, "it") == 0) return "Argomento \"%s\" non specificato.";
        if (strcmp(lang_code, "de") == 0) return "Argument \"%s\" nicht angegeben.";
        if (strcmp(lang_code, "el") == 0) return "Το όρισμα \"%s\" δεν έχει οριστεί.";
        if (strcmp(lang_code, "ru") == 0) return "Аргумент «%s» не указан.";
        if (strcmp(lang_code, "zh") == 0) return "未指定参数 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "未指定引數 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "引数 \"%s\" が指定されていません。";
        return "Argument \"%s\" not specified.";
    }
    if (strcmp(msg, "Cannot play sound file \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se puede reproducir el archivo de sonido \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Impossible de lire le fichier sonore \"%s\".";
        if (strcmp(lang_code, "it") == 0) return "Impossibile riprodurre il file audio \"%s\".";
        if (strcmp(lang_code, "de") == 0) return "Sounddatei \"%s\" kann nicht abgespielt werden.";
        if (strcmp(lang_code, "el") == 0) return "Δεν είναι δυνατή η αναπαραγωγή του αρχείου ήχου \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Невозможно воспроизвести звуковой файл «%s».";
        if (strcmp(lang_code, "zh") == 0) return "无法播放音频文件 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "無法播放音訊檔案 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "サウンドファイル \"%s\" を再生できません。";
        return "Cannot play sound file \"%s\".";
    }
    if (strcmp(msg, "Conifg \"%s\" not found.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se encontró la configuración \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Configuration \"%s\" introuvable.";
        if (strcmp(lang_code, "it") == 0) return "Configurazione \"%s\" non trovata.";
        if (strcmp(lang_code, "de") == 0) return "Konfiguration \"%s\" nicht gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Η ρύθμιση \"%s\" δεν βρέθηκε.";
        if (strcmp(lang_code, "ru") == 0) return "Конфигурация «%s» не найдена.";
        if (strcmp(lang_code, "zh") == 0) return "未找到配置 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "找不到設定 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "設定 \"%s\" が見つかりません。";
        return "Conifg \"%s\" not found.";
    }
    if (strcmp(msg, "Duplicated config key %s.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Clave de configuración duplicada %s.";
        if (strcmp(lang_code, "fr") == 0) return "Clé de configuration en double %s.";
        if (strcmp(lang_code, "it") == 0) return "Chiave di configurazione duplicata %s.";
        if (strcmp(lang_code, "de") == 0) return "Doppelter Konfigurationsschlüssel %s.";
        if (strcmp(lang_code, "el") == 0) return "Διπλότυπο κλειδί ρύθμισης %s.";
        if (strcmp(lang_code, "ru") == 0) return "Дублирующийся ключ конфигурации %s.";
        if (strcmp(lang_code, "zh") == 0) return "配置键 %s 重复。";
        if (strcmp(lang_code, "tw") == 0) return "設定鍵 %s 重複。";
        if (strcmp(lang_code, "ja") == 0) return "設定キー %s が重複しています。";
        return "Duplicated config key %s.";
    }
    if (strcmp(msg, "Error loading GUI file %s.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Error al cargar el archivo GUI %s.";
        if (strcmp(lang_code, "fr") == 0) return "Erreur lors du chargement du fichier GUI %s.";
        if (strcmp(lang_code, "it") == 0) return "Errore durante il caricamento del file GUI %s.";
        if (strcmp(lang_code, "de") == 0) return "Fehler beim Laden der GUI-Datei %s.";
        if (strcmp(lang_code, "el") == 0) return "Σφάλμα κατά τη φόρτωση του αρχείου GUI %s.";
        if (strcmp(lang_code, "ru") == 0) return "Ошибка загрузки GUI-файла %s.";
        if (strcmp(lang_code, "zh") == 0) return "加载 GUI 文件 %s 時出错。";
        if (strcmp(lang_code, "tw") == 0) return "載入 GUI 檔案 %s 時發生錯誤。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル %s の読み込み中にエラーが発生しました。";
        return "Error loading GUI file %s.";
    }
    if (strcmp(msg, "Failed to write save data.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Error al escribir los datos de guardado.";
        if (strcmp(lang_code, "fr") == 0) return "Échec de l'écriture des données de sauvegarde.";
        if (strcmp(lang_code, "it") == 0) return "Scrittura dei dati di salvataggio fallita.";
        if (strcmp(lang_code, "de") == 0) return "Fehler beim Schreiben der Spieldaten.";
        if (strcmp(lang_code, "el") == 0) return "Αποτυχία εγγραφής δεδομένων αποθήκευσης.";
        if (strcmp(lang_code, "ru") == 0) return "Ошибка записи данных сохранения.";
        if (strcmp(lang_code, "zh") == 0) return "保存数据写入失败。";
        if (strcmp(lang_code, "tw") == 0) return "儲存存檔資料失敗。";
        if (strcmp(lang_code, "ja") == 0) return "セーブデータの書き込みに失敗しました。";
        return "Failed to write save data.";
    }
    if (strcmp(msg, "File \"%s\" not exist.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "El archivo \"%s\" no existe.";
        if (strcmp(lang_code, "fr") == 0) return "Le fichier \"%s\" n'existe pas.";
        if (strcmp(lang_code, "it") == 0) return "Il file \"%s\" non esiste.";
        if (strcmp(lang_code, "de") == 0) return "Datei \"%s\" existiert nicht.";
        if (strcmp(lang_code, "el") == 0) return "Το αρχείο \"%s\" δεν υπάρχει.";
        if (strcmp(lang_code, "ru") == 0) return "Файл «%s» не существует.";
        if (strcmp(lang_code, "zh") == 0) return "文件 \"%s\" 不存在。";
        if (strcmp(lang_code, "tw") == 0) return "檔案 \"%s\" 不存在。";
        if (strcmp(lang_code, "ja") == 0) return "ファイル \"%s\" がありません。";
        return "File \"%s\" not exist.";
    }
    if (strcmp(msg, "Internal error.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Error interno.";
        if (strcmp(lang_code, "fr") == 0) return "Erreur interne.";
        if (strcmp(lang_code, "it") == 0) return "Errore interno.";
        if (strcmp(lang_code, "de") == 0) return "Interner Fehler.";
        if (strcmp(lang_code, "el") == 0) return "Εσωτερικό σφάλμα.";
        if (strcmp(lang_code, "ru") == 0) return "Внутренняя ошибка.";
        if (strcmp(lang_code, "zh") == 0) return "内部错误。";
        if (strcmp(lang_code, "tw") == 0) return "內部錯誤。";
        if (strcmp(lang_code, "ja") == 0) return "内部エラーが発生しました。";
        return "Internal error.";
    }
    if (strcmp(msg, "Invalid EOF on GUI file %s.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "EOF no válido en el archivo GUI %s.";
        if (strcmp(lang_code, "fr") == 0) return "Fin de fichier (EOF) non valide dans le fichier GUI %s.";
        if (strcmp(lang_code, "it") == 0) return "EOF non valido nel file GUI %s.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiges Dateiende (EOF) in GUI-Datei %s.";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρο τέλος αρχείου (EOF) στο αρχείο GUI %s.";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый EOF в GUI-файле %s.";
        if (strcmp(lang_code, "zh") == 0) return "GUI 文件 %s 中的 EOF 无效。";
        if (strcmp(lang_code, "tw") == 0) return "GUI 檔案 %s 中的 EOF 無效。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル %s の終端が不正です。";
        return "Invalid EOF on GUI file %s.";
    }
    if (strcmp(msg, "Invalid EOF while parsing anime file %s") == 0) {
        if (strcmp(lang_code, "es") == 0) return "EOF no válido al analizar el archivo de animación %s";
        if (strcmp(lang_code, "fr") == 0) return "Fin de fichier (EOF) non valide lors de l'analyse du fichier d'animation %s";
        if (strcmp(lang_code, "it") == 0) return "EOF non valido durante l'analisi del file di animazione %s";
        if (strcmp(lang_code, "de") == 0) return "Ungültiges Dateiende (EOF) beim Parsen der Animationsdatei %s";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρο τέλος αρχείου (EOF) κατά την ανάλυση του αρχείου κινουμένων σχεδίων %s";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый EOF при анализе файла анимации %s";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 %s 時遇到无效的 EOF。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 %s 時遇到無效的 EOF。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル %s の解析中に不正な終端に達しました。";
        return "Invalid EOF while parsing anime file %s";
    }
    if (strcmp(msg, "Invalid accel type \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Tipo de aceleración \"%s\" no válido.";
        if (strcmp(lang_code, "fr") == 0) return "Type d'accélération \"%s\" non valide.";
        if (strcmp(lang_code, "it") == 0) return "Tipo di accelerazione \"%s\" non valido.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiger Beschleunigungstyp \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρος τύπος επιτάχυνσης \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый тип ускорения «%s».";
        if (strcmp(lang_code, "zh") == 0) return "加速类型 \"%s\" 无效。";
        if (strcmp(lang_code, "tw") == 0) return "加速類型 \"%s\" 無效。";
        if (strcmp(lang_code, "ja") == 0) return "加速タイプ \"%s\" は無効です。";
        return "Invalid accel type \"%s\".";
    }
    if (strcmp(msg, "Invalid character \"%c\" while parsing anime file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Carácter \"%c\" no válido al analizar el archivo de animación \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Caractère \"%c\" non valide lors de l'analyse du fichier d'animation \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Carattere \"%c\" non valido durante l'analisi del file di animazione \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiges Zeichen \"%c\" beim Parsen der Animationsdatei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρος χαρακτήρας \"%c\" κατά την ανάλυση του αρχείου κινουμένων σχεδίων \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый символ «%c» при анализе файла анимации «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 \"%s\" 第 %d 行時发现无效字符 \"%c\"。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 \"%s\" 第 %d 行時發現無效字元 \"%c\"。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル \"%s\" の %d 行目を解析中に無効な文字 \"%c\" が見つかりました。";
        return "Invalid character \"%c\" while parsing anime file \"%s\" line %d.";
    }
    if (strcmp(msg, "Invalid character.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Carácter no válido.";
        if (strcmp(lang_code, "fr") == 0) return "Caractère non valide.";
        if (strcmp(lang_code, "it") == 0) return "Carattere non valido.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiges Zeichen.";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρος χαρακτήρας.";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый символ.";
        if (strcmp(lang_code, "zh") == 0) return "无效字符。";
        if (strcmp(lang_code, "tw") == 0) return "無效字元。";
        if (strcmp(lang_code, "ja") == 0) return "文字が不正です。";
        return "Invalid character.";
    }
    if (strcmp(msg, "Invalid fade method \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Método de desvanecimiento \"%s\" no válido.";
        if (strcmp(lang_code, "fr") == 0) return "Méthode de fondu \"%s\" non valide.";
        if (strcmp(lang_code, "it") == 0) return "Metodo di dissolvenza \"%s\" non valido.";
        if (strcmp(lang_code, "de") == 0) return "Ungültige Fade-Methode \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρη μέθοδος εξασθένισης \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый метод затухания «%s».";
        if (strcmp(lang_code, "zh") == 0) return "渐变方法 \"%s\" 无效。";
        if (strcmp(lang_code, "tw") == 0) return "漸變方法 \"%s\" 無效。";
        if (strcmp(lang_code, "ja") == 0) return "フェードメソッド \"%s\" は無効です。";
        return "Invalid fade method \"%s\".";
    }
    if (strcmp(msg, "Invalid fade type \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Tipo de desvanecimiento \"%s\" no válido.";
        if (strcmp(lang_code, "fr") == 0) return "Type de fondu \"%s\" non valide.";
        if (strcmp(lang_code, "it") == 0) return "Tipo di dissolvenza \"%s\" non valido.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiger Fade-Typ \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρος τύπος εξασθένισης \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимый тип затухания «%s».";
        if (strcmp(lang_code, "zh") == 0) return "渐变类型 \"%s\" 无效。";
        if (strcmp(lang_code, "tw") == 0) return "漸變類型 \"%s\" 無效。";
        if (strcmp(lang_code, "ja") == 0) return "フェードタイプ \"%s\" は無効です。";
        return "Invalid fade type \"%s\".";
    }
    if (strcmp(msg, "Invalid layer name \"%s\" while parsing anime file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Nombre de capa \"%s\" no válido al analizar el archivo de animación \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Nom de calque \"%s\" non valide lors de l'analyse du fichier d'animation \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Nome livello \"%s\" non valido durante l'analisi del file di animazione \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiger Ebenenname \"%s\" beim Parsen der Animationsdatei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρο όνομα επιπέδου \"%s\" κατά την ανάλυση του αρχείου κινουμένων σχεδίων \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимое имя слоя «%s» при анализе файла анимации «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 \"%s\" 第 %d 行時发现无效图层名 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 \"%s\" 第 %d 行時發現無效圖層名稱 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル \"%s\" の %d 行目を解析中に無効なレイヤー名 \"%s\" が見つかりました。";
        return "Invalid layer name \"%s\" while parsing anime file \"%s\" line %d.";
    }
    if (strcmp(msg, "Invalid layer name \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Nombre de capa \"%s\" no válido.";
        if (strcmp(lang_code, "fr") == 0) return "Nom de calque \"%s\" non valide.";
        if (strcmp(lang_code, "it") == 0) return "Nome livello \"%s\" non valido.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiger Ebenenname \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρο όνομα επιπέδου \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимое имя слоя «%s».";
        if (strcmp(lang_code, "zh") == 0) return "图层名 \"%s\" 无效。";
        if (strcmp(lang_code, "tw") == 0) return "圖層名稱 \"%s\" 無效。";
        if (strcmp(lang_code, "ja") == 0) return "レイヤー名 \"%s\" は無効です。";
        return "Invalid layer name \"%s\".";
    }
    if (strcmp(msg, "Invalid registered anime \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Animación registrada \"%s\" no válida.";
        if (strcmp(lang_code, "fr") == 0) return "Animation enregistrée \"%s\" non valide.";
        if (strcmp(lang_code, "it") == 0) return "Animazione registrata \"%s\" non valida.";
        if (strcmp(lang_code, "de") == 0) return "Ungültige registrierte Animation \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρη εγγεγραμμένη κίνηση \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимая зарегистрированная анимация «%s».";
        if (strcmp(lang_code, "zh") == 0) return "注册动画 \"%s\" 无效。";
        if (strcmp(lang_code, "tw") == 0) return "註冊動畫 \"%s\" 無效。";
        if (strcmp(lang_code, "ja") == 0) return "登録されているアニメーション \"%s\" は無効です。";
        return "Invalid registered anime \"%s\".";
    }
    if (strcmp(msg, "Invalid track \"%s\" specified.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Pista \"%s\" especificada no válida.";
        if (strcmp(lang_code, "fr") == 0) return "Piste \"%s\" spécifiée non valide.";
        if (strcmp(lang_code, "it") == 0) return "Traccia \"%s\" specificata non valida.";
        if (strcmp(lang_code, "de") == 0) return "Ungültiger Track \"%s\" angegeben.";
        if (strcmp(lang_code, "el") == 0) return "Ορίστηκε μη έγκυρο κομμάτι \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Указана недопустимая дорожка «%s».";
        if (strcmp(lang_code, "zh") == 0) return "指定的轨道 \"%s\" 无效。";
        if (strcmp(lang_code, "tw") == 0) return "指定的軌道 \"%s\" 無效。";
        if (strcmp(lang_code, "ja") == 0) return "トラック \"%s\" の指定が不正です。";
        return "Invalid track \"%s\" specified.";
    }
    if (strcmp(msg, "Label \"%s\" not found.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se encontró la etiqueta \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Étiquette \"%s\" introuvable.";
        if (strcmp(lang_code, "it") == 0) return "Etichetta \"%s\" non trovata.";
        if (strcmp(lang_code, "de") == 0) return "Label \"%s\" wurde nicht gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Η ετικέτα \"%s\" δεν βρέθηκε.";
        if (strcmp(lang_code, "ru") == 0) return "Метка «%s» не найдена.";
        if (strcmp(lang_code, "zh") == 0) return "未找到标签 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "找不到標籤 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "ラベル \"%s\" が見つかりません。";
        return "Label \"%s\" not found.";
    }
    if (strcmp(msg, "Layer not specifed while parsing anime file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se especificó la capa al analizar el archivo de animación \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Calque non spécifié lors de l'analyse du fichier d'animation \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Livello non specificato durante l'analisi del file di animazione \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Ebene nicht angegeben beim Parsen der Animationsdatei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Δεν ορίστηκε επίπεδο κατά την ανάλυση του αρχείου κινουμένων σχεδίων \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Слой не указан при анализе файла анимации «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 \"%s\" 第 %d 行時未指定图层。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 \"%s\" 第 %d 行時未指定圖層。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル \"%s\" の %d 行目を解析中にレイヤーが指定されていません。";
        return "Layer not specifed while parsing anime file \"%s\" line %d.";
    }
    if (strcmp(msg, "Macro \"%s\" not found.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se encontró la macro \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Macro \"%s\" introuvable.";
        if (strcmp(lang_code, "it") == 0) return "Macro \"%s\" non trovata.";
        if (strcmp(lang_code, "de") == 0) return "Makro \"%s\" wurde nicht gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Η μακροεντολή \"%s\" δεν βρέθηκε.";
        if (strcmp(lang_code, "ru") == 0) return "Макрос «%s» не найден.";
        if (strcmp(lang_code, "zh") == 0) return "未找到宏 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "找不到巨集 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "マクロ \"%s\" が見つかりません。";
        return "Macro \"%s\" not found.";
    }
    if (strcmp(msg, "No LHS specified.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se ha especificado LHS (lado izquierdo).";
        if (strcmp(lang_code, "fr") == 0) return "Aucun membre de gauche (LHS) spécifié.";
        if (strcmp(lang_code, "it") == 0) return "Nessun LHS specificato.";
        if (strcmp(lang_code, "de") == 0) return "Kein LHS angegeben.";
        if (strcmp(lang_code, "el") == 0) return "Δεν ορίστηκε αριστερό μέλος (LHS).";
        if (strcmp(lang_code, "ru") == 0) return "Левая часть (LHS) не указана.";
        if (strcmp(lang_code, "zh") == 0) return "未指定左值。";
        if (strcmp(lang_code, "tw") == 0) return "未指定左值。";
        if (strcmp(lang_code, "ja") == 0) return "左辺が指定されていません。";
        return "No LHS specified.";
    }
    if (strcmp(msg, "No RHS specified.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se ha especificado RHS (lado derecho).";
        if (strcmp(lang_code, "fr") == 0) return "Aucun membre de droite (RHS) spécifié.";
        if (strcmp(lang_code, "it") == 0) return "Nessun RHS specificato.";
        if (strcmp(lang_code, "de") == 0) return "Kein RHS angegeben.";
        if (strcmp(lang_code, "el") == 0) return "Δεν ορίστηκε δεξί μέλος (RHS).";
        if (strcmp(lang_code, "ru") == 0) return "Правая часть (RHS) не указана.";
        if (strcmp(lang_code, "zh") == 0) return "未指定右值。";
        if (strcmp(lang_code, "tw") == 0) return "未指定右值。";
        if (strcmp(lang_code, "ja") == 0) return "右辺が指定されていません。";
        return "No RHS specified.";
    }
    if (strcmp(msg, "No available button-id for a GUI button in GUI file \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No hay ID de botón disponible para un botón GUI en el archivo GUI \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Aucun ID de bouton disponible pour un bouton GUI dans le fichier GUI \"%s\".";
        if (strcmp(lang_code, "it") == 0) return "Nessun ID pulsante disponibile per un pulsante GUI nel file GUI \"%s\".";
        if (strcmp(lang_code, "de") == 0) return "Keine verfügbare Button-ID für einen GUI-Button in GUI-Datei \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Δεν υπάρχει διαθέσιμο ID κουμπιού για ένα κουμπί GUI στο αρχείο GUI \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Нет доступного ID кнопки для GUI-кнопки в GUI-файле «%s».";
        if (strcmp(lang_code, "zh") == 0) return "GUI 文件 \"%s\" 中没有可用的按钮 ID。";
        if (strcmp(lang_code, "tw") == 0) return "GUI 檔案 \"%s\" 中沒有可用的按鈕 ID。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル \"%s\" に使用可能なボタンIDがありません。";
        return "No available button-id for a GUI button in GUI file \"%s\".";
    }
    if (strcmp(msg, "No correspoinding \"for\" detected.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se detectó el \"for\" correspondiente.";
        if (strcmp(lang_code, "fr") == 0) return "Aucun \"for\" correspondant détecté.";
        if (strcmp(lang_code, "it") == 0) return "Nessun \"for\" corrispondente rilevato.";
        if (strcmp(lang_code, "de") == 0) return "Kein entsprechendes \"for\" gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Δεν εντοπίστηκε αντίστοιχο \"for\".";
        if (strcmp(lang_code, "ru") == 0) return "Не обнаружен соответствующий цикл «for».";
        if (strcmp(lang_code, "zh") == 0) return "未发现对应的 \"for\"。";
        if (strcmp(lang_code, "tw") == 0) return "找不到對應的 \"for\"。";
        if (strcmp(lang_code, "ja") == 0) return "対応する \"for\" がありません。";
        return "No correspoinding \"for\" detected.";
    }
    if (strcmp(msg, "No correspoinding \"if\" detected.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se detectó el \"if\" correspondiente.";
        if (strcmp(lang_code, "fr") == 0) return "Aucun \"if\" correspondant détecté.";
        if (strcmp(lang_code, "it") == 0) return "Nessun \"if\" corrispondente rilevato.";
        if (strcmp(lang_code, "de") == 0) return "Kein entsprechendes \"if\" gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Δεν εντοπίστηκε αντίστοιχο \"if\".";
        if (strcmp(lang_code, "ru") == 0) return "Не обнаружено соответствующее условие «if».";
        if (strcmp(lang_code, "zh") == 0) return "未发现对应的 \"if\"。";
        if (strcmp(lang_code, "tw") == 0) return "找不到對應的 \"if\"。";
        if (strcmp(lang_code, "ja") == 0) return "対応する \"if\" がありません。";
        return "No correspoinding \"if\" detected.";
    }
    if (strcmp(msg, "No correspoinding \"while\" detected.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se detectó el \"while\" correspondiente.";
        if (strcmp(lang_code, "fr") == 0) return "Aucun \"while\" correspondant détecté.";
        if (strcmp(lang_code, "it") == 0) return "Nessun \"while\" corrispondente rilevato.";
        if (strcmp(lang_code, "de") == 0) return "Kein entsprechendes \"while\" gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Δεν εντοπίστηκε αντίστοιχο \"while\".";
        if (strcmp(lang_code, "ru") == 0) return "Не обнаружен соответствующий цикл «while».";
        if (strcmp(lang_code, "zh") == 0) return "未发现对应的 \"while\"。";
        if (strcmp(lang_code, "tw") == 0) return "找不到對應的 \"while\"。";
        if (strcmp(lang_code, "ja") == 0) return "対応する \"while\" がありません。";
        return "No correspoinding \"while\" detected.";
    }
    if (strcmp(msg, "No matching endif found.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se encontró un endif correspondiente.";
        if (strcmp(lang_code, "fr") == 0) return "Aucun endif correspondant trouvé.";
        if (strcmp(lang_code, "it") == 0) return "Nessun endif corrispondente trovato.";
        if (strcmp(lang_code, "de") == 0) return "Kein passendes endif gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Δεν βρέθηκε αντίστοιχο endif.";
        if (strcmp(lang_code, "ru") == 0) return "Соответствующий endif не найден.";
        if (strcmp(lang_code, "zh") == 0) return "未找到匹配的 endif。";
        if (strcmp(lang_code, "tw") == 0) return "找不到對應的 endif。";
        if (strcmp(lang_code, "ja") == 0) return "endif が見つかりません。";
        return "No matching endif found.";
    }
    if (strcmp(msg, "No matching endmacro found.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se encontró un endmacro correspondiente.";
        if (strcmp(lang_code, "fr") == 0) return "Aucun endmacro correspondant trouvé.";
        if (strcmp(lang_code, "it") == 0) return "Nessun endmacro corrispondente trovato.";
        if (strcmp(lang_code, "de") == 0) return "Kein passendes endmacro gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Δεν βρέθηκε αντίστοιχο endmacro.";
        if (strcmp(lang_code, "ru") == 0) return "Соответствующий endmacro не найден.";
        if (strcmp(lang_code, "zh") == 0) return "未找到匹配的 endmacro。";
        if (strcmp(lang_code, "tw") == 0) return "找不到對應的 endmacro。";
        if (strcmp(lang_code, "ja") == 0) return "endmacro が見つかりません。";
        return "No matching endmacro found.";
    }
    if (strcmp(msg, "No operator specified.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se especificó ningún operador.";
        if (strcmp(lang_code, "fr") == 0) return "Aucun opérateur spécifié.";
        if (strcmp(lang_code, "it") == 0) return "Nessun operatore specificato.";
        if (strcmp(lang_code, "de") == 0) return "Kein Operator angegeben.";
        if (strcmp(lang_code, "el") == 0) return "Δεν ορίστηκε τελεστής.";
        if (strcmp(lang_code, "ru") == 0) return "Оператор не указан.";
        if (strcmp(lang_code, "zh") == 0) return "未指定操作符。";
        if (strcmp(lang_code, "tw") == 0) return "未指定運算子。";
        if (strcmp(lang_code, "ja") == 0) return "演算子が指定されていません。";
        return "No operator specified.";
    }
    if (strcmp(msg, "Property name too long.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Nombre de propiedad demasiado largo.";
        if (strcmp(lang_code, "fr") == 0) return "Nom de propriété trop long.";
        if (strcmp(lang_code, "it") == 0) return "Nome della proprietà troppo lungo.";
        if (strcmp(lang_code, "de") == 0) return "Eigenschaftsname zu lang.";
        if (strcmp(lang_code, "el") == 0) return "Το όνομα της ιδιότητας είναι πολύ μεγάλο.";
        if (strcmp(lang_code, "ru") == 0) return "Имя свойства слишком длинное.";
        if (strcmp(lang_code, "zh") == 0) return "属性名过长。";
        if (strcmp(lang_code, "tw") == 0) return "屬性名稱過長。";
        if (strcmp(lang_code, "ja") == 0) return "プロパティ名が長すぎます。";
        return "Property name too long.";
    }
    if (strcmp(msg, "Property value too long.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Valor de propiedad demasiado largo.";
        if (strcmp(lang_code, "fr") == 0) return "Valeur de propriété trop longue.";
        if (strcmp(lang_code, "it") == 0) return "Valore della proprietà troppo lungo.";
        if (strcmp(lang_code, "de") == 0) return "Eigenschaftswert zu lang.";
        if (strcmp(lang_code, "el") == 0) return "Η τιμή της ιδιότητας είναι πολύ μεγάλη.";
        if (strcmp(lang_code, "ru") == 0) return "Значение свойства слишком длинное.";
        if (strcmp(lang_code, "zh") == 0) return "属性值过长。";
        if (strcmp(lang_code, "tw") == 0) return "屬性值過長。";
        if (strcmp(lang_code, "ja") == 0) return "プロパティ値が長すぎます。";
        return "Property value too long.";
    }
    if (strcmp(msg, "Rule file is missing.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Falta el archivo de reglas.";
        if (strcmp(lang_code, "fr") == 0) return "Le fichier de règles est manquant.";
        if (strcmp(lang_code, "it") == 0) return "Il file delle regole è mancante.";
        if (strcmp(lang_code, "de") == 0) return "Regeldatei fehlt.";
        if (strcmp(lang_code, "el") == 0) return "Το αρχείο κανόνων λείπει.";
        if (strcmp(lang_code, "ru") == 0) return "Файл правил отсутствует.";
        if (strcmp(lang_code, "zh") == 0) return "缺少规则文件。";
        if (strcmp(lang_code, "tw") == 0) return "缺少規則檔案。";
        if (strcmp(lang_code, "ja") == 0) return "ルールファイルがありません。";
        return "Rule file is missing.";
    }
    if (strcmp(msg, "Rule file is not specified.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "No se ha especificado el archivo de reglas.";
        if (strcmp(lang_code, "fr") == 0) return "Le fichier de règles n'est pas spécifié.";
        if (strcmp(lang_code, "it") == 0) return "Il file delle regole non è specificato.";
        if (strcmp(lang_code, "de") == 0) return "Regeldatei nicht angegeben.";
        if (strcmp(lang_code, "el") == 0) return "Δεν ορίστηκε αρχείο κανόνων.";
        if (strcmp(lang_code, "ru") == 0) return "Файл правил не указан.";
        if (strcmp(lang_code, "zh") == 0) return "未指定规则文件。";
        if (strcmp(lang_code, "tw") == 0) return "未指定規則檔案。";
        if (strcmp(lang_code, "ja") == 0) return "ルールファイルが指定されていません。";
        return "Rule file is not specified.";
    }
    if (strcmp(msg, "Save data version mismatched.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "La versión de los datos de guardado no coincide.";
        if (strcmp(lang_code, "fr") == 0) return "La version des données de sauvegarde ne correspond pas.";
        if (strcmp(lang_code, "it") == 0) return "Versione dei dati di salvataggio non corrispondente.";
        if (strcmp(lang_code, "de") == 0) return "Version der Spieldaten stimmt nicht überein.";
        if (strcmp(lang_code, "el") == 0) return "Αναντιστοιχία έκδοσης δεδομένων αποθήκευσης.";
        if (strcmp(lang_code, "ru") == 0) return "Версия данных сохранения не совпадает.";
        if (strcmp(lang_code, "zh") == 0) return "存档数据版本不匹配。";
        if (strcmp(lang_code, "tw") == 0) return "存檔版本不符。";
        if (strcmp(lang_code, "ja") == 0) return "セーブデータのバージョンが一致しません。";
        return "Save data version mismatched.";
    }
    if (strcmp(msg, "Sequece too long while parsing anime file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Secuencia demasiado larga al analizar el archivo de animación \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Séquence trop longue lors de l'analyse du fichier d'animation \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Sequenza troppo lunga durante l'analisi del file di animazione \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Sequenz zu lang beim Parsen der Animationsdatei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Η σειρά είναι πολύ μεγάλη κατά την ανάλυση του αρχείου κινουμένων σχεδίων \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком длинная последовательность при анализе файла анимации «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 \"%s\" 第 %d 行時序列过长。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 \"%s\" 第 %d 行時序列過長。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル \"%s\" の %d 行目を解析中にシーケンスが長すぎます。";
        return "Sequece too long while parsing anime file \"%s\" line %d.";
    }
    if (strcmp(msg, "Specified ID is already used in GUI file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "El ID especificado ya se está usando en el archivo GUI \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "L'ID spécifié est déjà utilisé dans le fichier GUI \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "L'ID specificato è già in uso nel file GUI \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Die angegebene ID wird bereits in der GUI-Datei \"%s\" Zeile %d verwendet.";
        if (strcmp(lang_code, "el") == 0) return "Το καθορισμένο ID χρησιμοποιείται ήδη στο αρχείο GUI \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Указанный ID уже используется в GUI-файле «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "指定的 ID 已在 GUI 文件 \"%s\" 第 %d 行中使用。";
        if (strcmp(lang_code, "tw") == 0) return "指定的 ID 已在 GUI 檔案 \"%s\" 第 %d 行中使用。";
        if (strcmp(lang_code, "ja") == 0) return "指定されたIDはGUIファイル \"%s\" の %d 行目ですでに使用されています。";
        return "Specified ID is already used in GUI file \"%s\" line %d.";
    }
    if (strcmp(msg, "Tag name too long.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Nombre de etiqueta demasiado largo.";
        if (strcmp(lang_code, "fr") == 0) return "Nom de balise trop long.";
        if (strcmp(lang_code, "it") == 0) return "Nome del tag troppo lungo.";
        if (strcmp(lang_code, "de") == 0) return "Tag-Name zu lang.";
        if (strcmp(lang_code, "el") == 0) return "Το όνομα της ετικέτας είναι πολύ μεγάλο.";
        if (strcmp(lang_code, "ru") == 0) return "Имя тега слишком длинное.";
        if (strcmp(lang_code, "zh") == 0) return "标签名过长。";
        if (strcmp(lang_code, "tw") == 0) return "標籤名稱過長。";
        if (strcmp(lang_code, "ja") == 0) return "タグ名が長すぎます。";
        return "Tag name too long.";
    }
    if (strcmp(msg, "This API is not implemented yet.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Esta API aún no se ha implementado.";
        if (strcmp(lang_code, "fr") == 0) return "Cette API n'est pas encore implémentée.";
        if (strcmp(lang_code, "it") == 0) return "Questa API non è ancora implementata.";
        if (strcmp(lang_code, "de") == 0) return "Diese API ist noch nicht implementiert.";
        if (strcmp(lang_code, "el") == 0) return "Αυτό το API δεν έχει υλοποιηθεί ακόμα.";
        if (strcmp(lang_code, "ru") == 0) return "Этот API еще не реализован.";
        if (strcmp(lang_code, "zh") == 0) return "该 API 尚未实现。";
        if (strcmp(lang_code, "tw") == 0) return "此 API 尚未實作。";
        if (strcmp(lang_code, "ja") == 0) return "このAPIはまだ実装されていません。";
        return "This API is not implemented yet.";
    }
    if (strcmp(msg, "Too long word while parsing anime file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Palabra demasiado larga al analizar el archivo de animación \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Mot trop long lors de l'analyse du fichier d'animation \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Parola troppo lunga durante l'analisi del file di animazione \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Wort zu lang beim Parsen der Animationsdatei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Η λέξη είναι πολύ μεγάλη κατά την ανάλυση του αρχείου κινουμένων σχεδίων \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком длинное слово при анализе файла анимации «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 \"%s\" 第 %d 行時单词过长。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 \"%s\" 第 %d 行時單字過長。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル \"%s\" の %d 行目を解析中に単語が長すぎます。";
        return "Too long word while parsing anime file \"%s\" line %d.";
    }
    if (strcmp(msg, "Too many GUI buttons in GUI file \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Demasiados botones GUI en el archivo GUI \"%s\".";
        if (strcmp(lang_code, "fr") == 0) return "Trop de boutons GUI dans le fichier GUI \"%s\".";
        if (strcmp(lang_code, "it") == 0) return "Troppi pulsanti GUI nel file GUI \"%s\".";
        if (strcmp(lang_code, "de") == 0) return "Zu viele GUI-Buttons in GUI-Datei \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Πάρα πολλά κουμπιά GUI στο αρχείο GUI \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Слишком много GUI-кнопок в GUI-файле «%s».";
        if (strcmp(lang_code, "zh") == 0) return "GUI 文件 \"%s\" 中的 GUI 按钮过多。";
        if (strcmp(lang_code, "tw") == 0) return "GUI 檔案 \"%s\" 中的 GUI 按鈕過多。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル \"%s\" のGUIボタンが多すぎます。";
        return "Too many GUI buttons in GUI file \"%s\".";
    }
    if (strcmp(msg, "Too many drawmsg contexts.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Demasiados contextos drawmsg.";
        if (strcmp(lang_code, "fr") == 0) return "Trop de contextes drawmsg.";
        if (strcmp(lang_code, "it") == 0) return "Troppi contesti drawmsg.";
        if (strcmp(lang_code, "de") == 0) return "Zu viele drawmsg-Kontexte.";
        if (strcmp(lang_code, "el") == 0) return "Πάρα πολλά περιβάλλοντα drawmsg.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком много контекстов drawmsg.";
        if (strcmp(lang_code, "zh") == 0) return "drawmsg 上下文过多。";
        if (strcmp(lang_code, "tw") == 0) return "drawmsg 上下文過多。";
        if (strcmp(lang_code, "ja") == 0) return "drawmsg コンテキストが多すぎます。";
        return "Too many drawmsg contexts.";
    }
    if (strcmp(msg, "Too many macro calls.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Demasiadas llamadas a macros.";
        if (strcmp(lang_code, "fr") == 0) return "Trop d'appels de macros.";
        if (strcmp(lang_code, "it") == 0) return "Troppe chiamate macro.";
        if (strcmp(lang_code, "de") == 0) return "Zu viele Makroaufrufe.";
        if (strcmp(lang_code, "el") == 0) return "Πάρα πολλές κλήσεις μακροεντολών.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком много вызовов макросов.";
        if (strcmp(lang_code, "zh") == 0) return "宏调用过多。";
        if (strcmp(lang_code, "tw") == 0) return "巨集呼叫過多。";
        if (strcmp(lang_code, "ja") == 0) return "マクロの呼び出し回数が多すぎます。";
        return "Too many macro calls.";
    }
    if (strcmp(msg, "Too many nests.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Demasiados anidamientos.";
        if (strcmp(lang_code, "fr") == 0) return "Trop d'imbrications.";
        if (strcmp(lang_code, "it") == 0) return "Troppi annidamenti.";
        if (strcmp(lang_code, "de") == 0) return "Zu viele Verschachtelungen.";
        if (strcmp(lang_code, "el") == 0) return "Πάρα πολλές εμφωλεύσεις.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком много уровней вложенности.";
        if (strcmp(lang_code, "zh") == 0) return "嵌套过多。";
        if (strcmp(lang_code, "tw") == 0) return "巢狀層級過多。";
        if (strcmp(lang_code, "ja") == 0) return "ネストが深すぎます。";
        return "Too many nests.";
    }
    if (strcmp(msg, "Too many properties.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Demasiadas propiedades.";
        if (strcmp(lang_code, "fr") == 0) return "Trop de propriétés.";
        if (strcmp(lang_code, "it") == 0) return "Troppe proprietà.";
        if (strcmp(lang_code, "de") == 0) return "Zu viele Eigenschaften.";
        if (strcmp(lang_code, "el") == 0) return "Πάρα πολλές ιδιότητες.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком много свойств.";
        if (strcmp(lang_code, "zh") == 0) return "属性过多。";
        if (strcmp(lang_code, "tw") == 0) return "屬性過多。";
        if (strcmp(lang_code, "ja") == 0) return "プロパティが多すぎます。";
        return "Too many properties.";
    }
    if (strcmp(msg, "Too many registered animes.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Demasiadas animaciones registradas.";
        if (strcmp(lang_code, "fr") == 0) return "Trop d'animations enregistrées.";
        if (strcmp(lang_code, "it") == 0) return "Troppe animazioni registrate.";
        if (strcmp(lang_code, "de") == 0) return "Zu viele registrierte Animationen.";
        if (strcmp(lang_code, "el") == 0) return "Πάρα πολλές εγγεγραμμένες κινήσεις.";
        if (strcmp(lang_code, "ru") == 0) return "Слишком много зарегистрированных анимаций.";
        if (strcmp(lang_code, "zh") == 0) return "注册动画过多。";
        if (strcmp(lang_code, "tw") == 0) return "註冊動畫過多。";
        if (strcmp(lang_code, "ja") == 0) return "登録されているアニメーションが多すぎます。";
        return "Too many registered animes.";
    }
    if (strcmp(msg, "Undefined config key \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Clave de configuración \"%s\" no definida.";
        if (strcmp(lang_code, "fr") == 0) return "Clé de configuration \"%s\" non définie.";
        if (strcmp(lang_code, "it") == 0) return "Chiave di configurazione \"%s\" non definita.";
        if (strcmp(lang_code, "de") == 0) return "Undefinierter Konfigurationsschlüssel \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Μη ορισμένο κλειδί ρύθμισης \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Неопределенный ключ конфигурации «%s».";
        if (strcmp(lang_code, "zh") == 0) return "未定义的配置键 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "未定義的設定鍵 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "設定キー \"%s\" は定義されていません。";
        return "Undefined config key \"%s\".";
    }
    if (strcmp(msg, "Unexpected EOF.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Fin de archivo (EOF) inesperado.";
        if (strcmp(lang_code, "fr") == 0) return "Fin de fichier (EOF) inattendue.";
        if (strcmp(lang_code, "it") == 0) return "EOF imprevisto.";
        if (strcmp(lang_code, "de") == 0) return "Unerwartetes Dateiende (EOF).";
        if (strcmp(lang_code, "el") == 0) return "Μη αναμενόμενο τέλος αρχείου (EOF).";
        if (strcmp(lang_code, "ru") == 0) return "Непредвиденный конец файла (EOF).";
        if (strcmp(lang_code, "zh") == 0) return "意外的 EOF。";
        if (strcmp(lang_code, "tw") == 0) return "未預期的 EOF。";
        if (strcmp(lang_code, "ja") == 0) return "予期しないファイル終端です。";
        return "Unexpected EOF.";
    }
    if (strcmp(msg, "Unknown button property \"%s\" found in GUI file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Se encontró una propiedad de botón \"%s\" desconocida en el archivo GUI \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Propriété de bouton \"%s\" inconnue dans le fichier GUI \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Proprietà pulsante \"%s\" sconosciuta nel file GUI \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Unbekannte Button-Eigenschaft \"%s\" in GUI-Datei \"%s\" Zeile %d gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Βρέθηκε άγνωστη ιδιότητα κουμπιού \"%s\" στο αρχείο GUI \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Неизвестное свойство кнопки «%s» в GUI-файле «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "在 GUI 文件 \"%s\" 第 %d 行发现未知的按钮属性 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "在 GUI 檔案 \"%s\" 第 %d 行發現未知的按鈕屬性 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル \"%s\" の %d 行目に未知のボタンプロパティ \"%s\" があります。";
        return "Unknown button property \"%s\" found in GUI file \"%s\" line %d.";
    }
    if (strcmp(msg, "Unknown button type name \"%s\" in GUI file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Nombre de tipo de botón \"%s\" desconocido en el archivo GUI \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Nom de type de bouton \"%s\" inconnu dans le fichier GUI \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Nome tipo pulsante \"%s\" sconosciuto nel file GUI \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Unbekannter Button-Typname \"%s\" in GUI-Datei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Άγνωστο όνομα τύπου κουμπιού \"%s\" στο αρχείο GUI \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Неизвестное имя типа кнопки «%s» в GUI-файле «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "GUI 文件 \"%s\" 第 %d 行发现未知的按钮类型 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "GUI 檔案 \"%s\" 第 %d 行發現未知的按鈕類型 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル \"%s\" の %d 行目に未知のボタンタイプ \"%s\" があります。";
        return "Unknown button type name \"%s\" in GUI file \"%s\" line %d.";
    }
    if (strcmp(msg, "Unknown config key \"%s\".") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Clave de configuración \"%s\" desconocida.";
        if (strcmp(lang_code, "fr") == 0) return "Clé de configuration \"%s\" inconnue.";
        if (strcmp(lang_code, "it") == 0) return "Chiave di configurazione \"%s\" sconosciuta.";
        if (strcmp(lang_code, "de") == 0) return "Unbekannter Konfigurationsschlüssel \"%s\".";
        if (strcmp(lang_code, "el") == 0) return "Άγνωστο κλειδί ρύθμισης \"%s\".";
        if (strcmp(lang_code, "ru") == 0) return "Неизвестный ключ конфигурации «%s».";
        if (strcmp(lang_code, "zh") == 0) return "未知的配置键 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "未知的設定鍵 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "未知の設定キー \"%s\" です。";
        return "Unknown config key \"%s\".";
    }
    if (strcmp(msg, "Unknown global key \"%s\" found in GUI file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Se encontró una clave global \"%s\" desconocida en el archivo GUI \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Clé globale \"%s\" inconnue dans le fichier GUI \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Chiave globale \"%s\" sconosciuta nel file GUI \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Unbekannter globaler Schlüssel \"%s\" in GUI-Datei \"%s\" Zeile %d gefunden.";
        if (strcmp(lang_code, "el") == 0) return "Βρέθηκε άγνωστο παγκόσμιο κλειδί \"%s\" στο αρχείο GUI \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Неизвестный глобальный ключ «%s» в GUI-файле «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "在 GUI 文件 \"%s\" 第 %d 行发现未知的全局键 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "在 GUI 檔案 \"%s\" 第 %d 行發現未知的全域鍵 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "GUIファイル \"%s\" の %d 行目に未知のグローバルキー \"%s\" があります。";
        return "Unknown global key \"%s\" found in GUI file \"%s\" line %d.";
    }
    if (strcmp(msg, "Unkwon key \"%s\" while parsing anime file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Clave \"%s\" desconocida al analizar el archivo de animación \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Clé \"%s\" inconnue lors de l'analyse du fichier d'animation \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Chiave \"%s\" sconosciuta durante l'analisi del file di animazione \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Unbekannter Schlüssel \"%s\" beim Parsen der Animationsdatei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Άγνωστο κλειδί \"%s\" κατά την ανάλυση του αρχείου κινουμένων σχεδίων \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Неизвестный ключ «%s» при анализе файла анимации «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "解析动画文件 \"%s\" 第 %d 行時发现未知键 \"%s\"。";
        if (strcmp(lang_code, "tw") == 0) return "解析動畫檔案 \"%s\" 第 %d 行時發現未知鍵 \"%s\"。";
        if (strcmp(lang_code, "ja") == 0) return "アニメーションファイル \"%s\" の %d 行目を解析中に未知のキー \"%s\" が見つかりました。";
        return "Unkwon key \"%s\" while parsing anime file \"%s\" line %d.";
    }
    if (strcmp(msg, "Invalid config line at file \"%s\" line %d.") == 0) {
        if (strcmp(lang_code, "es") == 0) return "Línea de configuración no válida en el archivo \"%s\" en la línea %d.";
        if (strcmp(lang_code, "fr") == 0) return "Ligne de configuration non valide dans le fichier \"%s\" ligne %d.";
        if (strcmp(lang_code, "it") == 0) return "Linea di configurazione non valida nel file \"%s\" riga %d.";
        if (strcmp(lang_code, "de") == 0) return "Ungültige Konfigurationszeile in Datei \"%s\" Zeile %d.";
        if (strcmp(lang_code, "el") == 0) return "Μη έγκυρη γραμμή ρύθμισης στο αρχείο \"%s\" γραμμή %d.";
        if (strcmp(lang_code, "ru") == 0) return "Недопустимая строка конфигурации в файле «%s» в строке %d.";
        if (strcmp(lang_code, "zh") == 0) return "文件 \"%s\" 第 %d 行配置无效。";
        if (strcmp(lang_code, "tw") == 0) return "檔案 \"%s\" 第 %d 行設定無効。";
        if (strcmp(lang_code, "ja") == 0) return "ファイル \"%s\" の %d 行目の設定が不正です。";
        return "Invalid config line at file \"%s\" line %d.";
    }
    return msg;
}
