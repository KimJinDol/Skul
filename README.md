# 스컬: 더 히어로 슬레이어 모작 (DirectX 11)

2D 플랫포머 액션 게임 *스컬: 더 히어로 슬레이어*를 DirectX 11로 따라 만든 1인 프로젝트입니다.

- 시연 영상: https://youtu.be/s39pqdr1OYc
- 개발 환경: C++, DirectX 11, HLSL, Dear ImGui, Visual Studio 2019, FMOD
- 개발 인원: 1명

## 기반 코드에 대해

학원 수업에서 제공받은 **DirectX 11 엔진 프레임워크**(`GameEngine`) 위에 게임을 만들었습니다.
렌더링, 리소스·씬 관리, 컴포넌트 구조, 기본 충돌체 같은 틀은 수업 코드이고,
아래 **"직접 구현한 것"**이 제가 만든 부분입니다.

## 직접 구현한 것

| 기능 | 내용 | 볼 파일 |
|---|---|---|
| 충돌 처리 | 씬의 충돌체를 모아 순회하며 타입별로 충돌을 검사하고, 막힘은 충돌 지점을 계산해 위치를 되돌림 | `GameEngine/Include/Collision.cpp`, `Scene/SceneCollision.cpp` |
| 탄력·마찰 | 땅에 닿을 때의 Y 속도에서 중력만큼 빼 튕기고, 땅에 닿아 있는 시간과 마찰 계수로 감속해 멈춤. 원형 오브젝트는 굴러간 거리만큼 이미지 회전 | `Component/Transform.h/.cpp`, `Object/Include/Object/SkulDefaultHead.cpp` |
| 배경 이어 붙이기 | 배경 전용 샘플러에 Wrap·Clamp를 걸고 카메라 이동에 맞춰 UV를 옮겨 배경이 끊기지 않게 함 | `BackgroundImage.cpp`, `Resource/BackgroundShader.*`, `Bin/Shader/Standard2D.fx` |
| 배경 원근감 | 배경 이미지마다 카메라 영향도를 두어 먼 배경일수록 덜 움직이게 함 | `BackgroundImage.cpp` |
| 쿨타임 셰이더 | `atan2`로 중심에서 현재 UV의 각도를 구해 (남은 비율 × 360°)와 비교해 픽셀 색을 정함 | `Bin/Shader/UI.fx` (`UICoolTimePS`), `UI/CoolTime.*` |
| IMGUI 맵툴 확장 | 맵 이미지를 불러와 타일 선택 창을 만들고, 충돌체 모드를 추가해 맵에 충돌체를 배치·저장 | `GameEditor/Include/TileMapWindow.cpp` |
| 플레이어 (스컬) | 이동, 공격, 머리 던지기 | `Object/Include/Object/Skul.*`, `SkulDefaultHead.*` |
| 보스·몬스터 AI | 보스 쿠마(대지 강타, 점프 찍기, 방패, 궁극기)와 일반 몬스터의 상태 선택·공격 | `Object/Include/Object/KumaAI.*`, `AITask_*`, `AISelect*` |

## 프로젝트 구성

```
Skul/
  GameEngine.sln
  Copy.bat        GameEngine 빌드 결과(헤더, lib, 셰이더)를 다른 프로젝트로 복사
  GameEngine/     엔진 (수업 프레임워크 + 직접 추가한 기능)
  Object/         게임 코드: 스컬, 보스, 몬스터, UI
  Client2D/       실행 파일
  GameEditor/     맵·오브젝트 에디터 (MFC + ImGui)
```

## 저장소에 없는 것

- **그림·사운드 리소스:** 원작 게임의 저작물이라 올리지 않았습니다. 이 저장소만으로는 실행되지 않으니 동작은 영상으로 확인해 주세요.
- **외부 라이브러리:** FMOD, Dear ImGui, DirectXTex는 올리지 않았습니다.
- **빌드 결과물과 `Engine/` 폴더:** `GameEngine`을 빌드한 뒤 `Copy.bat`을 실행하면 다시 만들어집니다.
