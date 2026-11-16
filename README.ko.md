# Paxon 패키지 저장소

[English](README.md) · [简体中文](README.zh.md) · [日本語](README.ja.md) · **한국어**

Paxon은 **sopsh**, **개발 환경**, 그리고 **ni 생태계**의 여러 구성 요소를 위한 가벼운 패키지 매니저입니다.
paxon의 공식 기본 미러는 이 GitHub 저장소입니다.

[![Platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20%7C%20windows-18181b?style=flat&color=FFC0FF)](...)

문서와 자세한 정보는 [paxon 웹사이트](https://nknnt.com/paxon)를 참고하세요.

-----

## 설치

paxon으로 패키지를 설치합니다.

```bash
paxon -S <package-name>
```

netdust 패키지를 설치합니다.

```bash
paxon -S netdust@<package-name>
```
> [!NOTE]
> 이 기능은 **paxon 1.4.4 이상**이 필요합니다.

-----

## 비공개 패키지

**paxon 1.7.0 이상**에서는 나 자신, 내가 허용한 사람, 공유 키를 가진 사람만 설치할 수 있는 패키지를 게시할 수 있습니다. 비공개 패키지는 이 저장소가 아니라 [NKNNT Passport](https://passport.nknnt.com)에 있으며 `handle@package` 형식의 이름을 사용합니다.

설치하기:

```bash
paxon login                      # 처음 한 번, 브라우저가 열립니다
paxon -S <handle>@<package>
```

계정을 추가받지 않고 공유 키를 받았다면 환경 변수로 지정하세요. 그냥 설치 명령을 실행하면 paxon이 키를 물어봅니다(입력이 화면에 보이지 않습니다).

```bash
PAXON_SHARE_KEY=nk_... paxon -S <handle>@<package>
```

> [!IMPORTANT]
> 공유 키는 명령줄 인자로 받지 않습니다(paxon이 거부합니다). `ps`나 셸 기록에 남지 않게 하기 위해서입니다. 공유 키로는 해당 패키지 하나를 읽기만 할 수 있고, 게시하거나 계정에 접근할 수 없습니다.

내 패키지 게시하기:

```bash
paxon login
paxon create mytool                          # 또는: paxon publish ... --create
paxon publish mytool ./mytool 1.0.0          # 이름, 파일, 버전
paxon logs mytool                            # 누가 무엇을 올리고 받았는지 (소유자만)
```

- 업로드 진행률이 표시됩니다. 연결이 끊기면 **같은 명령을 다시 실행하세요. 멈춘 지점부터 이어서 올립니다.**
- 서버가 SHA-256과 크기를 직접 계산해서 일치하지 않는 파일은 거부합니다. 이미 올린 버전은 덮어쓸 수 없습니다.
- 크기와 패키지 수 한도는 플랜에 따라 다릅니다. `paxon whoami`로 내 플랜과 사용량을 볼 수 있습니다.
- handle 정하기, 사람과 공유 키 관리, 브라우저에서 업로드는 [passport.nknnt.com](https://passport.nknnt.com)에서 할 수 있습니다.

| 명령 | 설명 |
|---|---|
| `paxon login` / `logout` / `whoami` | 브라우저로 로그인, 로그아웃(서버의 토큰도 폐기), 계정과 한도 보기 |
| `paxon create <name>` | 내 handle 아래에 빈 비공개 패키지 만들기 |
| `paxon publish <name> <file> <version>` | 버전 업로드(이어서 올리기 가능). `--create`는 패키지가 없으면 만듭니다 |
| `paxon logs <name>` | 내 패키지의 감사 로그 |
| `paxon -S <handle>@<name>` | 비공개 패키지 설치 |

업데이트(`-Su`), `-R`, `-Q`, `rollback`, `pin`은 비공개 패키지에도 공개 패키지와 똑같이 동작합니다. `-Su`는 로그인한 상태에서만 비공개 패키지를 확인합니다.

> [!NOTE]
> `netdust@<module>`은 계속 이 저장소의 netdust 패키지를 뜻하며, 비공개 패키지로 연결되지 않습니다. handle `netdust`와 `meyku`는 예약되어 있습니다.

-----

## 보안과 유지보수

다운로드는 HTTPS 위에서 SHA-256으로 검증합니다. 해시가 없는 패키지는 사용자가 직접 승인하지 않는 한 거부됩니다. 비공개 패키지는 항상 검증되며 검증 없이 설치할 수 없습니다.

- [무결성과 보안](Docs/security.md): 해시 검증, 검증되지 않은 패키지, 롤백과 고정(pin)
- [관리자용](Docs/maintainers.md): 패키지 인덱스 요구 사항

로그인은 브라우저를 사용합니다(로컬 루프백 주소의 PKCE). 토큰은 macOS 키체인 또는 Linux Secret Service(`secret-tool`)에 저장하고, 둘 다 없으면 paxon 설정 폴더의 `0600` 파일에 저장하며 그 사실을 알려 줍니다. 키와 토큰은 `--verbose`에서도 출력하지 않습니다.

-----

> [!CAUTION]
> 로직 업데이트로 이 저장소는 1.4.1 이하 버전에서 더 이상 동작하지 않습니다.
>
> `paxon update`로 최신 버전으로 업데이트하세요.
