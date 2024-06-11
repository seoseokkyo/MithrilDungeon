// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Interfaces/OnlineSessionInterface.h"
#include <../../../../../../../Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h>
#include "MithrilDungeonGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString roomName;
	UPROPERTY(BlueprintReadOnly)
	FString hostName;
	UPROPERTY(BlueprintReadOnly)
	FString userName;
	UPROPERTY(BlueprintReadOnly)
	int32 maxPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 currentPlayerCount;
	UPROPERTY(BlueprintReadOnly)
	int32 pingMs;

	int32 index;

	FORCEINLINE void Set(int _index ,const FOnlineSessionSearchResult& item) // 복사일어나지않게
	{
		index = _index;
		/*item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);*/
		// 방장의 이름
		userName = item.Session.OwningUserName;
		// 최대 플레이어 수
		maxPlayerCount = item.Session.SessionSettings.NumPublicConnections;
		// 현재 방에 들어온 플레이어 수
		currentPlayerCount = maxPlayerCount - item.Session.NumOpenPublicConnections;
		pingMs = item.PingInMs; // 밀리세컨드
	};

	FORCEINLINE FString ToString() 
	{
		return FString::Printf(TEXT("%s, %s, %s, (%d/&d), &dms"), *roomName, *hostName, *userName, currentPlayerCount, maxPlayerCount, pingMs);
	}
};

/**
 * 
 */

 // 방찾기 요청 후 응답이 왔을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSessionSearchDelegate, const FSessionInfo&, info);

 class UDataTable;

UCLASS()
class MITHRILDUNGEON_API UMithrilDungeonGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	FCharacterStat GetCharacterDataTable(const FString& rowName);

	UFUNCTION(BlueprintCallable)
	FParentItemInfo GetParentItemDataTable(const FString& rowName);

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_characerStatDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings")
	UDataTable* dt_parentItemDataTable;

public:
	virtual void Init() override;

	// 세션 인터페이스를 만들고싶다.
	IOnlineSessionPtr sessionInterface;

	FSessionSearchDelegate OnMySessionSearchCompleteDelegate;

	// 방 생성  요청 기능
	void CreateMySession(FString roomName, int32 playerCount); // 방이름, 최대명수

	// 방 생성 응답.

	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	FString mysessionName = TEXT("JW");

	// 방을 찾고 싶다.
	TSharedPtr<FOnlineSessionSearch> sessionSearch; // Ptr로 만듬

	void FindOtherSessions();


	void OnFindSessionsComplete(bool bWasSuccessful);

	// 방에 조인하고 싶다.
	void JoinMySession(int32 index);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// 스팀에서 한글쓰기위한 함수
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
};

