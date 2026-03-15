# 클라이언트 서버 통신 흐름 구현

날짜: 2026-03-15
위치: `TheBoat/AiHistory/2026-03-15-client-server-flow.md`
관련 커밋: `170d721` (`클라이언트 서버 통신 흐름 구현`)

## 작업 목적
- 기존에 만들어 둔 텍스트 기반 TCP 서버 프로토콜에 맞춰 TheBoat 클라이언트 쪽 접속, 로그인, 매치메이킹 진입 흐름을 실제로 연결한다.
- 로그인 맵 -> 로비 맵 -> 전투 맵으로 이어지는 최소 플레이 루프를 서버 응답 기반으로 닫는다.
- 이후 전투 스냅샷 동기화 확장을 할 수 있도록 수신 스레드와 게임 스레드 사이 메시지 처리 구조를 만든다.

## 변경 전 상태
- `UBoatGameInstance`는 소켓 생성과 수신 스레드 시작만 하고 있었고, 실제 서버 `Connect`, 송신 API, 수신 패킷 파싱이 없었다.
- `FPacketReceiveRunnable`은 바이트를 읽기만 하고 수신 데이터 후처리를 하지 않았다.
- `ULoginWidget`은 버튼 클릭 시 바로 `Lobby` 맵으로 이동했고, `ULobbyWidget`은 버튼 클릭 시 바로 `FirstPersonMap`으로 이동했다.
- 전투 중 이동/발사 입력은 로컬에서만 처리되고 서버로 전송되지 않았다.

## 이번 작업에서 한 판단
- 통신 소유자는 `UGameInstance`가 가장 자연스럽다고 판단했다.
  - 로그인 맵, 로비 맵, 전투 맵을 넘나들어도 살아 있어야 하고, 위젯보다 수명이 길다.
- 서버 프로토콜이 줄바꿈 기반 텍스트 명령이므로 `FPacketReceiveRunnable`에서 줄 단위로 패킷을 끊고, 게임 스레드에서는 큐를 폴링해 해석하도록 분리했다.
  - 수신 스레드에서 직접 게임 로직을 건드리지 않도록 하여 UE 오브젝트 접근 위험을 줄였다.
- 로그인 성공과 매치 생성은 델리게이트 이벤트로 뽑았다.
  - 위젯은 상태만 구독하고, 통신 로직은 `UBoatGameInstance`에 남겨서 UI 결합을 낮췄다.
- 이동과 발사는 우선 최소한으로만 붙였다.
  - 로컬 이동/발사 발생 시 `MOVE`, `FIRE`를 서버로 보내지만, 서버의 `SNAPSHOT`/`PLAYER_MOVED`를 월드 액터에 완전 반영하는 단계는 이번 범위에서 제외했다.

## 변경 파일
- `Config/DefaultGame.ini`
  - 기본 서버 주소와 포트를 `127.0.0.1:5000`으로 추가했다.
- `Source/TheBoat/Public/BoatGameInstance.h`
- `Source/TheBoat/Private/BoatGameInstance.cpp`
  - 소켓 연결/해제, `HELLO`/`QUEUE`/`MOVE`/`FIRE` 송신, 수신 큐 폴링, 패킷 파싱, 상태 메시지/로그인 성공/매치 생성 델리게이트를 구현했다.
- `Source/TheBoat/FPacketReceiveRunnable.h`
- `Source/TheBoat/FPacketReceiveRunnable.cpp`
  - 수신 버퍼를 줄 단위 텍스트 패킷으로 분리해 MPSC 큐에 넣도록 수정했다.
- `Source/TheBoat/Public/LoginWidget.h`
- `Source/TheBoat/Private/LoginWidget.cpp`
  - 로그인 버튼에서 서버 접속과 `HELLO`를 보내고, 상태 메시지를 화면에 표시하며 `HELLO_ACK` 수신 시 `Lobby`로 이동하도록 바꿨다.
- `Source/TheBoat/Public/LobbyWidget.h`
- `Source/TheBoat/Private/LobbyWidget.cpp`
  - 시작 버튼에서 `QUEUE`를 보내고, `MATCH_CREATED` 수신 시 `FirstPersonMap`으로 이동하도록 바꿨다.
- `Source/TheBoat/CombatCharacter.h`
- `Source/TheBoat/CombatCharacter.cpp`
  - 로컬 플레이어 캐릭터가 일정 주기로 위치 변화를 감지해 `MOVE`를 서버로 보내도록 추가했다.
- `Source/TheBoat/TP_WeaponComponent.cpp`
  - 로컬 발사 시 `FIRE`를 서버로 같이 보내도록 추가했다.

## 일부러 아직 하지 않은 것
- 서버의 `SNAPSHOT`, `PLAYER_MOVED`, `PLAYER_FIRED`를 실제 전투 월드 액터 상태에 반영하는 동기화 로직은 아직 구현하지 않았다.
  - 현재는 상태 메시지로만 받고 있으며, 전투 매니저 구조를 더 명확히 정리한 뒤 붙이는 편이 안전하다.
- 매치 생성 후 서버가 내려주는 실제 전투 진입 데이터(`Spawner`, `Character`, 팀 배치 등) 연동은 아직 안 했다.
  - `CombatManager::OnEnterCombatWorld()`는 여전히 임시 데이터 기반이다.
- 로그인 이름 입력 UI, 큐 취소, 재접속, 소켓 예외 복구 같은 사용자 경험 보강은 이번 범위에서 제외했다.

## 검증 상태
- `UE_5.7` 환경에서 아래 빌드를 실행해 성공했다.
  - `Build.bat TheBoatEditor Win64 Development -Project='C:\\Users\\ohhh1\\OneDrive\\Desktop\\Gunny\\Programming\\TheBoat\\TheBoat.uproject' -WaitMutex`
- 빌드 중 확인한 이슈
  - `Queue` 헤더 경로와 `FTSTicker::FDelegateHandle` 타입 차이로 한 차례 컴파일 오류가 있었고 수정 후 재빌드하여 통과했다.
- 런타임 플레이 테스트는 이번 턴에서 직접 수행하지 않았다.
  - 에디터 빌드 성공까지만 검증했다.

## 다음 작업 제안
1. `SNAPSHOT`과 `PLAYER_MOVED`를 `UCombatManager`에서 해석해 원격 캐릭터 위치를 갱신하는 실제 동기화 로직을 붙인다.
2. 서버가 `MATCH_CREATED` 이후 전투 진입용 초기 데이터 패킷을 내려주게 하고, `CombatManager::OnEnterCombatWorld()`의 임시 데이터를 제거한다.
3. 로그인 위젯에 플레이어 이름 입력 필드를 추가하고, 서버 오류 메시지를 더 명확히 노출한다.
4. 소켓 끊김, 매치 이탈, 중복 큐 진입 등 예외 케이스를 상태 머신 형태로 정리한다.
