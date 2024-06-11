// Fill out your copyright notice in the Description page of Project Settings.


#include "MithrilDungeonGameInstance.h"
#include "StateComponent.h"
#include "ParentItem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include <../../../../../../../Source/Runtime/Core/Public/Templates/SharedPointer.h>
#include <../../../../../../../Plugins/Online/OnlineBase/Source/Public/Online/OnlineSessionNames.h>

FCharacterStat UMithrilDungeonGameInstance::GetCharacterDataTable(const FString& rowName)
{
	if (dt_characerStatDataTable != nullptr)
	{
		FString errorText;
		FCharacterStat* characerStatInfo = dt_characerStatDataTable->FindRow<FCharacterStat>(FName(rowName), errorText);

		if (characerStatInfo != nullptr)
		{
			return *characerStatInfo;
		}
		else
		{
			return FCharacterStat();
		}
	}

	return FCharacterStat();
}

FParentItemInfo UMithrilDungeonGameInstance::GetParentItemDataTable(const FString& rowName)
{
	if (dt_parentItemDataTable != nullptr)
	{
		FString errorText;
		FParentItemInfo* parentItemInfo = dt_parentItemDataTable->FindRow<FParentItemInfo>(FName(rowName), errorText);

		if (parentItemInfo != nullptr)
		{
			return *parentItemInfo;
		}
		else
		{
			return FParentItemInfo();
		}
	}

	return FParentItemInfo();
}

void UMithrilDungeonGameInstance::Init()
{
	Super::Init();

	// ����ý��ۿ��� �����������̽��� ��������ʹ�.
	auto subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface(); // ����ý������� �����ؼ� ����.

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMithrilDungeonGameInstance::OnCreateSessionComplete);

		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMithrilDungeonGameInstance::OnFindSessionsComplete);

		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMithrilDungeonGameInstance::OnJoinSessionComplete);
	}

	//FTimerHandle h;
	//GetWorld()->GetTimerManager().SetTimer(h, [&](){
	//	FindOtherSessions();//(TEXT("MyRoom"), 20);


	//	},2, false);
}

void UMithrilDungeonGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	FOnlineSessionSettings set;

	// 1. ��������Ʈ ���� �����ΰ�?
	set.bIsDedicated = false;

	// 2. �������� ��Ī�Ұ��ΰ�?
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";

	// 3. ��Ī�� �����Ұ��ΰ�? (<-> ģ���ʴ�θ� �Ұ��ΰ�?)
	set.bShouldAdvertise = true;

	// 4. Presence�� ��� �� ���ΰ�?
	set.bUsesPresence = true;
	// 5. ���ӵ��߿� �������ɿ���
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;
	// 6. ������ ���� ������ �ִ밹��
	set.NumPublicConnections = playerCount;
	// 7. Ŀ���� ���� : ���̸�, ȣ��Ʈ�̸�
	/*FString roomName_enc = StringBase64Encode(roomName);
	FString mysessionName_enc = StringBase64Encode(mysessionName);*/

	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mysessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	// 8. netID�� ��������ʹ�.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId(); // ���÷��̾��� ������ȣ, �ĺ��ڵ尰����.

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession : %s"), *roomName);

	// ���� ����� �޶�� ��û�ϰ�ʹ�.
	sessionInterface->CreateSession(*netID, FName(*roomName), set);

}

void UMithrilDungeonGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete -> sessionName : %s, bWasSuccessful : %d"), *sessionName.ToString(), bWasSuccessful);

	// ���� ������� �����ߴٸ� ServerTravel �ϰ�ʹ�.
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/MedievalDungeon/Maps/Demonstration?listen")); // ����Ʈ������ �����̶� ������ �̸��̹ٲ�, ������ �ڿ��ٰ� ?listen�� �����ָ� ��.
	}

}

void UMithrilDungeonGameInstance::FindOtherSessions()
{
	// �����������̽��� �̿��ؼ� ���� ã��ʹ�.
	sessionSearch = MakeShareable(new FOnlineSessionSearch);

	// �˻� ������ �����ϰ� �ʹ�.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// LAN ����
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL"; // ���̵Ǹ� Ʈ��, �ƴϸ� �޽�, ����ã���� �ƴ� ���������� ã����

	// �ִ� �˻� ��
	sessionSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UMithrilDungeonGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// ���� �����ߴٸ�
	if (bWasSuccessful)
	{
		// sessionSearch���� ������ ��������ʹ�. -> UI�� ǥ���ϰ�ʹ�.
		auto results = sessionSearch->SearchResults;
		for (int i = 0; i < results.Num(); i++)
		{
			auto item = sessionSearch->SearchResults[i];
			if (item.IsValid() == false) // ��û�ϴ� ������ �����ִµ� ���� �������� ���� ���� �� ����. �׷��� �ѹ� ����
			{
				continue;
			}
			FSessionInfo info;
			info.Set(i, item);

			FString roomName;
			FString hostName;
			item.Session.SessionSettings.Get(FName("ROOM_NAME"), roomName);
			item.Session.SessionSettings.Get(FName("HOST_NAME"), hostName);

			info.roomName = StringBase64Decode(roomName);
			info.hostName = StringBase64Decode(roomName);

			OnMySessionSearchCompleteDelegate.Broadcast(info);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *info.ToString())
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("play"))
	}
}

void UMithrilDungeonGameInstance::JoinMySession(int32 index)
{
	sessionInterface->JoinSession(0, FName(*mysessionName), sessionSearch->SearchResults[index]);

}

void UMithrilDungeonGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	// ȣ��Ʈ�� IP, Port��ȣ�� �����ͼ� ClientTravel �ϰ�ʹ�.
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("ClinetTravel URL : %s"), *url);

		if (url.IsEmpty() == false)
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute); // �ۼ��� �κ������� ������, �ۼַ�Ʈ�� �������� �������� ����.
		}
	}

}

FString UMithrilDungeonGameInstance::StringBase64Encode(const FString& str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 �� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UMithrilDungeonGameInstance::StringBase64Decode(const FString& str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
