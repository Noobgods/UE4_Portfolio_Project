# UE4_Portfolio_Project
포트폴리오 용으로 제작한 TPS 데모 게임 Source 입니다.

- 엔진 : Unreal Engine 4.25.4
- IDE : Visual Studio 2019 Version 16.9.4
- 인원 구성 : 개인 개발
- 개발 기간 : 20210426 ~ (진행 중)

# 개발 영상
> 2021.05.07 업데이트
>
> [![2021.05.07 개발 영상](http://img.youtube.com/vi/NnhALZ2ijoc/0.jpg)](https://youtu.be/NnhALZ2ijoc?t=0s)
>
   
    
# 기술 정의서
> ## Character Movement
>
> * 달리기 ( Shift + W 로만 달리기 가능/ Shift -> W or W -> Shift 순서로 입력해도 달리기 가능)
> * 무기교체 ( 근접무기, 1번 무기, 2번 무기로 교체가능)
> * 재장전 (무기마다 재장전 모션 다르게 설정 가능)
> * 조준 ( 조준할때 이동, 발사시 에임이 벌어지는 Dynamic CrossHair 적용)
>
> - 행동 Montage는 BeginAction Notify-> EndAction Notify 실행으로 StateComponent를 조작함


> ## Components
>
> * Action Component
> * Behavior Component
> * Inventory Component : ItemContainer
> * Montage Component
> * Option Component
> * State Component
> * Status Component
   
   
> ## Actions
>
> ### ActionData : DataAsset
> + Action에 대한 정보를 관리하는 데이터 에셋
> + 기능 : Action Object 생성과 Actor들을 스폰
>   (저장된 Data에서 만들어진 Action을 ItemData로 보내는 함수를 만들면 무기들을 Item화 가능)
> 
> ### Action : UObject 
> + Attachment (캐릭터에 Attach되는 무기 액터 정보, 콜리전 정보)
> - 델리게이트를 이용해 DoAction에 충돌 이벤트 전송
> + DoAction (무기에 대한 행동 정보)
> -	근접 무기 : DataAsset에 콤보를 저장하여 실행 가능
> - 원거리 무기 : HitScan 계산방식 사용 / Ammo Widget, Aim 클래스가 있음 / 총기, 에임 관련 옵션 들이 있음
> + Equipment (무기의 장착과 해제 정보)
> - 델리게이트를 이용해 Attachment에 Attach 이벤트 전송
   
   
> ## Items
> 
> ### ItemData : DataAsset
> + 위의 CItem에 대한 정보를 관리하는 데이터 에셋
> + 기능 : 아이템 스폰 (캐릭터에 스폰 or 스포너에 스폰)
>
> ### Item : UObject
> + ActorData (보여지는 액터 정보, 인벤토리 아이콘 저장 가능)
> + Attribute (아이템 정보를 가지고 있는 Struct)
>   Primary Key ID로 관리되는 정보들을 가지고 있음.
>   
> ### ItemContainer : UInterface
> + 아이템을 저장하는 오브젝트들에서 사용하는 Move, Add 함수 사용
   
   
> ## Actors
> 
> ### ItemSpawner : AActor, ItemContainer
> + 맵에 배치된 아이템을 가지고 있는 액터 클래스
> + 기능 : ItemContainer로 아이템 전송, Location에 아이템 스폰
