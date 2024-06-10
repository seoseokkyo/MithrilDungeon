

#include "Interfaces/MithrilDungeonHUD.h"
#include "Interfaces/MainMenuWidget.h"
#include "Interfaces/InteractionWidget.h"
#include "LootPanel.h"

AMithrilDungeonHUD::AMithrilDungeonHUD()
{

}

void AMithrilDungeonHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainManuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		MainManuWidget->AddToViewport(5);
		MainManuWidget->SetVisibility(ESlateVisibility::Collapsed); // 축소설정,Hide와 비슷
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMithrilDungeonHUD::DisplayMenu()
{
	if (MainManuWidget)
	{
		bIsMenuVisible = true;
		MainManuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMithrilDungeonHUD::HideMenu()
{
	if (MainManuWidget)
	{
		bIsMenuVisible = false;
		MainManuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMithrilDungeonHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode; // 게임화면만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();

		const FInputModeGameAndUI InputMode; // UI만 클릭하도록 설정
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}


}

void AMithrilDungeonHUD::ShowInteractionWidget()const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMithrilDungeonHUD::HideInteractionWidget()const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AMithrilDungeonHUD::UpdateInteractionWidget(const FInteractableData& InteractableData)const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}

}


