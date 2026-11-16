# Paxon パッケージリポジトリ

[English](README.md) · [한국어](README.ko.md) · **日本語** · [简体中文](README.zh.md)

Paxon は **sopsh**、**開発環境**、そして **ni エコシステム** の各コンポーネント向けの軽量パッケージマネージャーです。
paxon の公式デフォルトミラーは、この GitHub リポジトリです。

[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-18181b?style=flat&color=FFC0FF)](...)

ドキュメントや詳しい情報は [paxon の Web サイト](https://nknnt.com/paxon) をご覧ください。

-----

## インストール

paxon でパッケージをインストールします。

```bash
paxon -S <package-name>
```

netdust パッケージをインストールします。

```bash
paxon -S netdust@<package-name>
```
> [!NOTE]
> この機能には **paxon 1.4.4 以降** が必要です。

-----

## 非公開パッケージ

**paxon 1.7.0 以降** では、自分、許可した人、共有キーを持つ人だけがインストールできるパッケージを公開できます。非公開パッケージはこのリポジトリではなく [NKNNT Passport](https://passport.nknnt.com) に置かれ、`handle@package` という名前で扱います。

インストール:

```bash
paxon login                      # 最初に一度だけ。ブラウザが開きます
paxon -S <handle>@<package>
```

アカウントの追加ではなく共有キーを受け取った場合は、環境変数で指定してください。インストールコマンドをそのまま実行すると、paxon がキーを尋ねます(入力は画面に表示されません)。

```bash
PAXON_SHARE_KEY=nk_... paxon -S <handle>@<package>
```

> [!IMPORTANT]
> 共有キーはコマンドライン引数では受け付けません(paxon が拒否します)。`ps` やシェルの履歴に残さないためです。共有キーで行えるのは、そのパッケージ 1 つの読み取りだけで、公開やアカウントの操作はできません。

自分のパッケージを公開:

```bash
paxon login
paxon create mytool                          # または: paxon publish ... --create
paxon publish mytool ./mytool 1.0.0          # 名前、ファイル、バージョン
paxon logs mytool                            # 誰が何をアップロード/ダウンロードしたか(所有者のみ)
```

- アップロードの進捗が表示されます。接続が切れた場合は、**同じコマンドをもう一度実行すると中断した位置から再開します。**
- サーバーが SHA-256 とサイズを自分で計算し、一致しないファイルは拒否します。公開済みのバージョンを上書きすることはできません。
- サイズやパッケージ数の上限はプランによって異なります。`paxon whoami` でプランと使用量を確認できます。
- handle の決定、メンバーや共有キーの管理、ブラウザからのアップロードは [passport.nknnt.com](https://passport.nknnt.com) で行えます。

| コマンド | 説明 |
|---|---|
| `paxon login` / `logout` / `whoami` | ブラウザでログイン、ログアウト(サーバー上のトークンも失効)、アカウントと上限の表示 |
| `paxon create <name>` | 自分の handle の下に空の非公開パッケージを作成 |
| `paxon publish <name> <file> <version>` | バージョンをアップロード(再開可能)。`--create` で未作成ならパッケージも作成 |
| `paxon logs <name>` | 自分のパッケージの監査ログ |
| `paxon -S <handle>@<name>` | 非公開パッケージのインストール |

更新(`-Su`)、`-R`、`-Q`、`rollback`、`pin` は、非公開パッケージでも公開パッケージと同じように動作します。`-Su` はログイン中のみ非公開パッケージを確認します。

> [!NOTE]
> `netdust@<module>` はこれまでどおり、このリポジトリの netdust パッケージを指し、非公開パッケージには振り分けられません。handle の `netdust` と `meyku` は予約されています。

-----

## セキュリティとメンテナンス

ダウンロードは HTTPS 上で SHA-256 により検証されます。ハッシュのないパッケージは、ユーザー自身が承認しない限り拒否されます。非公開パッケージは常に検証され、未検証のままインストールすることはできません。

- [整合性とセキュリティ](Docs/security.md): ハッシュ検証、未検証パッケージ、ロールバックと固定(pin)
- [メンテナー向け](Docs/maintainers.md): パッケージインデックスの要件

ログインにはブラウザを使います(ローカルのループバックアドレスでの PKCE)。トークンは macOS のキーチェーンまたは Linux の Secret Service(`secret-tool`)に保存され、どちらもない場合は paxon の設定フォルダ内の `0600` ファイルに保存して、その旨を通知します。キーとトークンは `--verbose` でも表示されません。

-----

> [!CAUTION]
> ロジックの更新により、このリポジトリはバージョン 1.4.1 以前では動作しなくなりました。
>
> `paxon update` で最新バージョンに更新してください。
