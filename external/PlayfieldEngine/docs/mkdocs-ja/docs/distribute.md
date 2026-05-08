ゲームの配布
============

ゲームの準備ができたら、アセットを単一のパッケージファイルにまとめて、配布をかんたんにしましょう。

## assets.arc の作成

### Windows の場合

Playfield Engine に同梱されているパッケージングツールを利用できます：

1. `SDK/windows/` フォルダに行きます
2. すべてのアセット (main.ray, 画像、フォント、音声) を選択して、`playfield-pack.exe` にドロップします
3. ツールが `assets.arc` ファイルを同じディレクトリに作成します

このファイルの中にはすべてゲームアセットが含まれます。

### macOS の場合

Windows の場合と同様ですが、`SDK/macos/playfield-pack` を代わりに使用してください：

1. ターミナルを開きます.
2. `SDK/macos/` に行きます (`cd SDK/macos`)
3. `xattr -c playfield-pack` を実行して、macOS の検閲フラグを消去します
4. `./playfield-pack <files>` を実行して、assets.arc ファイルを作成します

### On Linux

Windows の場合と同様ですが、`SDK/linux/playfield-pack` を代わりに使用してください：

1. ターミナルを開きます.
2. `SDK/linux/` に行きます (`cd SDK/linux`)
4. `./playfield-pack <files>` を実行して、assets.arc ファイルを作成します

## 配布に必要なファイル

ゲームを配布するには、以下の２つのファイルが必要です。

* `playfield.exe`
* `assets.arc`

これらを１つのフォルダに入れてください。
プレーヤーは `playfield.exe` をダブルクリックするだけでゲームを起動できます。

## アイコンの変更

### Windows の場合

アイコンを変更できる何らかのソフトを利用して、`playfield.exe` のアイコンを変更します。

あるいは別の方法として:
1. Playfield Engine のソースコードをダウンロードします
2. `resources/windows/icon.ico` をあなたのアイコンに変更します
3. アプリを再ビルドします

### macOS の場合

1. Playfield Engine のソースコードをダウンロードします
2. `resources/macos/icon512.png` をあなたのアイコン画像に変更します
3. アプリを再ビルドします

### For Linux

1. Playfield のソースコードをダウンロードします
2. `external/StratoHAL/src/icon.c` をあなたのアイコンデータに変更します
  * このファイルはXPM画像のテキストである必要があります
3. アプリを再ビルドします
