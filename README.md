# SpaceStation

논문 구현 영상 : [https://www.youtube.com/watch?v=DU3mkiz4B9g](https://www.youtube.com/watch?v=DU3mkiz4B9g) 

게임 영상 : [https://www.youtube.com/watch?v=vEbmR1M-XIA](https://www.youtube.com/watch?v=vEbmR1M-XIA)

게임 파일 : Release에서 설치하실 수 있습니다.

## 주제
Real-time Voronoi Fracturing

## 참고한 논문
- Real-time fracturing in video games
- Real Time Mesh Fracturing Using 2D Voronoi Diagrams

## 과제 설계 배경 및 필요성
Real-Time Fracturing은 실시간으로 물체가 파괴되는 현상을 계산하고 시뮬레이션하는 기술이다. 게임 분야에서 기존에는 성능적인 문제로 인해 미리 파쇄 패턴을 정의해두고 시뮬레이션하는 Pre-Fracturing을 주로 사용했다. Pre-Fracturing은 파괴 시뮬레이션을 진행할 때, 한정된 상황밖에 연출이 불가능하다. 그러나 Real-Time Fracturing은 실시간으로 사용자와 인터렉티브하게 정보를 공유하며 보다 더 실제같은 파괴 시뮬레이션 경험을 사용자에게 제공할 수 있다. 또한, Real-Time Fracturing을 구현하고 적용시키기에 충분한 하드웨어와 소프트웨어들이 개발되고 있으며, 이에 대한 실험적인 적용 사례들도 조금씩 등장하고 있다. 따라서 이번 과제를 통해 Real-Time Fracturing을 구현하고 이에 대한 프로세스를 개선하여 더 발전시킬 수 있는 방법을 모색해보고자 한다.

## 과제 주요내용
   1) 타겟 논문에 대한 이해 : 타겟 논문에서 제안하는 Real-Time Fracturing 기술의 핵심 개념과 아이디어, 사용한 소프트웨어와 알고리즘을 상세히 분석한다.

   2) 프로세스 구현 : 타겟 논문을 분석한 내용을 바탕으로 실시간 파괴를 위한 코드를 작성한다. 이때, 프로세스는 모델링, 알고리즘 구현, 시각화 등을 모두 포함하는 프로토타입을 구현한다.

   3) 성능 최적화 : 구현된 Real-Time Fracturing의 성능을 프로파일링하고, 오류 및 개선점을 파악한다.
 
   4) 응용 : 완성된 Real-Time Fracturing을 응용하여 게임을 제작하고 활용에 대한 방안을 고려한다.

## 최종결과물의 목표
   1) 실시간으로 물체가 파쇄되는 패턴을 계산하고 이를 시뮬레이션에 적용한다.
   2) 프로세스가 실행되는 동안의 기기의 자원 사용량을 분석하고 목표 프레임에 도달되는지 확인한다.
   3) Real-Time Fracturing을 적용한 게임을 제작하고 시연한다.

## 결론 및 제언
보로노이를 사용한 실시간 파쇄는 물리적으로 타당한 결과를 만들지는 못한다. 그러나 영향 지점을 고려하고 보로노이 파쇄에 사용할 점 개수와 그것들의 위치를 조절해서 실제 물체가 부서지는 모습을 흉내낼 수는 있다. 이는 매번 다른 파쇄 결과를 생성할 수 있으며, 총알 충격 위치와 같이 결과에 영향을 미치는 영향 지점을 고려할 수 있다. 이는 현실적이고 파괴 가능한 환경을 만들고 게임 플레이를 신선하고 역동적으로 만들 수 있다.

이를 통해 사전 파쇄보다 현실적인 시각적 결과를 얻을 수 있지만 계산을 수행하는 데 걸리는 시간으로 인해 사전 파쇄보다 오래 걸린다. 비디오 게임에서 성능을 측정하는 가장 중요한 통계 중 하나는 프레임 레이트 또는 FPS와 연결된다. FPS 값은 1초 동안 화면에 렌더링되는 프레임 수와 동일하다. FPS가 높을수록 원활한 게임이 실행되는 것으로 나타난다. 이 수치가 낮으면 게임은 눈에 띄게 끊기면서 플레이가 불가능해질 수 있다. 대부분의 비디오 게임의 목표 프레임 레이트는 보통 초당 30프레임으로 설정된다. 우리가 구현한 실시간 파쇄에 대해서는, 단일 파쇄은 프레임에 부정적인 영향을 거의 미치지 않을 정도이고, 연속되는 파쇄에도 초당 30프레임 밑으로 떨어지지 않을 정도로 게임에 적용하기에 충분한 속도이다.

우리가 구현한 기술은 2D 보로노이 패턴을 계산하여 3D 메시에 적용하는 것이기에 이 기술을 사용하여 보다 사실적으로 파괴할 수 있는 메시의 종류에 대한 제한이 있다. 이는 건물이나 가구와 같은 메시에 대해서는 실감나는 파쇄를 제공하지 않는다. 어떤 물체가 적합한지 고려해야 한다. 적절한 파괴 가능한 물체의 예로는 벽과 바닥 등이 있을 것이다.

다른 파쇄 기법과 비교하여 보로노이 파쇄를 사용하는 것은 유리 소재를 나타내는 게임 객체에 대해서 파쇄 패턴을 충격 위치를 기반으로 생성하여 추가적인 사실감을 얻을 수 있는 장점이 있다. 유리 소재의 사전 파쇄와 충격 위치 기반 실시간 보로노이 파쇄를 비교하는 구현이 성공적으로 만들어졌으며, 보로노이 사이트의 수와 사이트의 위치 분포 계산 방법을 소스 코드 내에서 정의할 수 있다.

이후에, 3D 메시에 적용할 수 있는 다양한 파쇄 기법들에 대해 공부하고 구현해서 더욱 현실적이고 파괴 가능한 환경을 가진 게임을 만들고자 한다.

<hr/>

## 적용해서 만든 게임
총알 충돌 지점 근처 점들을 사용한 Real-time Voronoi Fracturing 기술을 사용하여 만든 게임 Space Station입니다.

## 게임의 장르 
FPS

## 게임의 배경 
Space Station에서 플레이어는 밀폐된 통로들에서 나아갈 수 있는 방향으로만 자동적으로 움직인다. 밀폐된 통로는 플레이어의 이동방향에서 상, 하, 좌, 우, 전진(혹은 후진) 방향으로 랜덤 생성되며, 통로 안에는 플레이어를 Game Over시킬 수 있는 적색, 녹색, 청색의 투명한 유리 개체가 랜덤한 크기와 간격으로 생성된다. 좌측 상단엔 현재 점수, 현재 스테이지, 사용 가능한 총알 수 UI가 있으며, 우측 하단엔 게이지 형태의 UI가 있다.

## 플레이어의 조작 
플레이어는 마우스를 움직여 시야를 조정할 수 있고, 왼쪽 마우스 클릭을 통해 총알을 발사한다.

## 게임의 규칙 
플레이어가 유리 개체에 부딪히면 게임 오버 된다. 게임 오버 되지 않으려면 총알을 발사해서 유리를 깨트려야 한다. 총알의 색과 유리의 색이 같아야 유리가 깨지며, 유리를 깨트릴 때마다 점수가 1점씩 오른다. 총알의 색은 디스플레이에 우측 하단에 위치한 게이지를 가리키는 화살표로 확인할 수 있다. UI 게이지에서 화살표가 가리키는 색상이 발사할 때의 총알의 색상이 된다. 화살표는 게이지 내에서 좌우로 움직이고 플레이어의 생존 시간이 지날수록 속도가 빨라진다. 플레이어의 이동속도도 점점 빨라진다. 오래 생존하면서 최대한 많은 유리를 깨트려야 높은 점수를 얻을 수 있다.
