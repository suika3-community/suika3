Suika3 ビルド手順
========================

Suika3 は CMake ビルドシステムを全面的に活用しています。

* 注意事項
    * CMake 3.22 以降が必要です。
    * Windows: Visual Studio 2022 または 2026 が必要です。
    * macOS: Xcode 8.2.1 以降が必要です。
    * Linux: gcc 4.4 以降が必要です（clang もサポートされています）。
    * フルビルドにかかる時間:
        * Intel Core i5 13400 上の Visual Studio 2026 で 20 秒
        * Intel Core i5 13400 上の Linux で 5 秒
        * Apple M5 上の macOS で 5 秒

---

## Windows (Visual Studio 2026)

Visual Studio は Windows での推奨ビルド環境であり、公式バイナリでも使用されています。

### 前提条件

* Intel、AMD、または Arm64 プロセッサを搭載した `Windows 11` PC
* C/C++ および CMake が構成された `Visual Studio 2026` がインストールされていること

### 手順

* リポジトリをクローンします。
* Visual Studio でソースコードフォルダのルートを開きます。
* CMake の構成が完了するまで待ちます。
* `VS2026 x64 Release` ターゲットを選択します。
* プロジェクトをビルドします。

ターゲットファイル `out/build/windows-vs2026-x64-release/suika3.exe` が作成されます。

---

## Windows (WSL2)

この方法では WSL2 上の MinGW を使用して Windows 用の exe ファイルを作成します。
MinGW で Suika3 をビルドすることは可能ですが、アンチウイルスソフトウェアによる誤検知の可能性があるため、推奨されません。

### 前提条件

* Intel、AMD、または Arm64 プロセッサを搭載した `Windows 11` PC
* `WSL2` 機能がインストールされていること
* WSL2 上に `Ubuntu` または `Debian` がインストールされていること

```
sudo apt-get install cmake ninja-build mingw-w64
```

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset windows-mingw-x86_64
cmake --build --preset windows-mingw-x86_64
```

ターゲットファイル `build-mingw-x86_64/suika3.exe` が作成されます。

---

## Linux (X11)

### 前提条件

* 任意のプロセッサを搭載した `Linux` マシン
* `X11` がインストールされていること

Debian, Ubuntu, Raspberry Pi OS の場合:
```
sudo apt-get install git cmake ninja-build build-essential libx11-dev libxpm-dev libasound2-dev mesa-common-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev gstreamer1.0-x
```

RedHat, Rocky Linux, Fedora などの場合:
```
sudo dnf groupinstall "Development Tools" "Development Libraries"
sudo dnf install patch git cmake ninja-build libX11-devel libXpm-devel alsa-lib-devel mesa-libGL-devel gstreamer1.0-devel gstreamer1.0-plugins-base-devel 
```

### 手順

ターミナルを開き、以下のように入力します。
```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset linux-x11
cmake --build --preset linux-x11
```

ターゲットファイル `build-linux-x11/suika3` が作成されます。

gdb で Suika3 をデバッグしたい場合は、`linux-x11` の代わりに `linux-x11-debug` プリセットを使用できます。

---

## Linux (Wayland)

Wayland のサポートはまだ実験的であることに注意してください。
KDE とは互換性がありますが、GNOME ではウィンドウ枠が表示されません。

### 前提条件

* 任意のプロセッサを搭載した `Linux` マシン
* `Wayland` がインストールされていること

Debian または Ubuntu の場合:
```
sudo apt-get install git cmake ninja-build build-essential libwayland-dev wayland-protocols libegl1-mesa-dev libasound2-dev
```

RedHat, Rocky Linux, Fedora などの場合:
```
sudo dnf groupinstall "Development Tools" "Development Libraries"
sudo dnf install patch git cmake ninja-build wayland-devel wayland-protocols-devel mesa-libEGL-devel alsa-lib-devel
```

### 手順

ターミナルを開き、以下のように入力します。
```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset linux-wayland
cmake --build --preset linux-wayland
```

ターゲットファイル `build-linux-wayland/suika3` が作成されます。

gdb で Suika3 をデバッグしたい場合は、`linux-wayland` の代わりに `linux-wayland-debug` プリセットを使用できます。

---

## macOS (App Bundle)

この方法は macOS 用のアプリバンドルを作成するもので、公式バイナリでも使用されています。

### 前提条件

* Apple Silicon または Intel プロセッサを搭載した Mac
* `macOS 11` 以降がインストールされていること
* `Xcode` がインストールされていること

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset macos
cmake --build --preset macos
```

ターゲット `build-macos/Suika3.app` が作成されます。

### DMG パッケージング

macOS 用に Suika3 を配布したい場合は、コード署名と公証（notarization）を行った DMG ファイルを作成する必要があります。
キーチェーンに `Developer ID Application` 証明書があり、Xcode で Apple Developer アカウントにログインしている必要があります。
zip ファイルで配布されたアプリバンドルは、アプリバンドル外にアクセスできないため、ここでは DMG を使用します。

```
cd macos

# アプリに署名します。
codesign --timestamp --options runtime --entitlements ../resources/macos/macos.entitlements --deep --force --sign "Developer ID Application" Suika3.app

# アプリを公証します。（時間がかかります）
ditto -c -k --sequesterRsrc --keepParent Suika3.app Suika3.zip
xcrun notarytool submit Suika3.zip --apple-id "$APPLE_ID" --team-id "$TEAM_ID" --password "$APP_SECRET" --wait
xcrun stapler staple Suika3.app

# DMG ファイルを作成します。
mkdir tmp
cp -Rv Suika3.app tmp/Suika3.app
hdiutil create -fs HFS+ -format UDBZ -srcfolder tmp -volname Suika3 Suika3.dmg

# 外のファイルへのアクセスを許可するため（Gatekeeper 回避）、DMG ファイルに署名します。
codesign --sign "Developer ID Application" Suika3.dmg
```

---

## macOS (CLI)

この方法は macOS 用の CLI（コマンドラインインターフェース）版 Suika3 を作成します。
開発やデバッグには便利ですが、アプリバンドルではないため、配布には推奨されません。

### 前提条件

* Apple Silicon または Intel プロセッサを搭載した Mac
* `macOS 11` 以降がインストールされていること
* `Xcode` がインストールされていること

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset macos-cli
cmake --build --preset macos-cli
```

ターゲット `build-macos/Suika3.app` が作成されます。

lldb で Suika3 をデバッグしたい場合は、`macos` の代わりに `macos-cli-debug` プリセットを使用できます。

---

## iOS

### 前提条件

* Apple Silicon または Intel プロセッサを搭載した Mac
* `macOS 11` 以降がインストールされていること
* `Xcode` がインストールされていること

### 手順

* アセットを `assets.arc` ファイルにパックします。
* [公式バイナリ](https://github.com/suika3-community/suika/releases)をダウンロードして展開します。
* `assets.arc` ファイルを `misc/ios/resources` フォルダにコピーします。
* Xcode で `misc/ios` フォルダを開きます。
* ビルドして実行します。

### スクラッチからのビルド

スクラッチからビルドしたい場合は、`cmake --preset ios-device` または `cmake --preset ios-simulator` を使用します。
その後、ビルドされた `libsuika3.a` ファイルを `misc/ios/lib` フォルダにコピーし、Xcode で `misc/ios` フォルダを開きます。

---

## Android

### 前提条件

* `Android Studio`

### 手順

* [公式バイナリ](https://github.com/suika3-community/suika/releases)をダウンロードして展開します。
* アセットファイルを `misc/android/app/src/main/assets` フォルダにコピーします。
* Android Studio で `misc/android` フォルダを開きます。

### スクラッチからのビルド

スクラッチからビルドしたい場合は、`cmake --preset android-arm64` を使用します。
その後、ビルドされた `libsuika3.so` ファイルを `misc/android/app/src/main/jniLibs/arm64-v8a` フォルダにコピーし、Android Studio で `misc/android` フォルダを開きます。

---

## WebAssembly

### 前提条件

* `emsdk` がインストールされていること（どの OS でも可）。

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset wasm
cmake --build --preset wasm
```

ターゲットファイル `wasm/index.html` が作成されます。

### テスト

実行するには、まず `wasm` フォルダに `assets.arc` ファイルを配置します。

その後、`python -m http.server` と入力し、ブラウザで `http://localhost:8000` を開きます。

Windows では、Python の代わりに `suika3-web.exe` を使用できます。これは小さな Web サーバーを実行し、自動的にブラウザを開きます。

---

## FreeBSD

### 前提条件

* `FreeBSD` マシン
* `cmake` および `ninja` がインストールされていること。

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset freebsd
cmake --build --preset freebsd
```

ターゲットファイル `build-freebsd/suika3` が作成されます。

---

## NetBSD

### 前提条件

* `NetBSD` マシン
* `cmake` および `ninja` がインストールされていること。

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset netbsd
cmake --build --preset netbsd
```

ターゲットファイル `build-netbsd/suika3` が作成されます。

---

## OpenBSD

### 前提条件

* `OpenBSD` マシン
* `cmake` および `ninja` がインストールされていること。

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset openbsd
cmake --build --preset openbsd
```

ターゲットファイル `build-openbsd/suika3` が作成されます。

---

## Unity プラグイン

### 前提条件

* フルビルドされた `LLVM-22` がインストールされていること。

### 手順

ターミナルを開き、以下のように入力します。

```
git clone https://github.com/suika3-community/suika3.git
cd suika3
cmake --preset unity-win64
cmake --build --preset unity-win64
```

ターゲットファイル `build-unity-win64/libsuika3.dll` が作成されます。

注: `win64` は `switch`、`ps5`、`xbox` のいずれかに置き換えてください。

ベンダー公式の SDK を使用する場合は、`cmake/toolchains/unity-*.cmake` を開き、`CMAKE_C_COMPILER`、`CMAKE_CXX_COMPILER`、および `CMAKE_AR` を書き換えてください。
（秘密保持契約（NDA）のため、正確なコマンド名は記載できません。）

