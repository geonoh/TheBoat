# Early Return 스타일 정리

날짜: 2026-03-18
위치: TheBoat

## 작업 목적
- 최근 로그인/로비/게임 인스턴스 관련 작업으로 생긴 중첩 `if` 구조를 정리한다.
- 널 체크가 필요한 곳은 가드 절과 `early return` 중심으로 통일한다.
- 같은 스타일 선호를 프로젝트 바깥 공용 작업 규칙 파일 `AGENTS.md`에도 남긴다.
- `.editorconfig` 기준에 맞춰 앞으로도 `CRLF` 저장 규칙을 유지할 수 있게 작업 흐름을 맞춘다.

## 변경 전 상태
- [Source/TheBoat/Private/LobbyWidget.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\LobbyWidget.cpp) 의 `NativeConstruct`, `NativeDestruct`, `Set`, `OnStartGameButtonClicked` 에서 `if` 안에 `if`가 연속으로 들어가 있었다.
- [Source/TheBoat/Private/LoginWidget.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\LoginWidget.cpp) 도 월드, 게임 인스턴스, 델리게이트 해제, 위젯 바인딩 처리에서 중첩 분기가 많았다.
- [Source/TheBoat/Private/BoatGameInstance.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\BoatGameInstance.cpp) 는 소켓 생성/접속/에러 패킷 처리에서 예외 흐름이 위에서 빨리 끝나지 않아 읽기 흐름이 길었다.
- [Source/TheBoat/FPacketReceiveRunnable.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\FPacketReceiveRunnable.cpp), [Source/TheBoat/TP_WeaponComponent.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\TP_WeaponComponent.cpp), HUD 계열 코드도 같은 스타일 혼재가 있었다.
- 공용 규칙 파일 [AGENTS.md](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\AGENTS.md) 에는 `LargeCamelCase` 등은 있었지만, 중첩 `if` 지양과 `early return` 선호는 명시되어 있지 않았다.

## 이번 작업에서 한 판단
- 널 체크, 월드 조회 실패, 게임 인스턴스 조회 실패처럼 함수 본문을 진행할 수 없는 조건은 바로 반환하는 방식으로 바꿨다.
- Unreal 수명주기 함수에서는 `Super` 호출 순서를 깨지 않도록 유지하면서도, 가능한 범위에서 중첩을 제거했다.
- `LoginWidget` 과 `LobbyWidget` 은 UI 바인딩 실패가 치명적일 수 있지만, 요청하신 스타일 우선으로 가드 절을 먼저 두고 흐름을 평탄화했다.
- `BoatGameInstance::ConnectToServer` 는 소켓 생성 실패, 소켓 서브시스템 없음, 호스트 파싱 실패, 실제 접속 실패를 각각 빠르게 반환하게 정리했다.
- `TP_WeaponComponent::Fire` 는 플레이어 컨트롤러, 카메라 매니저, 월드가 없으면 바로 종료하고, 실제 발사 흐름만 아래로 남기도록 정리했다.
- `CombatManager` 는 `GetWorld()` 와 `GetFirstPlayerController()` 를 먼저 확인하도록 바꾸고, 새 포인터 사용에 맞춰 `PlayerController` include 를 추가했다.
- 수정한 C++ 파일은 마지막에 다시 `CRLF + final newline` 으로 정규화했다.

## 변경 파일
- [Source/TheBoat/Private/LobbyWidget.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\LobbyWidget.cpp)
- [Source/TheBoat/Private/LoginWidget.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\LoginWidget.cpp)
- [Source/TheBoat/Private/BoatGameInstance.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\BoatGameInstance.cpp)
- [Source/TheBoat/FPacketReceiveRunnable.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\FPacketReceiveRunnable.cpp)
- [Source/TheBoat/TP_WeaponComponent.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\TP_WeaponComponent.cpp)
- [Source/TheBoat/CombatCharacter.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\CombatCharacter.cpp)
- [Source/TheBoat/CombatPlayerController.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\CombatPlayerController.cpp)
- [Source/TheBoat/Private/CombatManager.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\CombatManager.cpp)
- [Source/TheBoat/Private/LobbyHUD.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\LobbyHUD.cpp)
- [Source/TheBoat/Private/CombatHUD.cpp](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\TheBoat\Source\TheBoat\Private\CombatHUD.cpp)
- [AGENTS.md](C:\Users\ohhh1\OneDrive\Desktop\Gunny\Programming\AGENTS.md)

## 일부러 아직 하지 않은 것
- 전체 프로젝트 빌드는 아직 수행하지 않았다.
- `Source/TheBoat` 바깥 영역이나 이번 요청과 직접 무관한 파일의 분기 스타일은 전면 정리하지 않았다.
- `check(false)` 를 `ensure` 로 바꾸거나, UI 바인딩 실패 정책 자체를 바꾸는 구조적 리팩터링은 하지 않았다.

## 검증 상태
- `git diff --check` 기준으로 이번에 수정한 C++ 파일들에 공백/개행 문제는 없는 상태로 확인했다.
- `rg` 패턴 검색으로 `if` 블록 안에 바로 또 `if` 가 중첩되는 전형적인 형태는 `Source/TheBoat` 에서 남지 않도록 확인했다.
- 실제 Unreal 빌드와 런타임 검증은 아직 하지 않았다. 따라서 include 누락이나 위젯 바인딩 타이밍 관련 문제는 빌드 전까지는 확정할 수 없다.

## 다음 작업 제안
1. UE 빌드를 한 번 돌려서 이번 스타일 정리 중 생긴 컴파일 이슈가 없는지 확인한다.
2. 로그인/로비 플로우를 실제로 실행해서 `Passthrough`, `LoginSucceeded`, `MatchCreated` 경로가 기존과 동일하게 동작하는지 본다.
3. 앞으로 새로 작성하는 C++ 코드도 `AGENTS.md` 에 추가한 가드 절 규칙을 기준으로 작성한다.
