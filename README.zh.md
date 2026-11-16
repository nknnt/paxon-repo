# Paxon 软件包仓库

[English](README.md) · [한국어](README.ko.md) · [日本語](README.ja.md) · **简体中文**

Paxon 是一个轻量级包管理器,服务于 **sopsh**、**开发环境**以及 **ni 生态系统**的其他组成部分。
paxon 官方默认镜像就是这个 GitHub 仓库。

[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-18181b?style=flat&color=FFC0FF)](...)

文档和更多信息请访问 [paxon 网站](https://nknnt.com/paxon)。

-----

## 安装

使用 paxon 安装软件包:

```bash
paxon -S <package-name>
```

使用 paxon 安装 netdust 软件包:

```bash
paxon -S netdust@<package-name>
```
> [!NOTE]
> 此功能需要 **paxon 1.4.4 或更高版本**。

-----

## 私有软件包

使用 **paxon 1.7.0 或更高版本**,你可以发布只有你自己、你授权的人,或持有分享密钥的人才能安装的软件包。私有软件包存放在 [NKNNT Passport](https://passport.nknnt.com),而不是这个仓库,名称格式为 `handle@package`。

安装:

```bash
paxon login                      # 只需一次,会打开浏览器
paxon -S <handle>@<package>
```

如果别人给你的是分享密钥,而不是把你的账号加入授权名单,可以通过环境变量传入;也可以直接运行安装命令,paxon 会提示你输入(输入内容不会显示在屏幕上):

```bash
PAXON_SHARE_KEY=nk_... paxon -S <handle>@<package>
```

> [!IMPORTANT]
> paxon 不接受把分享密钥作为命令行参数(会直接拒绝),以免密钥出现在 `ps` 或 shell 历史记录中。分享密钥只能读取对应的那一个软件包,不能发布,也不能访问你的账号。

发布你自己的软件包:

```bash
paxon login
paxon create mytool                          # 或者: paxon publish ... --create
paxon publish mytool ./mytool 1.0.0          # 名称、文件、版本
paxon logs mytool                            # 谁上传、下载了什么(仅所有者)
```

- 上传时会显示进度。如果连接中断,**再次运行相同的命令,就会从中断的位置继续上传。**
- 服务器会自行计算 SHA-256 和大小,与声明不符的文件会被拒绝。已发布的版本不能被覆盖。
- 大小和软件包数量的上限取决于你的套餐。用 `paxon whoami` 查看你的套餐和使用情况。
- 设置 handle、管理成员和分享密钥,以及在浏览器中上传,都可以在 [passport.nknnt.com](https://passport.nknnt.com) 完成。

| 命令 | 说明 |
|---|---|
| `paxon login` / `logout` / `whoami` | 通过浏览器登录、退出登录(同时在服务器上吊销令牌)、查看账号和额度 |
| `paxon create <name>` | 在你的 handle 下创建空的私有软件包 |
| `paxon publish <name> <file> <version>` | 上传一个版本(可断点续传)。`--create` 会在软件包不存在时创建 |
| `paxon logs <name>` | 查看你的软件包的审计日志 |
| `paxon -S <handle>@<name>` | 安装私有软件包 |

更新(`-Su`)、`-R`、`-Q`、`rollback` 和 `pin` 对私有软件包的行为与公开软件包相同。`-Su` 只会在已登录时检查私有软件包。

> [!NOTE]
> `netdust@<module>` 仍然表示本仓库中的 netdust 软件包,不会被当作私有软件包。handle `netdust` 和 `meyku` 为保留名称。

-----

## 安全与维护

下载内容通过 HTTPS 并使用 SHA-256 校验。没有哈希值的软件包会被拒绝,除非你自己确认允许。私有软件包始终会被校验,不能以未校验的方式安装。

- [完整性与安全](Docs/security.md):哈希校验、未校验的软件包、回滚与固定(pin)
- [面向维护者](Docs/maintainers.md):软件包索引的要求

登录通过浏览器完成(使用本机回环地址的 PKCE)。令牌保存在 macOS 钥匙串或 Linux Secret Service(`secret-tool`)中;如果两者都没有,则保存到 paxon 配置文件夹中权限为 `0600` 的文件里,并会提示你。密钥和令牌绝不会被打印出来,即使使用 `--verbose` 也一样。

-----

> [!CAUTION]
> 由于逻辑更新,本仓库不再支持 1.4.1 及更早版本。
>
> 请使用 `paxon update` 更新到最新版本。
