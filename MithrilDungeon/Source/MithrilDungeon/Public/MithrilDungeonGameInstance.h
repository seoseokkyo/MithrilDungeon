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

	FORCEINLINE void Set(int _index ,const FOnlineSessionSearchResult& item) // �����Ͼ���ʰ�
	{
		index = _index;
		/*item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
		item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);*/
		// ������ �̸�
		userName = item.Session.OwningUserName;
		// �ִ� �÷��̾� ��
		maxPlayerCount = item.Session.SessionSettings.NumPublicConnections;
		// ���� �濡 ���� �÷��̾� ��
		currentPlayerCount = maxPlayerCount - item.Session.NumOpenPublicConnections;
		pingMs = item.PingInMs; // �и�������
	};

	FORCEINLINE FString ToString() 
	{
		return FString::Printf(TEXT("%s, %s, %s, (%d/&d), &dms"), *roomName, *hostName, *userName, currentPlayerCount, maxPlayerCount, pingMs);
	}
};

/**
 * 
 */

 // ��ã�� ��û �� ������ ���� �� ȣ��� ��������Ʈ
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

	// ���� �������̽��� �����ʹ�.
	IOnlineSessionPtr sessionInterface;

	FSessionSearchDelegate OnMySessionSearchCompleteDelegate;

	// �� ����  ��û ���
	void CreateMySession(FString roomName, int32 playerCount); // ���̸�, �ִ���

	// �� ���� ����.

	void OnCreateSessionComplete(FName sessionName, bool bWasSuccessful);

	FString mysessionName = TEXT("JW");

	// ���� ã�� �ʹ�.
	TSharedPtr<FOnlineSessionSearch> sessionSearch; // Ptr�� ����

	void FindOtherSessions();


	void OnFindSessionsComplete(bool bWasSuccessful);

	// �濡 �����ϰ� �ʹ�.
	void JoinMySession(int32 index);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// �������� �ѱ۾������� �Լ�
	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
};

