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

	// 서브시스템에서 세션인터페이스를 가져오고싶다.
	auto subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface(); // 서브시스템한테 의존해서 진행.

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

	// 1. 데디케이트 서버 접속인가?
	set.bIsDedicated = false;

	// 2. 랜선으로 매칭할것인가?
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	set.bIsLANMatch = subsysName == "NULL";

	// 3. 매칭을 공개할것인가? (<-> 친구초대로만 할것인가?)
	set.bShouldAdvertise = true;

	// 4. Presence를 사용 할 것인가?
	set.bUsesPresence = true;
	// 5. 게임도중에 참여가능여부
	set.bAllowJoinInProgress = true;
	set.bAllowJoinViaPresence = true;
	// 6. 참여할 공개 연결의 최대갯수
	set.NumPublicConnections = playerCount;
	// 7. 커스텀 정보 : 방이름, 호스트이름
	/*FString roomName_enc = StringBase64Encode(roomName);
	FString mysessionName_enc = StringBase64Encode(mysessionName);*/

	set.Set(FName("ROOM_NAME"), StringBase64Encode(roomName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	set.Set(FName("HOST_NAME"), StringBase64Encode(mysessionName), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	// 8. netID를 가져오고싶다.
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId(); // 그플레이어의 고유번호, 식별코드같은것.

	UE_LOG(LogTemp, Warning, TEXT("CreateMySession : %s"), *roomName);

	// 방을 만들어 달라고 요청하고싶다.
	sessionInterface->CreateSession(*netID, FName(*roomName), set);

}

void UMithrilDungeonGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete -> sessionName : %s, bWasSuccessful : %d"), *sessionName.ToString(), bWasSuccessful);

	// 만약 방생성을 성공했다면 ServerTravel 하고싶다.
	if (bWasSuccessful)
	{
		GetWorld()->ServerTravel(TEXT("/Game/MedievalDungeon/Maps/Demonstration?listen")); // 컨텐트폴더는 게임이랑 폴더로 이름이바뀜, 서버는 뒤에다가 ?listen을 적어주면 됨.
	}

}

void UMithrilDungeonGameInstance::FindOtherSessions()
{
	// 세션인터페이스를 이용해서 방을 찾고싶다.
	sessionSearch = MakeShareable(new FOnlineSessionSearch);

	// 검색 조건을 설정하고 싶다.
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	// LAN 여부
	sessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL"; // 널이되면 트루, 아니면 펄스, 랜을찾는지 아님 스팀서버를 찾는지

	// 최대 검색 수
	sessionSearch->MaxSearchResults = 10;

	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}

void UMithrilDungeonGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 만약 성공했다면
	if (bWasSuccessful)
	{
		// sessionSearch에서 정보를 가져오고싶다. -> UI로 표현하고싶다.
		auto results = sessionSearch->SearchResults;
		for (int i = 0; i < results.Num(); i++)
		{
			auto item = sessionSearch->SearchResults[i];
			if (item.IsValid() == false) // 요청하는 시점에 방이있는데 받은 시점에서 방이 깨질 수 있음. 그래서 한번 검증
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
	// 호스트의 IP, Port번호를 가져와서 ClientTravel 하고싶다.
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto* pc = GetWorld()->GetFirstPlayerController();
		FString url;
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("ClinetTravel URL : %s"), *url);

		if (url.IsEmpty() == false)
		{
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute); // 퍼셜은 부분적으로 가져감, 앱솔롯트는 가져가는 정보없이 접속.
		}
	}

}

FString UMithrilDungeonGameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UMithrilDungeonGameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
