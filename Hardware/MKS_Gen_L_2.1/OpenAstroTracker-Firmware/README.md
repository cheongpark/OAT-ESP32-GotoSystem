# OpenAstroTracker-Firmware
OpenAstroTracker의 공식 펌웨어입니다. 다른 중요한 리소스는 [여기](https://wiki.openastrotech.com/en/Links)에서 찾을 수 있습니다.

## 변경 로그
버전별 변경 사항에 대한 자세한 내용은 [Changelog](Changelog.md)를 참조하세요.

## 코딩 가이드라인

`.clang-format` 파일을 참조하세요. 코드가 포맷팅 가이드라인을 준수하는지 확인하기 위해 모든 PR에서 GitHub 액션이 실행됩니다.

### 로컬에서 clang-format 실행하기
* `clang-format` 버전 12를 설치하세요. _참고: 모든 배포판이 기본적으로 버전 12를 사용하지는 않습니다_
  * Windows: [LLVM 웹사이트](https://llvm.org/builds/)에서 설치 프로그램 이용 가능
  * Ubuntu: `sudo apt install clang-format-12`
  * ArchLinux: `sudo pacman -S clang`
* 포맷터 실행: 
  * VSCode 확장: [https://marketplace.visualstudio.com/items?itemName=xaver.clang-format](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)
  * 쉘: `bash -c 'shopt -s nullglob globstar;GLOBIGNORE=./src/libs/TimerInterrupt/*; for i in ./{.,src/**,unit_tests,boards/**}/*.{c,cpp,h,hpp}; do clang-format -i $i; done'`
  
## 기여

이것은 오픈 소스 프로젝트이며 누구나 기여할 수 있습니다. 풀 리퀘스트를 검토할 때 다음 규칙을 따릅니다:
- 풀 리퀘스트는 **특정 기능이나 버그 수정과 관련된 변경 사항만** 포함해야 합니다. 이 저장소에 병합되어야 할 여러 관련 없는 변경 사항이 있는 경우, 각각에 대해 별도의 풀 리퀘스트를 생성해야 합니다.
- 풀 리퀘스트는 **올바르게 빌드**되어야 합니다. 그렇지 않은 경우, 문제를 수정하고 소스 브랜치에 푸시하세요. matrix_build.py 스크립트를 사용하여 모든 중요한 구성을 로컬에서 빌드할 수 있습니다(CI와 유사하게 작동).
- 풀 리퀘스트는 모든 댓글이 **OAT 개발자**에 의해 해결된 **후**에만 병합될 수 있습니다. 놓친 문제로 이어질 수 있으므로 직접 댓글을 해결하지 마세요.
- 검토 후 작성자가 합리적으로 신속하게 풀 리퀘스트를 유지 관리하지 않는 경우, 누적된 병합 충돌과 원본 코드 변경으로 인해 대규모 작업이 필요할 수 있으므로 개발자는 병합 없이 닫기로 결정할 수 있습니다. 그런 다음 포크 브랜치에 필요한 모든 변경 사항을 적용한 후 풀 리퀘스트를 다시 생성할 수 있습니다.

## 개발

Arduino IDE가 지원되더라도, 개발을 위해 [PlatformIO](https://platformio.org/)가 있는 VSCode를 사용하는 것을 강력히 권장합니다. 자동 종속성 관리, 강력한 IDE, 디버깅, 자동 빌드 플래그 정의 등을 제공합니다.

### 디버깅

#### ATmega2560 기반

> :warning: **현재 디버깅은 mega2560 플랫폼에서만 지원됩니다!**

이 예제에서는 `ramps` 환경을 사용하지만, 파생된 환경도 사용할 수 있습니다.

> `platformio.ini`에서 `debug_port`를 설정해야 할 수도 있습니다. platformio는 포트를 자동 감지한다고 하지만 현재는 작동하지 않는 것 같습니다.

현재 펌웨어를 디버깅하는 gdb 쉘 시작:
```shell
pio run -e ramps -t clean  # 환경 정리
piodebuggdb -e ramps  # 디버그 세션 초기화
# 이렇게 하면 디버그 모드에서 펌웨어를 빌드한 다음 원격 gdb 세션을 초기화합니다.
# IDE에 platformio 통합이 있는 경우 시각적 디버그 기능이 있을 수 있습니다.
```

디버그 인터페이스로 `avr-stub`를 사용할 때는 2가지가 필요합니다:
1. 펌웨어에서 직렬 링크 0을 사용하지 않아야 합니다.
    - 따라서 디버그 빌드에서는 `Serial`을 사용하는 모든 외부 인터페이스가 비활성화됩니다(`Serial1`, `Serial2` 등은 괜찮음)
2. 인터럽트 벡터에 대한 독점 액세스
    - ISR 등록을 비활성화하기 위해 arduino 프레임워크(특히 `WInterrupts.c`)를 핫패치해야 합니다. 이 구현은 `pre_script_patch_debug.py`에 있으며, 자동으로 실행되어야 합니다.

> avr-stub가 RAM 모드에 있는 동안 펌웨어가 매우 느리게 실행되고 타이밍 관련 기능이 제대로 작동하지 않을 수 있습니다.

디버깅은 아직 약간 불안정하므로 안정적인 디버깅 세션을 얻기 위해 여러 번 시도해야 할 수도 있습니다.

자세한 정보는 [avr-stub 문서](https://github.com/jdolinay/avr_debug/tree/master/doc)에서 확인할 수 있습니다.

### Meade 명령 문서
Wiki의 Meade 명령 페이지는 다음을 실행하여 생성됩니다:
```shell
python .\scripts\MeadeCommandParser.py
```
메인 디렉토리에서 실행하세요. 페이지는 scripts 폴더에 생성되며 수동으로 Wiki에 복사하여 붙여넣어야 합니다. 다음과 같은 내용을 붙여넣어 버전을 수동으로 추가하세요(현재로서는):
```
> 이 문서는 펌웨어 **V1.13.9** 기준으로 최신입니다.
{.is-warning} 