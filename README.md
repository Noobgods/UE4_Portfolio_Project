# UE4_Portfolio_Project
> 포트폴리오 용으로 제작한 TPS 데모 게임 Source 입니다.
>
> - 엔진 : Unreal Engine 4.25.4
> - IDE : Visual Studio 2019 Version 16.9.4
> - 인원 구성 : 개인 개발

----------------------------------------
   
# 개발 영상
> 2021.05.11 업데이트
> [![2021.05.11 개발 영상](http://img.youtube.com/vi/S9OkDscNSng/0.jpg)](https://www.youtube.com/watch?v=S9OkDscNSng)
> 
> 2021.05.09 업데이트
> [![2021.05.09 개발 영상](http://img.youtube.com/vi/PDIeuZGZOV0/0.jpg)](https://www.youtube.com/watch?v=PDIeuZGZOV0)
> 
   
----------------------------------------
    
# 기술 정의서
> ## Character Movement
>
> * 달리기 ( Shift + W 로만 달리기 가능/ Shift -> W or W -> Shift 순서로 입력해도 달리기 가능)
> * 무기교체 ( 근접무기, 1번 무기, 2번 무기로 교체가능)
> * 재장전 (무기마다 재장전 모션 다르게 설정 가능)
> * 조준 ( 조준할때 이동, 발사시 에임이 벌어지는 Dynamic CrossHair 적용)
>
> - 행동 Montage는 BeginAction Notify-> EndAction Notify 실행으로 StateComponent를 조작함

----------------------------------------
   
> ## Components
>
> * Action Component
>   - 캐릭터의 행동을 관리
>   
> * Behavior Component 
>   - 적의 상태와 Behavior Tree 관리
>   
> * Inventory Component : ItemContainer
>   - 캐릭터의 아이템 관리 
>   
> * Montage Component
>   - 캐릭터의 Montage들을 정보를 관리하고 Play
>   
> * State Component
>   - 캐릭터의 상태 관리
>   
> * Status Component
>   - 캐릭터의 스테이터스 관리
>   
> * Option Component
>   - 기타 조작옵션 관리
   
----------------------------------------
   
> ## Actions
>
>> ### ActionData : DataAsset
>> + Action에 대한 정보를 관리하는 데이터 에셋
>> + 기능 : Action Object 생성과 Actor들을 스폰
>>   (저장된 Data에서 만들어진 Action을 ItemData로 보내는 함수를 만들면 무기들을 Item화 가능 -> 구현 예정)
> 
>> ### Action : UObject 
>> + Attachment (캐릭터에 Attach되는 무기 액터 정보, 콜리전 정보)
>>    - 델리게이트를 이용해 DoAction에 충돌 이벤트 전송
>> + DoAction (무기에 대한 행동 정보)
>>    -	근접 무기 : DataAsset에 콤보를 저장하여 실행 가능
>>    - 원거리 무기 : HitScan 계산방식 사용 / Ammo Widget, Aim 클래스가 있음 / 총기, 에임 관련 옵션 들이 있음
>> + Equipment (무기의 장착과 해제 정보)
>>    - 델리게이트를 이용해 Attachment에 Attach 이벤트 전송
   
----------------------------------------
   
> ## Items
> 
>> ### ItemData : DataAsset
>> + CItem에 대한 정보를 관리하는 데이터 에셋
>> + 기능 : 아이템 스폰 (캐릭터에 스폰 or 스포너에 스폰)
>
>> ### Item : UObject
>> + ActorData (보여지는 액터 정보, 인벤토리 아이콘 저장 가능)
>> + Attribute (아이템 정보를 가지고 있는 Struct)
>>    - Primary Key ID로 관리되는 정보들을 가지고 있음.
>   
>> ### ItemContainer : UInterface
>> + 아이템을 저장하는 오브젝트들에서 사용하는 Move, Add 함수 사용
   
----------------------------------------
   
> ## Actors
> 
>> ### ItemSpawner : AActor, ItemContainer
>> + 맵에 배치된 아이템을 가지고 있는 액터 클래스
>> + 기능 : ItemContainer로 아이템 전송, Location에 아이템 스폰
> 
>> ### EnemySpawner : AActor (구현 예정)
>> + 적을 스폰하는 기능을 가지고 있는 액터 클래스
>> + 기능 : 적 관리, 적 스폰

 * 추가할 내용
    - Enemy 구조, Action 플로우 차트
 * 개발할 내용(우선순위 순으로)
    - 인벤토리 UI
    - 장비 아이템화
    - 적 AI
    - 적 추가
    - 컨텐츠 추가
