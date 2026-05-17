<div align="center">
  <img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/logo-mid.png" alt="Suika3 Logo" width="480" hspace="20">
  <h1 align="center" style="border-bottom: none">
    <a href="https://suika3.vn">Suika3</a>:
    モダン/超高移植性/フルスタックな<br>
    2Dゲームエンジン(ビジュアルノベルDSL搭載)
  </h1>
  <p>
    Suika3 は「第三世代標準汎用動的視覚表現実行基盤」<br>
    (Standard Universal Infrastructure for Kinetic Arts Gen3)
    の略です。<br>
  </p>
generation
  <p>
    Suika3は、モバイルデバイスへの<br>
    深い愛情から生まれました——<br>
    主要な3Dエンジンが置き去りにしてきたプラットフォームです。<br>
    <br>
    ビジュアルノベルやあらゆる種類の2Dゲームに対応した、<br>
    強力かつ本番環境対応のゲームエンジンで、<br>
    あらゆるプラットフォームで美しく動作するよう設計されています。
  </p>
  <a href="https://discord.gg/YZsq9u9Mgr"><img src="https://img.shields.io/badge/suika3.vn-Discord-orange"></a>
  <img src="https://img.shields.io/badge/License-ZLib-orange.svg">
  <img src="https://img.shields.io/github/stars/awemorris/suika3.svg?style=flat&logo=github&colorB=orange&label=stars">
  <img src="https://img.shields.io/github/forks/awemorris/suika3.svg">
</div>

---

<div align="center">
  <p>
    独自のJIT/AOTスクリプト基盤により、<br>
    まったく同じゲームをSteamとApp Storeの両方に<br>
    同時リリースすることができ、<br>
    従来の移植作業の手間を一切省けます！
  </p>
  <img src="https://img.shields.io/badge/Desktop-Windows%20%2F%20macOS%20%2F%20Linux%20%2F%20Chromebook%20%2F%20Raspberry%20Pi-blue"><br>
  <img src="https://img.shields.io/badge/Mobile-iOS%20%2F%20Android%20%2F%20HarmonyOS%20NEXT-blue"><br>
  <img src="https://img.shields.io/badge/Console-Switch%20%2F%20PS5%20%2F%20Xbox-blue"><br>
  <img src="https://img.shields.io/badge/Web-Emscripten-blue"><br>
  <img src="https://img.shields.io/badge/Exotic-FreeBSD%20%2F%20NetBSD%20%2F%20OpenBSD%20%2F%20Solaris%20%2F%20Haiku-blue"><br>
  <img src="https://img.shields.io/badge/Store-App%20Store%20%2F%20Play%20Store%20%2F%20Microsoft%20Store%20%2F%20Steam%20%2F%20itch%2eio%20%2F%20App%20Gallery-green"><br>
  <br>
</div>

---

<!-- SCREENSHOT -->
<div align="center">
  <a href="https://noctvm.io/test/">
    ブラウザでプレイ！<br>
    <img src="https://github.com/awemorris/suika3/blob/main/docs/img/screenshot-20260406.webp" alt="Suika3 サンプルゲームのスクリーンショット"><br>
  </a>
  Suika3 自身で書かれた、見やすいランチャーが登場！<br>
  <img src="https://github.com/awemorris/suika3/blob/main/docs/img/screenshot-20260429.webp" alt="Suika3 ランチャーのスクリーンショット"><br>
</div>

---

## 概要 (TL;DR)

- **対象**: モバイルファーストでありながら、どこでも動作
- **高性能**: Cで書かれたネイティブ実装
- **ハイブリッドスクリプティング**: App Store対応のためのAOTフォールバック付きJIT VM
- **広範なプラットフォーム対応:**
    - **デスクトップ:** Windows, macOS, Linux, Chromebook, Raspberry Pi
    - **モバイル:** iOS, Android, HarmonyOS NEXT
    - **Web:** WebAssembly
    - **コンソール:** Xbox (GDK)、Unity連携によるPS5/Switch/Xbox \[要DevKit]
    - **その他:** FreeBSD, NetBSD, OpenBSD, Solaris, Haiku
- **ユーザー:** プロのクリエイター、インディースタジオ、企業

---

## インストール

### フルSDKのダウンロード

[Suika3-SDK-Full.zip をダウンロード](https://github.com/awemorris/suika3/releases/latest/download/Suika3-SDK-Full.zip)

[はじめかた](docs/mkdocs-en/docs/getting-started.md) を参照してください。

### パッケージマネージャー

Linux (Flathub):
```
flatpak install --user flathub vn.suika3.engine
flatpak run vn.suika3.engine
```

macOS (Homebrew):
```
brew tap awemorris/suika3
brew install suika3
```

FreeBSD (Ports):
```
pkg install suika3
```

### ソースからビルド

プラットフォーム別の手順については [build.md](docs/mkdocs-en/docs/build.md) を参照してください。

```
# リポジトリをクローン
git clone https://github.com/awemorris/suika3.git
cd suika3

# ビルドディレクトリを作成
mkdir build && cd build

# ビルドしてインストール
cmake .. && cmake --build . --parallel && sudo cmake --install .

# サンプルを実行
cd ../game
suika3
```

---

## Visual Studio Codeでの構文ハイライト

`@lalalll-lalalll` 氏による素晴らしいVS Code拡張機能があり、`NovelML`、`Ray`、`GUI`、`Anime` ファイルの構文ハイライトに対応しています。

詳細はこちら: [NovelML-Helper](https://github.com/lalalll-lalalll/NovelML-Helper)

この拡張機能をインストールするには、リポジトリにアクセスし、[Releasesページ](https://github.com/lalalll-lalalll/NovelML-Highlighter/releases) から .vsix ファイルをダウンロードして、VS Codeにインストールしてください。

<img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/syntax-highlighter-3.png" alt="ハイライターのインストール 1" width="320" hspace="20">

---

## VS Code 連携

### Copilot による補完

SDK にはプロジェクト内にフルドキュメントが含まれており、Copilotがそれをもとにしてコードを Copilot Chat や Completion で補完できるため、`NovelML` と `Ray` を Copilot で書くことができます。

### デスクトップでの実行

展開したフォルダを `Visual Studio Code` で開きます。

- コマンドパレットをクリックし、`Run Task --> Suika3: Run` を選択（または `Ctrl-Shift-B` を押す）
- Suika3 が即座に起動します！

<img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/vscode-2.webp" alt="VSCode 2" width="640" hspace="20">

- スクリプトのエラーは `PROBLEMS` タブに表示されます。
- エラーをクリックすると、ソースコードの該当行に直接ジャンプできます。

<img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/vscode-3.webp" alt="VSCode 3" width="640" hspace="20">

### iOS および Android での実行

1 分で iOS / Android 開発者になれる夢のような話があるとしたら……？
Suika3 ならそれが可能です。

- ワンクリック:
    - `Suika3: Build iOS IPA` または `Suika3: Build Android APK` タスクを選択すると、Suika3は自動的にアプリをビルドし、USBで接続されたスマートフォンにインストールします。
- Androidの場合:
    - スマートフォンで `開発者モード` と `USBデバッグ` をオンにする
    - Suika3側での手動セットアップは不要
    - Android Studioも不要
    - 有償ライセンスも不要
    - JDKとAndroid SDKはビルドプロセス中に自動的にダウンロードされます
- iOSの場合:
    - Xcode 15以降が必要（無料）
    - Xcode で iPhone を認識させておく必要がある
    - iPhoneシミュレーターでのテストに有償ライセンスは不要
    - 実機でのテストには `Apple Developer Program` が必要（年間99ドル）

Suika3は、iPhone / iPad / Android アプリを作成してストアにリリースするための、世界で最もスムーズなツールの一つだと確信しています！

まだリリースの準備ができていなくても、あなたのゲームが iPhone でスムーズに動いているのを友達に見せたときの顔を想像してみてください。その日の主役はあなたです。

パブリッシャーを探しているなら、プレイ可能で高品質なモバイルデモは最強の武器です。どんなピッチデッキよりも最強のPRです！

---

## はじめに

Suika3は、商業モバイルアプリ開発を目的とする、ビジュアルノベルを主軸に据えた2Dゲームエンジンです。
Awe Morris が率いるSuika3プロジェクトによって開発されています。

Suika3 で作られたゲームは、`NovelML` と `Ray` スクリプティングプラットフォームによって動作します。

- **NovelML**: タグベースで人間が読みやすい、拡張可能なDSLで、ビジュアルノベル専用に設計されています。シームレスな会話とシーン制御のための簡潔な宣言型タグが特徴です。開発者は `Ray` を使ってカスタムタグを追加できます。

- **Ray**: Suika3 の堅牢なスクリプト言語。デスクトッププラットフォームでは高速なJITコンパイラ（Sun-Ray JIT VM; コードネーム「諏訪武叡」）を使用し、本番環境やモバイル環境向けにネイティブバイナリにコンパイルすることもできます（Lunar-Ray AOTコンパイラ; コードネーム「少彦智叡」）。インタープリター（Ubi-Ray Interpreter; コードネーム「天満梅貴」）も備えています。Rayというネイティブ技術により、VNだけでなく汎用的な2Dゲームも制作できます。(Rayコードネーム: 「神威/和光全球之天媛」)

Suika3はプロ向けツールとして設計されていますが、楽しみのために使っていただくことも大歓迎です——楽しさこそが、すべての素晴らしいコミュニティの原動力です。

> [!TIP]
> Suika3は25年以上の独自R&Dから生まれた野心的なエンジンであり、現在もアーリーアダプターの皆さんとともにその未来を形作っています。

> [!TIP]
> コードネームはそれぞれ、著者がさまざまな神社を訪れる中で不思議な体験をしていき、尊敬するようになった大御神および諸大神の御名です。
> 諏訪武叡＝建御名方神（国譲りの武神）、少彦智叡＝少彦名命（国造りの智神）、天満梅貴＝菅原道真公（梅を愛した学問の神）、和光全球之天媛＝天照大御神（世を照らす調和の神）

---

## 主な特徴

Suika3は最新の技術で設計されており、以下を実現します:

- **高性能**: Sun-Ray JITを搭載し、合成ベンチマークでインタープリタ比2.5〜4.5倍の実行速度を誇ります。

- **軽量**: 開発途上国のRaspberry Piなど低スペックなハードウェアでも60fpsでスムーズに動作するよう設計されています。

- **真の移植性**: 「どこでも移植できる」戦略で設計されており、ほぼすべての現代的なプラットフォームとの互換性を確保しています。

- **拡張性**: NovelMLはRay言語を使ってシームレスに拡張できます。開発者は `Tag_*()` という名前のRay関数を書くだけでカスタムタグを実装できます。

- **洗練されたアーキテクチャ**: `Story-View-Logicアーキテクチャ` によってゲーム開発が効率化されます。`NovelML` がゲームのタイムラインとフローを管理し、`GUI` と `Anime` がUI/UXを担当し、`Ray` が複雑な実装詳細をカプセル化します。

- **信頼性**: 構造化された `品質保証` プロセスに従い、信頼性の高いソフトウェアを提供します。

- **モバイルUI/UX:** デフォルトのUXはハンバーガーメニューを備えたモバイルファーストです。モバイルのユーザビリティとストア審査リスクの観点から、メッセージウィンドウ上の小さなボタンは主要ターゲットではありません。

- **バッテリーフレンドリー:** Core Ultra 5 228VでCPU使用率わずか1%、Apple M5で8%、Apple A15 Bionicで20%という60fps動作は、外出先での長時間プレイに最適です！スマートフォンやノートPCの過熱を防ぎます——かなり「クール」ですよね？

- **長期サポート:** 高い移植性を持つコードベースで構築されており、2030年代、2040年代、そしてそれ以降の将来のプラットフォームもサポートするよう設計されています。LTSバージョンには最低10年間のバグ修正サポートが含まれます。

---

## 目次

- [カンバン（ステータス）](#カンバンステータス)
- [はじめかたガイド](#はじめかたガイド)
- [ローカルでのビルド](#ローカルでのビルド)
- [クイックルック](#クイックルック)
- [パッケージ](#パッケージ)
- [サンプル](#サンプル)
- [なぜRayなのか？](#なぜrayなのか)
- [技術概要](#技術概要)
- [ガベージコレクション](#ガベージコレクション)
- [互換性リスト](#互換性リスト)
- [ドキュメント](#ドキュメント)
- [国際化](#国際化)
- [サードパーティライブラリ](#サードパーティライブラリ)
- [CMakeプリセット](#cmakeプリセット)
- [コードベースと成熟度](#コードベースと成熟度)
- [品質保証](#品質保証)
- [採用状況](#採用状況)
- [リポジトリ構成](#リポジトリ構成)
- [アセットファイルフォーマット](#アセットファイルフォーマット)
- [ゲームのパッケージングと配布](#ゲームのパッケージングと配布)
- [エンジン機能一覧](#エンジン機能一覧)
- [スクリーンショット](#スクリーンショット)
- [バージョニングポリシー](#バージョニングポリシー)
- [ライセンス](#ライセンス)
- [サポートと連絡先](#サポートと連絡先)
- [コミュニティ](#コミュニティ)
- [コントリビューション](#コントリビューション)
- [系譜: 壮大な旅路](#系譜-壮大な旅路)
- [なぜSuika3なのか？: 私たちの理念](#なぜsuika3なのか-私たちの理念)
- [FAQ](#faq)

---

## カンバン（ステータス）

**現在のバージョンは `26.05.x` です:**

- 品質は日々向上しており、`26.05.x` は現在安定版と見なされています。
- 来たる `Suika3 26.07 LTS` リリース（2026年7月1日予定）に向けて、現在 `品質安定化期間`（2026年4月1日〜6月30日）中です。
- 軽微なバグが残っている可能性がありますが、LTSリリースまでに完全に解決されます。
- 全問題の詳細リストは [BUGS.md](BUGS.md) を参照してください。
- 詳細は [品質保証](#品質保証) を参照してください。

**現在のステータス:**

- ✅ Suika2コードベースの整理 (OpenNovel) (2024年6月〜11月)
- ✅ スクリプト言語の実装 (NoctLang) (2024年12月〜2025年3月)
- ✅ 2Dゲームエンジンの実装 (Playfield Engine) (2025年3月〜10月)
- ✅ タグ実行エンジンの実装 (Suika3) (2026年1月〜2月)
- ✅ すべてのC関数を安定した「C API」にリファクタリング (2026年1月〜2月)
- ✅ すべてのタグをCで実装 (2026年1月〜2月)
- ✅ 「C API」をRayでラップ (2026年2月〜3月)
- ✅ APIフリーズ (2026年3月7日)
- ✅ GUIアニメーション実装 (2026年3月10日)
- ✅ コードフリーズ (2026年3月12日)
- ✅ マイルストーン: `Suika3 26.04 (=LTS RC1)` (2026年4月1日)
- ✅ マイルストーン: `Suika3 26.05 (=LTS RC2)` (2026年5月1日)
- マイルストーン: `Suika3 26.06 (=LTS RC3/GM)` (2026年6月1日)
- 目標: `Suika3 26.07 LTS` 公開リリース (2026年7月1日)

**残りのタスク:**
- ドキュメント: `SRS: システム要件仕様書`
- ドキュメント: `SDS: システム設計仕様書`

---

## はじめかたガイド

[はじめかたガイド](docs/mkdocs-en/getting-started.md) も参照してください。

このガイドでは、最初のビジュアルノベルプロジェクトをいくつかの簡単なステップで開始する方法を説明します。

### インストール

エンジンを動かして、その魔法を体験しましょう！

**Windows:**
- **ダウンロードと展開**
    - [Suika3-full.zip](https://github.com/awemorris/suika3/releases/latest/download/Suika3-full.zip) をダウンロードし、任意のフォルダに展開します。
- **起動**
    - フォルダを開き、`suika3.exe` を実行してサンプルゲームを起動します！

**macOS:**
- **ダウンロードと展開**
    - Suika3-SDK-Full.zip をダウンロードし、任意のフォルダに展開します。
- **ディスクイメージのマウント**
    - `SDK/macos/` に移動し、`Suika3.dmg` を開きます。
- **アプリバンドルの設定**
    - DMGから `Suika3` アプリを `suika3.exe`（とデータフォルダ）がある同じフォルダにコピーします。
    - 注意: アプリバンドルはゲームデータと同じ場所に置く必要があります。
- **起動**
    - `Suika3` アプリをダブルクリックしてサンプルゲームを起動します！

**Linux:**
- **ダウンロードと展開**
    - Suika3-full.zip をダウンロードし、任意のディレクトリに展開します。
- **Flatpakパッケージのインストール**
    - `SDK/linux/` に移動し、`Suika3.flatpak` を開きます（または `flatpak install --user Suika3.flatpak` を実行します）。
    - これにより `.novel` および `.ray` ファイルがSuika3エンジンに関連付けられます。
- **起動**
    - 展開したフォルダを開き、`start.novel` をダブルクリックしてサンプルゲームを起動します！

### ストーリーをカスタマイズする (`start.novel`)

ゲームに自分の言葉を語らせましょう。

- **開く:**
    - プロジェクトフォルダ内の `start.novel` ファイルを、お好みのテキストエディタで開きます。
- **編集:**
    - ファイルの先頭に以下のタグを追加します:
    ```
    [text text="こんにちは、世界！これが私の最初のゲームです。"]
    ```
- **テスト:**
    - ファイルを保存してSuika3を再実行します。
    - 画面に新しいメッセージが表示されるはずです！

- **編集:** 既存のテキストを以下のコマンドで置き換えます:
```
[text text="こんにちは、世界！これが私の最初のゲームです。"]
```
- **テスト:** ファイルを保存して `suika3.exe` を再実行します。画面に新しいメッセージが表示されるはずです！

### 内部の仕組み（上級者向けTips）

ゲームの画面サイズを簡単に変更できます。

- **場所の確認:** エディタで `main.ray` ファイルを開きます。
- **変更:** `func setup()` セクションを探します。ここで解像度とウィンドウのタイトルを変更できます:
```
// ウィンドウが開かれたときに呼ばれます。
func setup() {
    return {
        width:      1280,            // ゲームの幅
        height:     720,             // ゲームの高さ
        title:      "私の最初のゲーム", // ゲームのタイトル
        fullscreen: false            // フルスクリーンモードはtrueに設定
    };
}
```

`main.ray` ファイルの下部には `Ray` でのコアゲームロジックが含まれています。高度なカスタマイズをする場合を除き、これらの関数はそのままにしておくのが最善です:

- `func start()`: ゲーム起動時に一度だけ呼ばれます。
- `func update()`: ゲームロジックの処理のために毎フレーム実行されます。
- `func render()`: 更新後に画面上のすべてを描画します。

```
// ゲーム開始前に呼ばれます。
func start() {
    // ここでプラグインをロードします。
    // Suika.loadPlugin("testplugin");

    // 以下の行は削除しないでください。
    Suika.start();
}

// フレームレンダリング前に呼ばれます。
func update() {
    // 以下の行は削除しないでください。
    Suika.update();
}

// 毎フレームのレンダリング時に呼ばれます。
func render() {
    // 以下の行は削除しないでください。
    Suika.render();
}
```

> [!TIP]
> これらの関数はSuika3を動かす `Playfield Engine` のコアメカニズムです。Suika.start()、Suika.update()、Suika.render() は、ゲームが正常に機能するために必ず残しておく必要があります。

---

## クイックルック

### NovelML

`start.novel` ファイルはこのように見えます:

```
# 背景画像を表示する。
[bg file="bg/coast.png" time="0.5"]

# キャラクター画像を表示する。
[ch center="ch/midori-normal.png" time="0.5" fade="normal"]

# 質問テキストを表示する。
[text text="名前を選んでください。"]

# 選択肢を表示する。
[choose
    name="choose_result"
    text1="三月"
    text2="四月"
    text3="五月"
]

# 選択結果で分岐する。
[if lhs="${choose_result}" op="==" rhs="三月"]
    [set name="name" value="三月"]
[elseif lhs="${choose_result}" op="==" rhs="四月"]
    [set name="name" value="四月"]
[else]
    [set name="name" value="五月"]
[endif]

# テキストを表示する。
[text name="${name}" text="こんにちは！私の名前は${name}です。"]
```

---

## サンプル

リリースzipには [サンプルゲーム](game) が含まれています。

`game/` フォルダで以下を確認できます:
- デモプロジェクト
- サンプルアセット
- NovelMLの使用例
- Rayの使用例

---

### なぜRayなのか？

- **即時反映:** コンパイルサイクルも待ち時間もありません。組み込みのJITコンパイラは編集直後にスクリプトを実行するため、ゲームプレイを調整してリアルタイムで結果を確認できます。

- **真のネイティブパフォーマンス:** iOS、Android、コンソールで拡張機能をフルスピードで実行できます。JITが制限されているプラットフォームでも、AOTのおかげでパフォーマンスを犠牲にする必要はありません。Rayはネイティブ技術です。

- **サンドボックスモデル:** スクリプトは任意のファイルやネットワークにアクセスできません。ゲームアセットとセーブデータのみ、エンジンが管理するAPIを通じてアクセス可能です。

- **スムーズなストア承認:** リジェクトのリスクを大幅に低減します。最終ビルドはAOTによるネイティブコードで構成されるため、App Storeやコンソールの審査をスムーズに通過できます。

- **学びやすく、使いこなせる:** JavaScriptにインスパイアされた構文は初心者にも優しく、複雑なシステムに経験豊富な開発者が求める深い柔軟性も備えています。

- **長期的な安定性:** 言語を自社で開発しているため、上流プロジェクトの破壊的変更の影響を受けません。フルスタックを所有・管理しているため、スクリプトは永久に互換性を保ちます。

要するに、JIT、AOT、インタープリターを単一プロジェクトに統合した実用的な無料言語が見つからなかったため、私たち自身で作りました。

---

## 技術概要

これらの仕様にワクワクするなら、あなたは正しい場所にいます。

Suika3は単なるSDLラッパーにスクリプトを追加したものではありません。独自のレンダリングおよびオーディオバックエンドと、独自のスクリプト言語を備えており、完全に独立したゲームエンジンとして位置づけられています。

### コアアーキテクチャ

Suika3は包括的な2Dゲームエンジン `PlayField Engine` を基盤としています。つまり、Suika3はPlayfield APIを使って完全に拡張可能です。

Playfield EngineはもともとSuika3専用に開発されました。「Playfield」という名前は、AweがSIE PlayStationファームウェアチームに在籍していた頃、PlayStationにインスパイアされて名付けられました。

```
+------------------------------+
|        NovelML (Tags)        | --> 他のゲームジャンルにも展開予定
+------------------------------+
|     Plugin Tags (by Ray)     | --> Rayはネイティブバイナリにコンパイル可能！
+------------------------------+
|       Base Tags (by C)       |
+------------------------------+
|       Ray Wrapper API        |
+------------------------------+
|         Suika3 C API         |
+------------------------------+
|       Playfield C API        |
+---------------+--------------+
|   StratoHAL   |   NoctLang   |
+---------------+--------------+
|       Operating System       |
+------------------------------+
```

- **スクリプティング**: ゲームスクリプティング用に設計された小さくも強力な言語、[NoctLang](https://github.com/awemorris/NoctLang) を統合しています。

* **レンダリング**: ネイティブのDirectX 9/11/12、Metal、OpenGL、OpenGL ES、WebGLをサポートし、幅広い互換性を実現しています。

* **オーディオ**: ネイティブのDirectSoundまたはXAudio2（Windows）、Audio Unit（macOS/iOS）、ALSA（Linux）、OSS（BSD）などのAPIによる軽量オーディオサポートを提供します。

### StratoHAL

StratoHALは2001年から開発された2Dゲームエンジンのコードベースに起源を持ち、Zlibライセンスのもと卓越した安定性の実績を誇ります。

Windows 9x時代から四半世紀以上にわたって進化し、macOS、Linux、iOS、Android、WebAssembly、Unityのサポートにまで成長しました。10年以上にわたってスマートフォン上で安定して動作しています。

SDL3がすでに人気のあるフリー/リブレソフトウェアの代替として存在しますが、StratoHALは以下をカバーしています:

- SDL3と同じ主要プラットフォーム。

- ソースツリー上のNDA制限コードに依存せず、Unity連携によるコンソールサポートを独自に提供。

- 古いプラットフォーム向けのソフトウェア3Dレンダリング。

### プラットフォームサポートとコンポーネント

|プラットフォームタイプ  |OS / プラットフォーム |グラフィックス           |サウンド               |SDK                  |
|------------------------|----------------------|-------------------------|-----------------------|---------------------|
|デスクトップ            |Windows               |DirectX 12/11/9          |DirectSound            |Win32 API            |
|                        |macOS                 |Metal                    |Audio Unit             |AppKit (Objective-C) |
|                        |ChromeOS              |WebGL 2                  |OpenAL (Emscripten)    |Emscripten (C)       |
|                        |Linux                 |OpenGL 3                 |ALSA                   |C, X11 or Wayland    |
|                        |*BSD                  |OpenGL 3                 |/dev/dsp \| /dev/audio |C, X11               |
|                        |Solaris 11            |Software                 |/dev/dsp               |C, X11               |
|                        |Solaris 10            |Software                 |/dev/audio             |C, X11               |
|                        |Qt                    |OpenGL 3                 |Qt Sound               |Qt                   |
|モバイル                |iOS                   |Metal                    |Audio Unit             |UIKit (Objective-C)  |
|                        |Android               |OpenGL ES 2              |OpenSL ES              |Android NDK          |
|                        |HarmonyOS NEXT        |OpenGL ES 2              |OpenSL ES              |OpenHarmony SDK      |
|Web                     |WebAssembly (Wasm)    |WebGL 2                  |OpenAL (Emscripten)    |Emscripten (C)       |
|コンソール              |Unity                 |Unity                    |Unity                  |Unity Native Plugin  |
|                        |Xbox Series X\|S      |DirectX 12               |XAudio2                |Microsoft GDK        |

### コンソールUnityプラグインに関する注意事項

Playfield EngineはWindows 64ビットやゲームコンソールを含むプラットフォーム向けにUnityプラグインバイナリを提供しています。これらのバイナリは公式バージョンのLLVM/Clangツールチェーンのみで構築されています（プロプライエタリなSDKは使用していません）。

Xboxシリーズについては、Unity経由ではなくMicrosoft GDKネイティブポートを直接使用できます。

### NoctLang

```
Ray = NoctLang + Suika3 API + Playfield API
```

**NoctLang** はアプリ内スクリプティング向けの軽量スクリプト言語です。ゲーム指向の構文を持ち、明瞭さ、即時起動、エンジンとの密な統合を重視しています。

組み込みのJITコンパイラはほとんどのゲームコンソールとスマートフォンをカバーする幅広いCPUアーキテクチャをサポートしています:

- ✅ Intel x86 (Xbox) [jit-x86.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-x86.c)
- ✅ AMD64/x86_64 (PS4/PS5/Xbox One/Xbox series X|S) [jit-x86_64.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-x86_64.c)
- ✅ ARMv5-ARMv7 (Nintendo DS/PS Vita) [jit-arm32.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-arm32.c)
- ✅ Arm64 (Switch/Switch2) [jit-arm64.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-arm64.c)
- ✅ PowerPC 32 (Wii/GameCube) [jit-ppc32.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-ppc32.c)
- ✅ PowerPC 64 (PS3/Xbox 360) [jit-ppc64.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-ppc64.c)
- ✅ MIPS32 (PS1/PSP) [jit-mips32.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-mips32.c)
- ✅ MIPS64 (N64/PS2) [jit-mips64.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-mips64.c)
- ✅ RISC-V 32 (将来のデバイス向け) [jit-riscv32.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-riscv32.c)
- ✅ RISC-V 64 (将来のデバイス向け) [jit-riscv64.c](https://github.com/awemorris/suika3/blob/main/external/PlayfieldEngine/external/NoctLang/src/core/jit-riscv64.c)

これらのアーキテクチャは十分にサポートされており、少なくとも [テストスイート](external/PlayfieldEngine/external/NoctLang/tests/run-syntax.sh) をすべてパスすることができます。

ただし、開発用マシンが不足しているため、以下はまだサポートされていません（インタープリターのみ）:

- ❌ SH-4 (Dreamcast) (インタープリターで動作)
- ❌ Sun SPARC (インタープリターで動作)
- ❌ HP PA-RISC (インタープリターで動作)
- ❌ Motorola 68000 (インタープリターで動作)
- ❌ Loongson (インタープリターで動作)
- ❌ IBM Z (インタープリターで動作)
- **チャレンジ:** これらのマシンをお持ちの方は、3日間の開発環境へのsshアクセスをご提供ください。移植できます ;-)

### AOTコンパイル

JITが制限されているプラットフォーム（モバイルやコンソールなど）では、NoctLangは少なくともインタープリターモードにフォールバックでき、Cソースバックエンドを使ったモダンなAOT（事前コンパイル）も利用できます——厳格に管理された環境でも完全な互換性を確保します。

詳細は [AOT](docs/mkdocs-en/docs/aot.md) を参照してください。

### スクリプト実行モード

|プラットフォーム      |モード             |
|------------------|-------------------|
|Windows x86       |JIT                |
|Windows x64       |JIT                |
|Windows arm64     |JIT                |
|macOS x86_64      |JIT                |
|macOS arm64       |JIT                |
|Linux x86         |JIT                |
|Linux x86_64      |JIT                |
|Linux armv7       |JIT                |
|Linux arm64       |JIT                |
|iOS               |インタープリターまたはAOT |
|Android           |インタープリターまたはAOT |
|HarmonyOS NEXT    |インタープリターまたはAOT |
|WebAssembly       |インタープリターまたはAOT |
|Unity Plugin      |インタープリターまたはAOT |
|Xbox              |インタープリターまたはAOT |
|FreeBSD           |JIT                |
|NetBSD            |JIT                |
|OpenBSD           |インタープリターまたはAOT |
|Solaris 11 x86_64 |JIT                |
|Solaris 11 Sparc  |インタープリターまたはAOT |

### ランタイムフットプリント

Suika3のフットプリントは非常に小さいです。

| OS           | バイナリサイズ | メモリ使用量               |
|--------------|------------|--------------------------|
| Windows 11   | 2 MB       | タスクマネージャーで300 MB  |
| Windows 2000 | 3 MB       | タスクマネージャーで8 MB   |

> [!TIP]
> モダンなグラフィックスアーキテクチャのため、DirectX 12を使用するゲームアプリはゲームアセットをロードする前から少なくとも300 MBのメモリを消費します。エンジン自体はWindows 2000上でわずか8 MBしか消費しません。

### JITパイプライン

NoctLangはデュアルIRアーキテクチャを使用しており、HIRが解析を担当し、LIRがインタープリター、JIT、AOTバックエンドにわたる統一的な実行を保証します。

- **HIR（高水準中間表現）**
    - プログラム解析のための構造化制御フローグラフ（CFG）
    - 代数的簡約のための式DAG
    - 将来の高度な最適化の基盤

```
CFG for "func foo(a) { if (a > 0) { return a; } else { return -a; } }"

  +---------------+
  | 0: Func Block |         -- pred: none, succ: 1
  +---------------+
     +-------------+
     | 1: IF Block |        -- pred: 0, succ: 2 (true), 3 (false)
     +-------------+
        +----------------+
        | 2: Basic Block |  -- pred 1, succ 5
        +----------------+
     +---------------+
     | 3: Else Block |      -- pred 1, succ 4
     +---------------+
        +----------------+
        | 4: Basic Block |  -- pred 3, succ 5
        +----------------+
  +--------------+
  | 5: End Block |          -- pred 2, 4
  +--------------+
                                 (pred = predecessor, succ = successor)
     
```

- **LIR（低水準中間表現）**
    - VMバイトコード。解釈とJITコード生成入力の両方の主要フォーマット。
    - 高速かつポータブルな解釈を実現する高い抽象レベル。
    - JITでの効率的なマシンコードへの変換に十分なコンパクトさ。

```
  LIR for "a = 1 + 2"

    ICONST       %0, 1               ; 定数1をロード
    ICONST       %1, 2               ; 定数2をロード
    ADD          %2, %0, %1          ; 合計を計算
    STORESYMBOL  "a", %2             ; グローバル変数"a"に結果を格納
```

コンパイルステージは以下の通りです。

```
 +-----+     +-----+     +-----+     +-----+
 | SRC | --> | AST | --> | HIR | --> | LIR | -----> [インタープリタバックエンド]
 +-----+     +-----+     +-----+     +-----+   |
                                               +--> [JITバックエンド]
                                               |
                                               +--> [Cバックエンド]
```

HIRとLIRの分離により以下が可能になります:

- **軽量なJITパイプライン**: 解析からコード生成までのオーバーヘッドを最小化。
- **アーキテクチャの明確さ**: 各ステージが明確に定義された役割を持ち、保守が容易。
- **移植性**: 同じLIRを直接解釈することも、最適化されたマシンコードに変換することも可能。

上述の通り、HIRが構造を表現し、LIRが実行を表現します。この分離により、NoctLangは最適化の機会を犠牲にすることなくJITパイプラインを軽量に保てます。

すべてのJITバックエンドが同じLIRから変換を行うため、アーキテクチャ間の移植性は自然に確保されます。この統一されたアプローチが、NoctLangを移植性が高く保守性に優れたものにしています。

### 統一実行モデル

NoctLangはJIT、AOT、インタープリターの各実行モデル全体で厳格なバイトコードセマンティクスを維持します。
各NoctLangバイトコード命令は対応するランタイムC関数に直接マッピングされます。
メソッドレベルのベースラインJITであるNoctLang JITでは、ほとんどのバイトコード命令がランタイム関数呼び出しに変換されます。
同じメカニズムがAOTとインタープリター実行の両方に適用されます。
その結果、3つの実行モデルすべてが統一されたインフラを活用し、完全なセマンティクスの等価性を保証します。

この合理化された統一実行モデルはNoctLangの中核的な独自性を表しています。
我々の知る限り、NoctLangはJIT、AOT、インタープリタ実行を統一しながら、すべての実行モードで厳格なセマンティクスの一貫性を維持した最初の実用的な言語です。

---

## ガベージコレクション

`Ray` は、Java HotSpot VMのアーキテクチャにインスパイアされた高性能な世代別ガベージコレクターを備えています。この設計により、開発者は恐ろしい「GCスパイク」やフレームドロップに悩まされることなく制作に集中できます。

スクリプト内のオブジェクト（文字列、辞書）はGCによって管理され、テクスチャなどの大型アセットはネイティブコードによって管理されます。

### コアメカニズム: 世代別GC

システムはメモリ管理を最適化するためにオブジェクトを2つのグループに分類します:

* 新世代: ほとんどのオブジェクトは短命です。Suika3はこれらを高速なコピーアルゴリズム（セミスペースコピーGC）で処理し、一時的なデータを瞬時にクリアします。（通常 < 0.1ms）

* 旧世代: 長寿命のオブジェクトはここに移動されます。このエリアではマーク・スイープ・コンパクトGCアルゴリズムを使用し、フラグメンテーションを防ぐために定期的にメモリを再編成します。（通常 10〜300ms）

詳細な実装については、[gc.h](external/PlayfieldEngine/external/NoctLang/src/core/gc.h) と [gc.c](external/PlayfieldEngine/external/NoctLang/src/core/gc.c) を確認してください。

### フレーム同期レイテンシーハイディング

Rayの本当の「魔法」はそのタイミングにあります。新世代コピーGCを毎フレーム実行することで、GC処理時間を自然なフレーム間隔の中に効果的に隠します。

この世代別戦略のおかげで、より重い旧世代マーク・スイープはほとんど起動されず、プレイヤーに常に60fpsの体験を提供します。

---

## 互換性リスト

### プラットフォーム対応状況の概要

`サポート済み` は上流（`Playfield Engine`）が完全にサポートしていることを意味します。

| カテゴリ    | OS / 環境          | ステータス      | 最終確認       | 確認環境                         |
|------------|-------------------|-------------|--------------|----------------------------------|
| **デスクトップ** | Windows       | ✅ サポート済み | 2026年4月3日 | Windows 11 (x64/Arm64)           |
|            | macOS             | ✅ サポート済み | 2026年4月3日 | macOS 26 Tahoe (Apple Silicon)   |
|            | Linux             | ✅ サポート済み | 2026年4月3日 | Ubuntu 24.04 LTS (x86_64)        |
| **モバイル** | iOS              | ✅ サポート済み | 2026年4月3日 | iOS 18                           |
|            | Android           | ✅ サポート済み | 2026年4月3日 | Android 15                       |
|            | HarmonyOS NEXT    | ✅ サポート済み | -            | API 10+                          |
| **BSD**    | FreeBSD           | ✅ サポート済み | 2026年4月5日 | 15.0-RELEASE amd64               |
|            | NetBSD            | ✅ サポート済み | 2026年4月5日 | 10.0 amd64, aarch64, armv7       |
|            | OpenBSD           | ✅ サポート済み | 2026年4月5日 | 7.8 amd64, aarch64, armv7        |
| **Web**    | WebAssembly (Wasm)| ✅ サポート済み | 2026年4月3日 | Chrome, Edge, Safari             |
|            | Chromebook        | ✅ サポート済み | -            | Chrome Browser / Linux Container |
| **その他** | Unity連携         | ✅ サポート済み | -            | Unity 6.2 (Windows x86_64)       |

### 64ビットWindowsバイナリ互換性リスト

公式推奨バイナリは64ビット版です。

| OS      | バージョン                  | パッチ | CPU    | ランタイム                            | 64ビットバイナリ | Direct3D |
|---------|-----------------------------|-------|--------|---------------------------------------|---------------|----------|
| Windows | 11                          |       | x64    | (不要)                                | ✅            | 12.0     |
| Windows | 11                          |       | arm64  | (不要)                                | ✅            | 12.0     |
| Windows | 10                          |       | x64    | (不要)                                | ✅            | 12.0     |
| Windows | 10                          |       | arm64  | (不要)                                | ✅            | 12.0     |
| Windows | 8.1                         |       | x64    | (不要)                                | ✅            | 11.0     |
| Windows | 8                           |       | x64    | (不要)                                | ✅            | 11.0     |
| Windows | 7                           | SP1   | x64    | (不要)                                | ✅            | 11.0     |
| Windows | 7                           |       | x64    | UCRT Update (KB2999226)               | ✅            | 11.0     |
| Windows | Vista                       | SP2   | x64    | Platform Update                       | ✅            | 11.0     |
| Windows | Vista                       | SP1   | x64    | DirectX End-User Runtimes (June 2010) | ✅            | 9.0      |
| Windows | Vista                       |       | x64    | DirectX End-User Runtimes (June 2010) | ✅            | 9.0      |
| Windows | XP Professional x64 Edition | SP2   | x64    | DirectX End-User Runtimes (June 2010) | ✅            | 9.0      |
| Windows | XP Professional x64 Edition | SP1   | x64    | DirectX End-User Runtimes (June 2010) | ✅            | 9.0      |
| Windows | XP Professional x64 Edition |       | x64    | DirectX End-User Runtimes (June 2010) | ✅            | 9.0      |

### 32ビットWindows互換性リスト

Suika3は後方互換性のために32ビットバイナリ `suika3-32.exe` を提供しており、古いランタイムを通じてレガシーシステムをサポートします。

| OS      | バージョン                  | パッチ | CPU    | ランタイム                             | 32ビットバイナリ | Direct3D               |
|---------|-----------------------------|-------|--------|----------------------------------------|---------------|------------------------|
| Windows | 11                          |       | x64    | (不要)                                 | ✅            | 12.0                   |
| Windows | 11                          |       | arm64  | (不要)                                 | ✅            | 12.0                   |
| Windows | 10                          |       | x86    | (不要)                                 | ✅            | 12.0                   |
| Windows | 10                          |       | x64    | (不要)                                 | ✅            | 12.0                   |
| Windows | 10                          |       | arm64  | (不要)                                 | ✅            | 12.0                   |
| Windows | 8.1                         |       | x86    | (不要)                                 | ✅            | 11.0                   |
| Windows | 8.1                         |       | x64    | (不要)                                 | ✅            | 11.0                   |
| Windows | 8                           |       | x86    | (不要)                                 | ✅            | 11.0                   |
| Windows | 8                           |       | x64    | (不要)                                 | ✅            | 11.0                   |
| Windows | 7                           | SP1   | x86    | (不要)                                 | ✅            | 11.0                   |
| Windows | 7                           |       | x86    | (不要)                                 | ✅            | 11.0                   |
| Windows | 7                           | SP1   | x64    | (不要)                                 | ✅            | 11.0                   |
| Windows | 7                           |       | x64    | (不要)                                 | ✅            | 11.0                   |
| Windows | Vista                       | SP2   | x86    | DirectX 11 Platform Update             | ✅            | 11.0                   |
| Windows | Vista                       | SP1   | x86    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | Vista                       |       | x86    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | Vista                       | SP2   | x64    | DirectX 11 Platform Update             | ✅            | 11.0                   |
| Windows | Vista                       | SP1   | x64    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | Vista                       |       | x64    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | XP                          | SP3   | x86    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | XP                          | SP2   | x86    |                                        | -->           | VS2008が必要            |
| Windows | XP                          | SP1   | x86    |                                        | -->           | VS2008が必要            |
| Windows | XP                          |       | x86    | DirectX 9.0b Runtime                   | ✅            | 9.0                    |
| Windows | XP Professional x64 Edition | SP2   | x64    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | XP Professional x64 Edition | SP1   | x64    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | XP Professional x64 Edition |       | x64    | DirectX End-User Runtimes (June 2010)  | ✅            | 9.0                    |
| Windows | 2000                        | SP4   | x86    | DirectX End-User Runtimes (June 2010)  | -->           | VS2008が必要            |
| Windows | 2000                        | SP3   | x86    | DirectX End-User Runtimes (June 2010)  | -->           | VS2008が必要            |
| Windows | 2000                        | SP2   | x86    | DirectX End-User Runtimes (June 2010)  | -->           | VS2008が必要            |
| Windows | 2000                        | SP1   | x86    | DirectX End-User Runtimes (June 2010)  | -->           | VS2008が必要            |
| Windows | 2000                        |       | x86    | DirectX End-User Runtimes (June 2010)  | -->           | VS2008が必要            |
| Windows | Me                          |       | x86    |                                        | -->           | VC++ 6.0が必要          |
| Windows | 98                          |       | x86    |                                        | -->           | VC++ 6.0が必要          |
| Windows | 95                          |       | x86    |                                        | -->           | VC++ 6.0が必要          |
| Windows | NT 4.0                      |       | x86    |                                        | -->           | VC++ 6.0が必要          |
| Windows | NT 3.51                     |       | x86    |                                        | ❌            | Win32 APIが不十分       |

### macOS互換性リスト

| OS                     | バージョン | Mac CPU              | ステータス                    |
|------------------------|---------|----------------------|------------------------------|
| macOS Tahoe            | 26.0    | arm64 / x86_64       | ✅ OK                        |
| macOS Sequoia          | 15.0    | arm64 / x86_64       | ✅ OK                        |
| macOS Sonoma           | 14.0    | arm64 / x86_64       | ✅ OK                        |
| macOS Ventura          | 13.0    | arm64 / x86_64       | ✅ OK                        |
| macOS Monterey         | 12.0    | arm64 / x86_64       | ✅ OK                        |
| macOS Big Sur          | 11.0    | arm64 / x86_64       | ✅ OK                        |
| macOS Catalina         | 10.15   | x86_64               | ✅ OK                        |
| macOS Mojave           | 10.14   | x86_64               | ✅ OK                        |
| macOS High Sierra      | 10.13   | x86_64               | ✅ OK                        |
| macOS Sierra           | 10.12   | x86_64               | ✅ OK                        |
| OS X El Capitan        | 10.11   | x86_64               | ✅ OK                        |
| OS X Yosemite          | 10.10   | x86_64               | ❌ Metalなし                 |
| OS X Mavericks         | 10.9    | x86_64               | ❌ Metalなし                 |
| Mac OS X Puma          | 10.1    | ppc                  | ❌ Metal・AVFoundationなし   |
| Mac OS X Cheetah       | 10.0    | ppc                  | ❌ Metal・AVFoundationなし   |

### Linux互換性リスト

| ディストリビューション | バージョン             | CPU             | ステータス | グラフィックス                                |
|-------------------|-----------------------|-----------------|--------|----------------------------------------|
| Debian            | 13 / 12               | x86_64, arm64   | ✅     | X11 + OpenGL 3またはWayland + OpenGL ES 3 |
| Raspberry Pi OS   | Debian 13             | arm64, armv7    | ✅     | X11 + OpenGL ES                         |
| Ubuntu            | 24.04 / 22.04 / 20.04 | x86_64, arm64   | ✅     | X11 + OpenGL 3またはWayland + OpenGL ES 3 |
| Kubuntu           | 25.04                 | x86_64, arm64   | ✅     | X11 + OpenGL 3またはWayland + OpenGL ES 3 |
| Linux Mint        | 22.3                  | x86_64          | ✅     | X11 + OpenGL 3またはWayland + OpenGL ES 3 |
| Fedora            | 43                    | x86_64          | ✅     | X11 + OpenGL 3またはWayland + OpenGL ES 3 |
| Odroid各種        |                       | arm64           | ✅     | GBM + DRM + EGL + OpenGL ES             |

### iOS互換性リスト

| OS     | ステータス |
|--------|----------|
| iOS 26 | ✅       |
| iOS 18 | ✅       |
| iOS 17 | ✅       |
| iOS 16 | ✅       |
| iOS 15 | ✅       |
| iOS 14 | ✅       |
| iOS 13 | ✅       |
| iOS 12 | ✅       |
| iOS 11 | ✅       |
| iOS 10 | ✅       |
| iOS 9  | ✅       |
| iOS 8  | ✅       |
| iOS 7  | Metalなし |

### Android互換性リスト

| OS          | ステータス |
|-------------|--------|
| Android 16  | ✅     |
| Android 15  | ✅     |
| Android 14  | ✅     |
| Android 13  | ✅     |
| Android 12  | ✅     |
| Android 11  | ✅     |
| Android 10  | ✅     |
| Android 9   | ✅     |
| Android 8   | ✅     |
| Android 7   | ✅     |
| Android 6   | ✅     |
| Android 5   | ✅     |
| Android 4.4 | ✅     |

### HarmonyOS / OpenHarmony互換性リスト

| OS         | ステータス |
|------------|--------|
| API 20     | ✅     |
| API 12     | ✅     |

### FreeBSD

| OS         | ステータス   |
|------------|-------------|
| FreeBSD 15 | ✅          |
| FreeBSD 14 | ✅          |

### NetBSD

| OS         | ステータス   |
|------------|-------------|
| NetBSD 10  | ✅          |
| NetBSD 9   | ✅          |
| NetBSD 8   | ✅          |

### OpenBSD

| OS           | ステータス   |
|--------------|-------------|
| OpenBSD 7.8  | ✅          |

### Solaris

| OS         | ステータス     |
|------------|-------------|
| Solaris 11 | ✅          |
| Solaris 10 | ✅          |
| Solaris 9  | 未確認       |
| Solaris 8  | 未確認       |
| Solaris 7  | 未確認       |

---

## ドキュメント

ドキュメントはウェブサイトで公開されています:
- [English](https://suika3.vn/en/docs/)
- [Spanish](https://suika3.vn/es/docs/)
- [French](https://suika3.vn/fr/docs/)
- [German](https://suika3.vn/de/docs/)
- Italian (翻訳中)
- [Russian](https://suika3.vn/ru/docs/)
- Greek (翻訳中)
- [簡体中文](https://suika3.vn/zh-Hans/docs/)
- [繁体中文](https://suika3.vn/zh-Hant/docs/)
- [日本語](https://suika3.vn/ja/docs/)

---

## 国際化

Suika3は以下の言語をサポートしています。

| 言語                   | コード      | 翻訳タイプ              | 翻訳スコープ                    | 進捗          |
|------------------------|-------------|--------------------------|----------------------------------|---------------|
| English (フォールバック)  | `en`        | オリジナル               | UI / ドキュメント / サンプル      | 基準          |
| English (アメリカ)        | `en-us`     | なし                     | UI                               | -             |
| English (イギリス)        | `en-gb`     | なし                     | UI                               | -             |
| Spanish (フォールバック)  | `es`        | 機械翻訳                 | UI                               | 完了          |
| Spanish (スペイン)        | `es-es`     | なし                     | UI                               | -             |
| Spanish (ラテンアメリカ)  | `es-la`     | なし                     | UI                               | -             |
| French (フォールバック)   | `fr`        | 機械翻訳                 | UI                               | 完了          |
| French (フランス)         | `fr-fr`     | なし                     | UI                               | -             |
| French (カナダ)           | `fr-ca`     | なし                     | UI                               | -             |
| Italian                | `it`        | 機械翻訳                 | UI                               | -             |
| German                 | `de`        | 機械翻訳                 | UI                               | 完了          |
| Greek                  | `el`        | 機械翻訳                 | UI                               | 完了          |
| Russian                | `ru`        | 機械翻訳                 | UI                               | 完了          |
| Simplified Chinese     | `zh-cn`     | 機械翻訳                 | UI                               | 完了          |
| Traditional Chinese    | `zh-tw`     | 機械翻訳                 | UI                               | 完了          |
| Japanese               | `ja`        | ネイティブ翻訳            | UI                               | 完了          |

### 言語の切り替え方法

Suika3はシステムロケールを使用します。言語を切り替えるには、システムロケールを設定してください。

また、サンプルゲームの設定画面には言語切り替えボタンがあります。

### 翻訳への貢献

翻訳の改善へのコミュニティからの貢献を歓迎します！より多くの開発者に届けるために、以下の方法で貢献できます:

0.  `grep -r _TR() .` を実行してオリジナルメッセージを検出します。
1.  **ファイルの場所を確認**: 翻訳アセットは以下の3つのコアディレクトリで管理されています:
    - `resources/i18n/message.txt` (Suika3部分、`S3_TR()`)
    - `external/PlayfieldEngine/resources/i18n/message.txt` (Playfield Engine部分、`PF_TR()`)
    - `external/PlayfieldEngine/external/NoctLang/i18n/message.txt` (NoctLang部分、`N_TR()`)
    - `external/PlayfieldEngine/external/StratoHAL/i18n/message.txt` (StratoHAL部分、`HAL_TR()`)
2.  **変更を提出**:
    - タイポや奇妙な機械翻訳を見つけたら、**プルリクエスト**を遠慮なく開いてください。
    - 新しい言語を追加したい場合は、調整のためにまず**Issueを開いて**ください！

> [!TIP]
> 手動・プロフェッショナルによる翻訳は機械生成コンテンツより優先されます。人間のタッチは大きな違いをもたらします！

---

## サードパーティライブラリ

当エンジンはいくつかのフリー/リブレソフトウェアライブラリを基盤として構築されています。ビルドの再現性と長期的なメンテナンス性を確保するため、必要なすべてのソースコードtarballとパッチはこのリポジトリの `external/PlayfieldEngine/external/StratoHAL/lib/` ディレクトリに同梱されています。

### コアライブラリ

| ライブラリ        | 用途                | 主な特徴                                                    |
|------------------|---------------------|-------------------------------------------------------------|
| Playfield Engine | 2Dゲームエンジン    | 基盤ゲームエンジン（自社開発）                               |
| NoctLang         | スクリプト言語      | スクリプト言語（自社開発）                                   |
| zlib             | 圧縮                | 汎用データ圧縮のためのDeflateアルゴリズム                    |
| libpng           | 画像                | PNGイメージサポートのリファレンスライブラリ                  |
| JPEG9            | 画像                | 業界標準のJPEG画像展開                                       |
| libwebp          | 画像                | モダンな高効率画像フォーマットサポート                       |
| FreeType2        | フォント            | 高品質なフォントレンダリングとグリフ処理                     |
| libogg           | オーディオコンテナ  | Oggマルチメディアファイルのビットストリーム処理              |
| libvorbis        | オーディオコーデック | BGMとSEに使用される非可逆音声圧縮                           |
| brotli           | 圧縮                | WebとデータアセットのためのWOFF高圧縮率圧縮                  |
| bzip2            | 圧縮                | アーカイブ向けの高品質データ圧縮                            |

### ライセンス遵守

各ライブラリはそれぞれのフリー/リブレソフトウェアライセンスに従って使用されています。各ライセンスの全文と著作権表示については、このリポジトリの `NOTICE` ファイルを参照してください。

---

## CMakeプリセット

Suika3には様々なプラットフォームとビルド設定をカバーするCMakeプリセットが付属しています。

|プリセット                          |プラットフォーム      |コンパイラ  |ディレクトリ                          |ターゲット    |タイプ         |
|------------------------------------|----------------------|-----------|---------------------------------------|--------------|---------------|
|windows-vs2022-x86-debug            |Windows               |MSVC       |out/build/windows-vs2022-x86-debug     |suika3.exe    |実行ファイル   |
|windows-vs2022-x86-release          |Windows               |MSVC       |out/build/windows-vs2022-x86-release   |suika3.exe    |実行ファイル   |
|windows-vs2022-x64-debug            |Windows               |MSVC       |out/build/windows-vs2022-x64-debug     |suika3.exe    |実行ファイル   |
|windows-vs2022-x64-release          |Windows               |MSVC       |out/build/windows-vs2022-x64-release   |suika3.exe    |実行ファイル   |
|windows-vs2022-arm64-debug          |Windows               |MSVC       |out/build/windows-vs2022-arm64-debug   |suika3.exe    |実行ファイル   |
|windows-vs2022-arm64-release        |Windows               |MSVC       |out/build/windows-vs2022-arm64-release |suika3.exe    |実行ファイル   |
|windows-vs2022-gdk-desktop          |Windows               |MSVC       |out/build/windows-vs2022-gdk-desktop   |suika3.exe    |実行ファイル   |
|windows-vs2022-gdk-xbox-xs          |Windows               |MSVC       |out/build/windows-vs2022-gdk-xbox-xs   |suika3.exe    |実行ファイル   |
|windows-vs2026-x86-debug            |Windows               |MSVC       |out/build/windows-vs2026-x86-debug     |suika3.exe    |実行ファイル   |
|windows-vs2026-x86-release          |Windows               |MSVC       |out/build/windows-vs2026-x86-release   |suika3.exe    |実行ファイル   |
|windows-vs2026-x64-debug            |Windows               |MSVC       |out/build/windows-vs2026-x64-debug     |suika3.exe    |実行ファイル   |
|windows-vs2026-x64-release          |Windows               |MSVC       |out/build/windows-vs2026-x64-release   |suika3.exe    |実行ファイル   |
|windows-vs2026-x64-console-release  |Windows               |MSVC       |out/build/windows-vs2026-x64-release   |suika3.exe    |実行ファイル   |
|windows-vs2026-arm64-debug          |Windows               |MSVC       |out/build/windows-vs2026-arm64-debug   |suika3.exe    |実行ファイル   |
|windows-vs2026-arm64-release        |Windows               |MSVC       |out/build/windows-vs2026-arm64-release |suika3.exe    |実行ファイル   |
|windows-vs2026-gdk-desktop          |Windows               |MSVC       |out/build/windows-vs2026-gdk-desktop   |suika3.exe    |実行ファイル   |
|windows-vs2026-gdk-xbox-xs          |Windows               |MSVC       |out/build/windows-vs2026-gdk-xbox-xs   |suika3.exe    |実行ファイル   |
|windows-mingw-x86                   |Windows               |MinGW-GCC  |build-mingw-x86                        |suika3.exe    |実行ファイル   |
|windows-mingw-x86_64                |Windows               |MinGW-GCC  |build-mingw-x86_64                     |suika3.exe    |実行ファイル   |
|windows-mingw-arm64                 |Windows               |MinGW-LLVM |build-mingw-arm64                      |suika3.exe    |実行ファイル   |
|windows-mingw-win95                 |Windows 9x            |MinGW-GCC  |build-mingw-win95                      |suika3.exe    |実行ファイル   |
|macos                               |macOS                 |Clang      |build-macos                            |Suika3.app    |アプリバンドル |
|macos-cli                           |macOS (CLI)           |Clang      |build-macos                            |Suika3.app    |アプリバンドル |
|linux                               |Linux (X11)           |GCC        |build-linux                            |suika3        |実行ファイル   |
|linux-wayland                       |Linux (Wayland)       |GCC        |build-linux                            |suika3        |実行ファイル   |
|linux-gdm                           |Linux (GBM)           |GCC        |build-linux                            |suika3        |実行ファイル   |
|linux-gdm-rot90                     |Linux (GBM)           |GCC        |build-linux                            |suika3        |実行ファイル   |
|linux-fbdev                         |Linux (fbdev)         |GCC        |build-linux                            |suika3        |実行ファイル   |
|linux-x11soft                       |Linux                 |GCC        |build-linux                            |suika3        |実行ファイル   |
|freebsd                             |FreeBSD               |Clang      |build-freebsd                          |suika3        |実行ファイル   |
|netbsd                              |NetBSD                |GCC        |build-netbsd                           |suika3        |実行ファイル   |
|openbsd                             |OpenBSD               |Clang      |build-openbsd                          |suika3        |実行ファイル   |
|solaris11                           |Solaris11             |SunCC      |build-solaris11                        |suika3        |実行ファイル   |
|solaris10                           |Solaris10             |SunCC      |build-solaris10                        |suika3        |実行ファイル   |
|haiku                               |Haiku OS              |GCC        |build-haiku                            |suika3        |実行ファイル   |
|wasm                                |WebAssembly           |Emscripten |build-wasm                             |index.html    |HTML + Wasm    |
|wasm-local                          |Chromebook            |Emscripten |build-wasm-local                       |index.html    |HTML + Wasm    |
|ios-device                          |iOS Device            |Clang      |build-ios-device                       |libsuika3.a   |スタティックライブラリ |
|ios-simulator                       |iOS Simulator         |Clang      |build-ios-simulator                    |libsuika3.a   |スタティックライブラリ |
|android-x86                         |Android x86           |Clang      |build-android-x86                      |libsuika3.so  |共有ライブラリ |
|android-x86_64                      |Android x86_64        |Clang      |build-android-x86_64                   |libsuika3.so  |共有ライブラリ |
|android-armv7                       |Android armv7         |Clang      |build-android-armv7                    |libsuika3.so  |共有ライブラリ |
|android-arm64                       |Android arm64         |Clang      |build-android-arm64                    |libsuika3.so  |共有ライブラリ |
|android-riscv64                     |Android riscv64       |Clang      |build-android-riscv64                  |libsuika3.so  |共有ライブラリ |
|openharmony-arm64                   |HarmonyOS NEXT arm64  |Clang      |build-openharmony-arm64                |libsuika3.a   |スタティックライブラリ |
|openharmony-armv7                   |HarmonyOS NEXT armv7  |Clang      |build-openharmony-armv7                |libsuika3.a   |スタティックライブラリ |
|openharmony-x86_64                  |HarmonyOS NEXT x86_64 |Clang      |build-openharmony-x86_64               |libsuika3.a   |スタティックライブラリ |
|unity-win64                         |Unity Plugin          |Clang-CL   |build-unity-win64                      |libsuika3.dll |DLLプラグイン  |
|unity-switch                        |Unity Plugin          |Clang      |build-unity-switch                     |libsuika3.a   |スタティックライブラリ |
|unity-ps5                           |Unity Plugin          |Clang      |build-unity-ps5                        |libsuika3.a   |スタティックライブラリ |
|unity-xbox                          |Unity Plugin          |Clang      |build-unity-xbox                       |libsuika3.a   |スタティックライブラリ |

---

## コードベースと成熟度

Suika3は10万行以上のコード（LoC）を持つ堅牢なビジュアルノベルエンジンです。週末プロジェクトではなく、25年以上にわたって進化してきた成熟したコードベースです。

- **実証された安定性:** 2001年から改良されてきたコアモジュールを含みます。

- **モダンなアーキテクチャ:** 2016年に分離されたクリーンなHAL（ハードウェア抽象化レイヤー）と、2022年に実装された高性能GPUレンダリングを特徴とします。

- **ネイティブマルチプラットフォーム:** 主にCで構築され、C++（DirectX）、Swift（macOS/iOS）、Objective-C（macOS/iOS）、C#（Unity）、シェーダー（HLSL/GLSL/Metal）などのプラットフォームネイティブ言語も使用しています。

---

## 品質保証

**ソフトウェアエンジニアリング** において、信頼性は本質的に時間の関数です。Suika3は2026年4月に新規リリースされたため、エンジンはフィールドテスト済みの稼働時間という観点では初期ライフサイクル段階にあります。そのため、正式な品質メトリクスはまだ完全には適用できません。ただし、アーキテクチャは20年以上の開発の専門知識を継承しており、最初のLTS（長期サポート）に向けたQAプロセスの強固な基盤を確保しています。

### 安定性への道筋

私たちは本番環境向けエンジンの提供に尽力しています。QAロードマップは以下の通りです:

1. **バグ追跡**: 特定されたすべての問題の包括的なログを維持しています。[BUGS.md](BUGS.md) を参照

2. **データドリブンの強化**: 根本原因と解決率を分析してソフトウェアの安定性を定量化・改善します。

3. **商業グレードの基準**: 最終目標は、商業アプリ制作の厳格な要件を満たす堅牢性のレベルを達成することです。

---

## 採用状況

「モバイルビジュアルノベル」という新しい市場の創出を目指しており、まだ実績を確立していません。

**アーリーアダプター** としてこの新しい市場の形成に関わることに興味がある方は、ぜひコミュニティへの参加をお待ちしています。

実績のあるツールを好む場合は、Suika3がさらに成熟してからプロジェクトを再検討することをお勧めします。

---

## リポジトリ構成

```
include/                           # 公開ヘッダー
src/                               # エンジンソースコード
resources/                         # エンジン用アセットとリソース
  projects/                          # iOS、Androidなど向けの公式プロジェクトベース
cmake/                             # CMake設定ファイルとプリセット
docs/                              # ドキュメントソースファイル (MkDocs)
external/                          # サードパーティライブラリと依存関係
  PlayfieldEngine/                   # コア2Dゲームエンジン
    external/                          # Playfield Engineのサブ依存関係
      NoctLang/                          # エンジンが使用するスクリプト言語
        include/                           # NoctLangの公開ヘッダー
        src/                               # NoctLangのソースコード
          core/                              # NoctLangのコア実装
      StratoHAL/                         # クロスプラットフォームサポート用ハードウェア抽象化レイヤー
        include/                           # StratoHALの公開ヘッダー
        src/                               # StratoHALのソースコード
```

---

## アセットファイルフォーマット

- 画像:
    - 対応フォーマット: PNG, JPEG, WebP
- オーディオ:
    - 対応フォーマット: Ogg Vorbis, 44100Hz, ステレオまたはモノラル
- フォント:
    - 対応フォーマット: TrueType (TTF), OpenType (OTF)

---

## ゲームのパッケージングと配布

| OS                 | アセット                                                                                                                           |
|--------------------|--------------------------------------------------------------------------------------------------------------------------------|
| Windows            | アセットは `assets.arc` というファイル名に格納してください。                                                                        |
| macOS              | アセットはアプリバンドル内の `Contents/Resources/assets.arc` に格納してください。                                                  |
| Linux              | アセットは `assets.arc` というファイル名に格納してください。                                                                        |
| iOS                | アセットはアプリバンドル内の `Contents/Resources/assets.arc` に格納してください。                                                  |
| Android            | アセットはパックしないでください。`app/src/main/assets/` フォルダに通常のファイルとして直接追加してください。                        |
| HarmonyOS NEXT     | アセットはパックしないでください。`entry/src/main/resources/rawfiles/` フォルダに通常のファイルとして直接追加してください。          |
| WebAssembly (Wasm) | アセットは `assets.arc` に格納し、`index.html` と同じ場所に置いてください。                                                        |
| Unity              | アセットはパックしないでください。`Assets/Resources/StreamingAssets/` フォルダに通常のファイルとして直接追加してください。          |

ゲームアセットから `assets.arc` を生成するには `suika3-pack` ツールを使用してください。`suika3-pack` ツールはエンジンが効率的にロードできる難読化されたアーカイブを作成します。（難読化アルゴリズムはキー回転XORであり、強固なセキュリティを意図したものではありません。カジュアルな改ざんを防ぐのに役立ちます。難読化キーは「key.h」で変更できます。）

Windows、macOS、Linuxでは、Visual Studio Codeの定義済みタスクを使用できます。

- 検索バーをクリック
- `Run Task` をクリック
- `Suika3: Create a package file` をクリック

---

## エンジン機能一覧

**2Dゲームサポート:**
- テクスチャソース:
    - カラー
    - 画像
    - フォント
- レンダリング:
    - ピクセル単位
    - スケーリング
    - 3Dトランスフォーム
- オーディオ再生 (Vorbis)
- アセットファイルアクセス
- セーブデータアクセス

**ビジュアルノベル標準サポート:**
- メッセージ表示
- 選択肢
- 背景・キャラクター表示・トランジション
- サウンド再生
- 動画再生
- レイヤードラスター画像アニメーション（シフト、スケール、回転）
- UI/UX構築
- セーブ/ロード
- スキップモード
- オートモード
- 既読スキップ
- メッセージ履歴表示とボイス再生
- CGギャラリー
- ローカライゼーション
- シミュレーションパラメーター表示

**将来の予定:**
- 2D RPGマップ移動
- 3Dキャラクターモデル
- ネットワークプレイ

---

## ロードマップ

- ビジュアルノベル（完了）
- ストーリーパートを持つその他の2Dゲーム

---

## スクリーンショット

<div align="center">
  <img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/screenshot-win11.webp" alt="Windows 11のSuika3" width="320" hspace="20">
  <p>Windows 11</p><br>
  <img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/screenshot-ios.webp" alt="iOSのSuika3" width="320" hspace="20">
  <p>iOS</p><br>
  <img src="https://raw.githubusercontent.com/awemorris/suika3/refs/heads/main/docs/img/screenshot-android.webp" alt="AndroidのSuika3" width="320" hspace="20">
  <p>Android</p><br>
</div>

---

## バージョニングポリシー

`年.月.パッチレベル`

| バージョン | レベル                             | リリース日       | サポート期間        |
|----------|------------------------------------|-----------------|---------------------|
| 26.04    | 機能ロールアップ / LTS RC1          | 2026年4月       | - コードネーム「北辰天歸」 |
| 26.05    | 機能ロールアップ / LTS RC2          | 2026年5月       | -                   |
| 26.06    | 機能ロールアップ / LTS RC3          | 2026年6月       | -                   |
| 26.07    | 長期サポート                        | 2026年7月       | 10年以上（最低）     |
| 26.08    | 機能ロールアップ                    | 2026年8月       | -                   |
| 26.09    | 機能ロールアップ                    | 2026年9月       | -                   |
| 26.10    | 機能ロールアップ                    | 2026年10月      | -                   |
| 26.11    | 機能ロールアップ                    | 2026年11月      | -                   |
| 26.12    | 機能ロールアップ                    | 2026年12月      | -                   |
| 27.01    | 機能ロールアップ                    | 2027年1月       | -                   |
| 27.02    | 機能ロールアップ                    | 2027年2月       | -                   |
| 27.03    | 機能ロールアップ                    | 2027年3月       | -                   |
| 27.04    | 機能ロールアップ / LTS RC1          | 2027年4月       | -                   |
| 27.05    | 機能ロールアップ / LTS RC2          | 2027年5月       | -                   |
| 27.06    | 機能ロールアップ / LTS RC3          | 2027年6月       | -                   |
| 27.07    | 長期サポート                        | 2027年7月       | 10年以上（最低）     |
| 27.08    | 機能ロールアップ                    | 2027年8月       | -                   |
| 27.09    | 機能ロールアップ                    | 2027年9月       | -                   |
| 27.10    | 機能ロールアップ                    | 2027年10月      | -                   |
| 27.11    | 機能ロールアップ                    | 2027年11月      | -                   |
| 27.12    | 機能ロールアップ                    | 2027年12月      | -                   |
| 28.01    | 機能ロールアップ                    | 2028年1月       | -                   |
| 28.02    | 機能ロールアップ                    | 2028年2月       | -                   |
| 28.03    | 機能ロールアップ                    | 2028年3月       | -                   |
| 28.04    | 機能ロールアップ / LTS RC1          | 2028年4月       | -                   |
| 28.05    | 機能ロールアップ / LTS RC2          | 2028年5月       | -                   |
| 28.06    | 機能ロールアップ / LTS RC3          | 2028年6月       | -                   |
| 28.07    | 長期サポート                        | 2028年7月       | 10年以上（最低）     |

---

## ライセンス

Suika3は許容的な [フリー/リブレソフトウェア](https://www.gnu.org/philosophy/free-sw.en.html) であり、許容的な `ZLibライセンス` のもとリリースされています。

* 商業ゲームにSuika3を使用できます。
* ゲームコードをオープンソースにする必要はありません。
* エンジンを改変・再配布できます。
* クレジット表記は歓迎しますが、必須ではありません。

```
Suika3
Copyright (c) 2026 Awe Morris / SCHOLA SUIKAE
```

完全なライセンスは [LICENSE](LICENSE) と [NOTICE](NOTICE) で確認できます。

---

## サポートと連絡先

一般的な問い合わせ、バグ報告、技術的なサポートについて:

- **GitHub Issues**: 正式なバグ追跡と機能リクエストに最適です。

- **[Discordサーバー](https://discord.gg/YZsq9u9Mgr)**: リアルタイムの議論と素早い質問に最適な場所です。

クリエイターが自らのストーリーを世に届けられるよう、タイムリーなサポートの提供に努めています。

### プロフェッショナル＆エンタープライズサポート

**コミュニティ** がGitHubとDiscordを通じて積極的なサポートを提供していますが、商業制作ではしばしば専用サポートや保証された応答時間が必要になることも理解しています。

**正式なメンテナンス契約、優先バグ修正、プライベート相談** が必要なスタジオや開発者は、メールで直接ご連絡ください。プロジェクトを成功させるためのサポートに尽力します。

- **連絡先**: Awe Morris <awe@suika3.vn>

**正式な契約はいらないですか？問題ありません。** インディー開発者や趣味の方でカジュアルにやり取りしたい場合は、GitHub IssueやDiscordでご質問ください。いつでも喜んでお話し、できるだけ早く返答や修正を行います！;-)

---

## コミュニティ

### Discord

[Discordサーバー](https://discord.gg/YZsq9u9Mgr)

私たちのサーバーはあらゆる形の差別を許容しません。人種、性別、スキルレベル、神経多様性を含むがこれに限らない、すべての人間の違いを受け入れる包括的な空間です。

### 募集

現在、以下の分野でのコントリビューターを探しています:

* ドキュメント編集者
* テスト担当ソフトウェアエンジニア
* HarmonyOS NEXTテスター

### インクルーシビティへのコミットメント

すべての人に歓迎的で安全な環境を提供することに尽力しています。あらゆる形の差別は厳しく禁じられています。

私たちのコミュニティは、以下を含むがこれに限らない、あらゆるバックグラウンドの開発者を温かく歓迎します:

- **アイデンティティと表現:** 人種、民族、国籍、言語、性自認と表現、性的指向、性的特徴。

- **個人的な属性:** 年齢、体型、外見、疾患、神経多様性。

- **生活の文脈:** 信仰、信念、障害（可視的なものに限らない）、経験レベル、教育、社会経済的地位。

- **正義と包摂:** 私たちは前科を持つ個人を明示的に歓迎します。更生の力と、礼儀正しく参加するすべてのコントリビューターの固有の価値を強く信じているからです。

### ガバナンス

Suika3は、ダウンストリームでカスタマイズして使ってもらうために、信頼できるクリーンなアップストリームのコードベースを提供しています。
ユーザーは自分のニーズに合わせたプライベートフォークを維持することを推奨されています。
独自のリブランドバージョンを作成することは、プロジェクトの自然かつ完全にサポートされた利用方法です。

この理念のため、Suika3は正式なガバナンス構造を採用していません。
その代わり、プロジェクトはコミュニティユーザーとの緊密な協力のもと、リードメンテナーによって推進されています。

---

## コントリビューション

どなたからの貢献も歓迎します。
バグの修正、ドキュメントの改善、新機能の提案など、あなたの協力が **Suika3** を前進させます。

---

## 系譜: 長い長い旅路

Suika3は20年以上にわたる不断のイノベーションと開発の集大成です:

- **Suika Studio (2001〜2004)**: 独自のGUIベースのエディタを備えた、私たちのコードベースの起源。[アーカイブ](https://github.com/awemorris/suika-studio-2002)

- **Unfruitiful (2005〜2015)**: クロスプラットフォームサポートのための堅牢な移植性レイヤーの確立に焦点を当てた10年間のR&D。

- **Suika2 (2016〜2024)**: 現在のアーキテクチャの礎石であり、10年間の専念したR&Dの成果。広範なプラットフォーム互換性を持つ完全なビジュアルノベル体験を提供します。Suika3を使ったいくつかのゲームがApp Storeで販売されました。[アーカイブ](https://github.com/awemorris/suika2)

- **Playfield Engine (2025〜)**: Suikaシリーズのコア移植性レイヤーから派生した多目的2Dエンジン。[リポジトリ](https://github.com/awemorris/PlayfieldEngine)

- **Suika3 (2026〜)**: これらの遺産を統合し、`NovelML` と `Ray` を導入することで、Suika3は前身の安定性と現代技術の柔軟性を両立させています。

---

## なぜSuika3なのか？: 私たちの理念

私たちは既存のVN市場を見ていません。まだ存在しないモバイルVN市場の開拓を目指しています。

異なる目標を持つクリエイターは、Ren'Py、Unity、Godotなどのエンジンにも優れたリソースを見つけることができます。

### ミッション: 持続可能なエコシステムの構築

ビジュアルノベルの商業的成功への献身は、メディアの長期的な健全性への戦略的対応です。2030年代にビジュアルノベルが栄えるためには、趣味の域を超え、活気ある創造産業として再確立される必要があると信じています。

2010年代以降、ビジュアルノベル市場は大きな成長課題に直面しています。重要なボトルネックの一つは、**iOSとAndroid上でシームレスなネイティブ体験を提供できる高性能ソフトウェアエンジンの不足** でした。開発者は通常PCで作業しますが、今日ほとんどの人にとって主要な個人用コンピューターはスマートフォンです。

現代のモバイルプラットフォーム向けのアクセシブルなプロフェッショナルグレードのツールがなければ、多くのクリエイターは限られた配信チャネルに制限されてきました。その結果、メディアはグローバルなモバイルファーストのオーディエンスに届くことが困難となり、全体的な拡大が遅れました。

無料の趣味プロジェクトは文化的に価値があり不可欠ですが、それだけでは必ずしも大きな創造産業を持続できないかもしれません。真に健全なエコシステムには以下が必要かもしれません:

- **商業的に実行可能なゲーム** — プレイヤーが主要デバイスで利用でき、クリエイターが長期的なキャリアを築くために必要な経済活動を生み出すタイトル。

- **経済的自立** — ストーリーテリングを通じて、特にアジアやグローバルサウスの才能が経済的障壁を克服できるよう支援。

- **生きている産業** — メディアをニッチな関心事から、創造性が自立につながる持続可能な市場へと変革すること。

私たちの見解では、商業的成功は当然のプロフェッショナルとしての期待です。

この理念は趣味や実験的なプロジェクトを否定するものではないことに注意してください。私たちは単に異なる問題空間に焦点を当てています: スケールでの持続可能性。

### ビジョン: 成長の触媒

私たちは単なるツールの提供を目指しているのではありません。私たちの目標は **成長の触媒** を提供することです。

重いフレームワークのオーバーヘッドなしにネイティブパフォーマンスを提供する「どこでも移植できる」エンジンを提供することで、開発者が最も重要なことに集中できるようにします: **語り継がれるストーリーを語ること。**

ビジュアルノベルを愛しているからこそ、メディアを前進させ、今後数十年にわたって活気のある経済的に実行可能な芸術形式として維持することに尽力しています。

### 価値観: 持続可能なキャリアのための才能の支援

私たちの価値観は、特にアジアやグローバルサウスの才能が、ストーリーテリングを通じて持続可能なキャリアを築けるよう支援することです。
適切なツールがあれば、創造性は経済的障壁を克服できると信じています。

許容的ライセンスへのコミットメントは、ソフトウェア、ハードウェア、予算の制限に関わらず、App StoreとGoogle Playでオリジナルビジュアルノベルを出版・販売する人々のためのものです。

---

## FAQ

- [これは何ですか？](#これは何ですか)
- [Ren'Py、Unity、Godotなどの既存エンジンの競合ですか？](#renpy-unity-godotなどの既存エンジンの競合ですか)
- [今すぐ使って大丈夫ですか？](#今すぐ使って大丈夫ですか)
- [「25年以上の成熟度」と「運用実績ゼロ」は矛盾しませんか？](#25年以上の成熟度と運用実績ゼロは矛盾しませんか)
- [バスファクター1ですよね？](#バスファクター1ですよね)
- [企業は独自のフォークを維持できますか？](#企業は独自のフォークを維持できますか)
- [誰が意思決定をしますか？](#誰が意思決定をしますか)
- [Suika3はオープンソースですか？商業利用できますか？](#suika3はオープンソースですか商業利用できますか)
- [サードパーティライブラリのライセンスはどうなっていますか？](#サードパーティライブラリのライセンスはどうなっていますか)
- [iOS/コンソールでJITが無効なのですが、どうすればいいですか？](#iosコンソールでjitが無効なのですがどうすればいいですか)
- [スクリプトは安全ですか？ファイルやネットワークにアクセスできますか？](#スクリプトは安全ですかファイルやネットワークにアクセスできますか)
- [ストア審査を通過しやすいですか？](#ストア審査を通過しやすいですか)
- [「すべての現代プラットフォームをサポート」は本当ですか？](#すべての現代プラットフォームをサポートは本当ですか)
- [コンソールサポートの「Unity経由」とはどういう意味ですか？](#コンソールサポートのunity経由とはどういう意味ですか)
- [HarmonyOS NEXTはどの程度サポートされていますか？](#harmonyos-nextはどの程度サポートされていますか)
- [「2.5〜4.5倍速い」は再現できますか？測定条件は？](#25-45倍速いは再現できますか測定条件は)
- [旧世代GCは10〜300msかかりますが、フレームドロップしませんか？](#旧世代gcは10-300msかかりますがフレームドロップしませんか)
- [ゲーム制作はどこから始めればいいですか？サンプルはありますか？](#ゲーム制作はどこから始めればいいですかサンプルはありますか)
- [プラグインの開発方法は？](#プラグインの開発方法は)
- [既存のアセット（Ren'PyやUnityなど）から移行できますか？](#既存のアセットrenpy-やunityなどから移行できますか)
- [ドキュメントはどこにありますか？最新版ですか？](#ドキュメントはどこにありますか最新版ですか)
- [日本語/中国語/...はサポートされていますか？](#日本語中国語はサポートされていますか)
- [困ったときはどこに相談すればいいですか？](#困ったときはどこに相談すればいいですか)
- [企業向けのSLAやメンテナンス契約はありますか？](#企業向けのslaやメンテナンス契約はありますか)
- [最小動作要件は？](#最小動作要件は)
- [「サポート済み」とはどういう意味ですか？](#サポート済みとはどういう意味ですか)
- [破壊的変更はありますか？互換性ポリシーは？](#破壊的変更はありますか互換性ポリシーは)
- [DLCやアプリ内課金はサポートしていますか？](#dlcやアプリ内課金はサポートしていますか)
- [なぜAppleとiPhoneに深くこだわるのですか？](#なぜappleとiphoneに深くこだわるのですか)
- [なぜiOSでPythonを使うのは難しいのですか？](#なぜiosでpythonを使うのは難しいのですか)

### これは何ですか？

Suika3は、プロのスタジオと商業アプリ開発を念頭に置いて設計された次世代ビジュアルノベルエンジンです。ネイティブ実装（主にC）による高性能、長期的なメンテナンス性、幅広いプラットフォームサポートを目指しています。

### Ren'Py、Unity、Godotなどの既存エンジンの競合ですか？

競合というより、異なる問題領域をターゲットにしています。Suika3は「商業利用に耐えられるモバイル中心のVN市場の創出」という方向性を明確に示しており、既存のエンジンを有力な選択肢として尊重しています。

### 今すぐ使って大丈夫ですか？

最初のLTSリリースに向けた安定化は順調に進んでおり、品質メトリクスは着実に改善されています。26.05シリーズはすでに一般利用に十分な安定性を持っています。

ユーザーベースはまだ小さいため、早期採用するか、さらなる成熟を待つかを選択できますが、GitHub Issuesは通常1日以内に解決されます。これにより、すべてのユーザーに高いレベルのコミュニティサポートが保証されています。

### 「25年以上の成熟度」と「運用実績ゼロ」は矛盾しませんか？

Suika3自体は新しい統合ですが、Suika2やStratoHALを含む長い系譜の上に構築されています。つまり「古い基盤を持つ新しいパッケージ」です。

### バスファクター1ですよね？

ポリシーでは「フォークとリブランドの奨励」と「正式なガバナンスなしのクリーンなアップストリームの維持」を明示しており、ダウンストリームのメンテナンスを容易にするアプローチを育んでいます。

### 企業は独自のフォークを維持できますか？

「ソフトウェア要件仕様書」（SRS）と「ソフトウェア設計仕様書」（SDS）の両方が提供されており、エンタープライズメンテナンスに最適です。さらに「プロフェッショナル/エンタープライズサポート」が利用可能で、採用組織への必要なノウハウの移転が可能です。

### 誰が意思決定をしますか？

リード開発者が意思決定を行います。コミュニティのための正式なガバナンス構造は維持していません。

### Suika3はオープンソースですか？商業利用できますか？

Suika3はzlibライセンスのもとリリースされており、FOSSの定義のもとオープンソースソフトウェア（OSS）として認定されています。商業プロジェクトで自由に使用できます。

ただし、GitHubでよく見られる多くのプロジェクトとは若干異なる位置づけです。多くのOSSプロジェクトが既存の大規模なエコシステムの上に構築されることで繁栄するのに対し、Suika3はほぼゼロから開発されたコアコンポーネントを中心に設計されており、外部依存関係は絶対的な最小限に抑えられています。

GNU/Linuxの哲学とより広いオープンソースコミュニティに深い敬意を持ちながらも、常に私たち自身の厳格な基準を満たす「商業グレードの品質」を提供することを目標としてきました。この強い独立性のため、Suika3は典型的なコミュニティ主導のOSSプロジェクトよりも「OSSとして提供された商業ソフトウェア」に近く感じられるかもしれません。

そのため、このプロジェクトを基にしたクローズドソースフォークや商業製品を積極的に歓迎します。

### サードパーティライブラリのライセンスはどうなっていますか？

Suika3で使用されているサードパーティライブラリの各ライセンスの全文と著作権表示については、[NOTICE](NOTICE) を参照してください。

### iOS/コンソールでJITが無効なのですが、どうすればいいですか？

公式バイナリはこれらの環境でインタープリターを使用します。インタープリターは十分に高速だと考えていますが、速度が必要な場合や、インタープリターでさえストア審査の問題になる場合は、[AOT](docs/mkdocs-en/aot.md) を使用してください。これはインタープリターと比較して2.5〜4.5倍の速度を誇る完璧なソリューションです。

### スクリプトは安全ですか？ファイルやネットワークにアクセスできますか？

スクリプトはサンドボックス環境で実行され、リソースバンドル外のファイルやネットワークにアクセスできません。ゲームロジックに安全なよう設計されたエンジンが公開するAPIにのみアクセスできます。

### ストア審査を通過しやすいですか？

AOTコンパイルを使用することで、ストア審査のハードルが劇的に下がると考えています。汎用スクリプトを実行するのではなく、単純にタグを実行するネイティブアプリになるためです。ただし、ストア審査の結果は、グラフィックデザイン、ユーザー体験、コンテンツを含むゲームの品質にかかっている場合があるため、結果を保証することはできません。

### 「すべての現代プラットフォームをサポート」は本当ですか？

信じがたいとは思いますが、私たちの確認ではすべて動作しています。ただし、製品としてリリースする際は、ご自身の責任でQA/QCを実施してください。

### コンソールサポートの「Unity経由」とはどういう意味ですか？

Suika3をコンソールに移植しても、NDAのためにソースコードを開示できません。ただし、コンソール向けのUnityの専用バージョンが利用可能です。そのため、Suika3をUnityプラグインとして移植しました。このプラグインを使用することで、Unity内でSuika3を実行できます。つまり、コンソール版のUnityと「デベロッパーキット」マシンがあれば、コンソールでもSuika3を実行できます。

コンソールにはUI/UXの要件があるため、追加で実装する必要があることをご理解ください。

### HarmonyOS NEXTはどの程度サポートされていますか？

コア開発者は全員が中国外に在住しており、中国で販売される実機を所持していません。そのため、機能はエミュレーターでのみ検証されています。

### 「2.5〜4.5倍速い」は再現できますか？測定条件は？

これはパフォーマンスの差異を測定するための合成ベンチマークです。

測定コード:
```
func main() {
    var sum = 0;
    for(i in 0..10000) {
        for(j in 0..100000) {
            sum = sum + 1;
        }
    }
}
```

| マシン                  | JIT (秒)     | インタープリター (秒)  | スケーリング (JIT vs インタープリター) |
|-------------------------|--------------|-----------------------|------------------------------|
| Intel Core i9 12900H    | 3.32         | 13.2                  | 4.0x                         |
| Intel Core Ultra 5 228V | 5.78         | 15.6                  | 2.7x                         |
| Intel Xeon Silver 4114  | 8.08         | 36.4                  | 4.5x                         |
| Apple M5                | 2.77         | 10.6                  | 3.8x                         |
| IBM POWER8              | 43.719       | 117.8                 | 2.7x                         |

実際のゲームアプリでは、パフォーマンスの差異はロジックとスクリプト実行量によって変わる可能性があります。ただし、典型的なビジュアルノベルのシナリオでは、AOT使用時にインタープリターと比較して約5倍の高速化が観察されています。

### 旧世代GCは10〜300msかかりますが、フレームドロップしませんか？

旧世代GCはほとんど起動されません。画像などの大型バッファはCによって管理され、適切に手動で解放されます。GCの対象はスクリプト内の文字列と辞書オブジェクトであり、異常が発生しない限り旧世代GCは起動されません。

### ゲーム制作はどこから始めればいいですか？サンプルはありますか？

Suika3で最初のゲームを作成するためのステップバイステップガイドは [はじめかたガイド](docs/mkdocs-en/docs/getting-started.md) を参照してください。このリポジトリの `game/` ディレクトリにサンプルゲームも提供しており、自分のプロジェクトの参考や出発点として使用できます。

### プラグインの開発方法は？

Suika3のプラグインを作成する詳細な手順は [プラグイン開発ガイド](docs/mkdocs-en/docs/plugin.md) を参照してください。

### 既存のアセット（Ren'PyやUnityなど）から移行できますか？

モバイルをターゲットにする必要があり、既存のエンジンでそれが困難な場合にのみ移行を検討してください。それ以外の場合、移行の労力は割に合わないかもしれません。

画像とオーディオはフォーマットを一括変換することで移行できますが、UIは手動での移植が必要です。シナリオも手動での移植が必要ですが、AIによってある程度の自動変換が可能かもしれません。

### ドキュメントはどこにありますか？最新版ですか？

最新のドキュメントは [Documentation](docs/mkdocs-en/docs/index.md) を参照してください。

### 日本語/中国語/...はサポートされていますか？

ゲームはUnicode準拠なので、Unicode合成を使用しない言語であれば、ゲームコンテンツに任意の言語を使用できます。エンジン自体は国際化されており、エラーメッセージはローカル言語で表示されます。

日本語とCJKゲームに対して、Suika3は以下をサポートしています:
* 禁則処理を含む行分割ルール
* ルビ注釈
* 縦書き

右から左への書き記しシステムは、開発チームにその言語話者がいないため、現時点では未実装です。実装に協力できる方はぜひ開発者に連絡してください。

### 困ったときはどこに相談すればいいですか？

GitHubまたはDiscordで質問してください。GitHubに不慣れでDiscordが嫌いな場合は、メールでも大丈夫です ;-)

### 企業向けのSLAやメンテナンス契約はありますか？

必要なサービスレベルをご要望ください。バグ修正のみをカバーする契約の場合、費用は通常月額100ドル程度です。機能追加を含む包括的な契約の場合、1ゲームタイトルあたり約5000ドルです。新興国にお住まいでこれらの費用を賄えない場合は、オプションを議論するためにご連絡ください。

### 最小動作要件は？

サポートされているプラットフォームとその要件の詳細については [互換性リスト](#互換性リスト) を参照してください。

- 公式構成
    - ディスプレイ: 640x480 24bpp
    - CPU: 1GHz
    - RAM: 512MB
    - GPU: DirectX 9 / OpenGL 3.0 / OpenGL ES 2.0 / Metal 1.0
    - ディスク: 2MB + アセット

- 組み込みデバイス向け最小構成
    - ディスプレイ: 任意サイズ（24bpp必須）
    - CPU: 200MHz
    - RAM: 32MB
    - GPU: 不要
    - ディスク: 2MB + アセット

### 「サポート済み」とはどういう意味ですか？

サンプルゲームが実機またはエミュレーターで動作することを検証するプロセスをパスしたことを示します。

これは単にそのプラットフォーム向けの動作するコードベースがあることを意味しており、ゲームの最終的なQAは自分で行う必要があります。

### 破壊的変更はありますか？互換性ポリシーは？

過去のリリースに対するAPI仕様への後方互換性を壊す変更は行いません。既存のAPI関数は変更されず、代わりに新しいAPIが追加されます。動作を変更する必要がある場合は、動作モードを設定するフラグが提供されます。なお、セーブデータの仕様はリリース間で互換性がない場合があります。単一の製品については、使用するメジャーリリースバージョンを固定してロックしてください。

### DLCやアプリ内課金はサポートしていますか？

いいえ、サポートしておらず、サポートする予定もありません。

代わりに、Apple Arcadeのようなサブスクリプション型の配信モデルを想定してください。このモデルでは、ビジュアルノベルは書籍のように長期間にわたって読まれる作品となり、YouTube動画に似た継続的な視聴とプレイ時間を通じて収益化されます。

モバイル上のビジュアルノベル市場は、このロングテール型の配信に適応しない限り生き残れません。

プロジェクトが少数のコアファンに支えられている場合でも、DLCへの依存よりもグッズ販売を推奨します。追加コンテンツは有料セグメントに分割するのではなく、無料アップデートとして提供すべきです。作品は常に単一の統一されたエクスペリエンスであるべきです。

現時点では、これは先進的な見方ですが、特にギャンブルの仕組みに似たアプリ内課金モデルは、地域ポリシーによって規制が強化される可能性があります。そのような規制が拡大し、より広いグローバルスタンダードになる可能性もあります。

そのため、アプリ内課金に依存しないビジネスモデルを構築することは、リスク軽減策であるだけでなく、より持続可能な収益への道でもあります。さらに重要なことは、各作品が利用可能で発見可能な状態を保ち、継続的にプレイされ続け、長期にわたって楽しまれ愛されることができるということです。

さらに、プラットフォームホルダーは配信の中核であり、不可欠なビジネスパートナーです。モバイルとコンソールでは、「作りたいもの」だけでなく「プラットフォーマーが期待する体験」に基づいてプロジェクトを設計する必要があります。これは純粋な芸術的創造とは異なる次元のように思えるかもしれません。しかし、プロのクリエイターとして、これは作品を持続的に届けるための不可欠なプロセスです。

ゲームサブスクリプションサービスはまだ新興・進化中の市場です。この段階でその成長に貢献することで、クリエイターはプラットフォームホルダーとの強い関係を築き、より大きな可視性とサポートから恩恵を受けることができます。エコシステムの形成への早期参加は、市場が成熟するにつれて報われるでしょう。

これがSuika3プロジェクトの核心的な戦略です。

### なぜAppleとiPhoneに深くこだわるのですか？

Apple製品は常に最も手頃な価格や広くアクセスしやすいものではないかもしれません。
しかし40年以上にわたり、Appleはコンピューティングの景観を継続的に再定義してきました。

私たちは彼らのプラットフォームをサポートすることを誇りに思い、iPhoneからVision Proまで、そのコミットメントは揺るぎません。

### なぜiOSでPythonを使うのは難しいのですか？

App Storeでは、Appleは動的コード実行を伴う「互換性のための仮想マシン（VM）」に対して事実上の厳格な制限を設けています。書かれたガイドラインに明示的に詳述されているのではなく、レビュープロセスの「暗黙のルール」として開発者の間で広く認識されています。顕著な例は、Javaランタイムを同梱した汎用アプリがほぼ皆無であることです。

対照的に、ゲーム開発でよく使われる `Lua` などの言語は比較的受け入れられています。これは、Luaが「サンドボックス化」しやすく、アプリのコア機能から隔離された制限されたスクリプト環境に適しているためです。言い換えると、レビュープロセス後にOSのコア機能とアプリの基本的な動作を任意に変更できない構造を可能にします。

しかし、`Python` は話が別です。Pythonはデフォルトで堅牢な組み込みサンドボックスメカニズムを持っておらず、設計によってはスクリプトがiOSの機能に幅広くアクセスできます。Appleの観点からは、これはレビュー後に重大な動作変更を許可する「制御困難なVM」と見なされることが多く、頻繁に却下の対象となります。

「ではなぜApp StoreにRen'Pyのゲームがあるのか？」と思うかもしれません。これが核心です。実際、Appleのレビュープロセスには一定の裁量的な運用があります。大手パブリッシャーからのプロジェクトや、「ストアエコシステム全体に価値を追加する」とAppleが判断した高品質な作品は、Pythonベースであっても通過する可能性があります。

本質的に、Ren'Pyでストアに挑戦するには、レビュワーを納得させるための圧倒的なクオリティが必要です。しかし、ビジュアルノベルは芸術的な感性に大きく依存するジャンルです。主に技術的なコンプライアンスに焦点を当てているレビュワーにその価値を瞬時に伝えることは簡単ではありません。

ここでSuika3の出番です。

Suika3は、スクリプトをAOT（事前コンパイル）でネイティブコードに事前変換するか、厳格に管理されたサンドボックスVM内で実行する設計を採用しています。このアプローチにより、App Storeのレビュープロセスに関連する技術的なリスクが大幅に軽減されます。

これらの技術的な障壁を最初から取り除くことで、Suika3は開発者が「作品のクオリティ」だけで勝負できるよう支援します。
