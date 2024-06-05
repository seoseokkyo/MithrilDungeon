// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryTooltip.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "inventory/ItemBase.h"
#include "Inventory/InventoryItemSlot.h"


void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();
	
	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();
	
	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Equipmentable :
		break;
	case EItemType::Armor:
		break;
	case EItemType::Weapon:
		break;
	case EItemType::Shield:
		break;
	case EItemType::Spell:
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		break;
	case EItemType::Mundane:
		ItemType->SetText(FText::FromString("Mundane Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		UsageText->SetVisibility(ESlateVisibility::Collapsed);
		MaxStackSizeText->SetVisibility(ESlateVisibility::Collapsed);
		break;
		default: 
			break;

	}

	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);
	MaxStackSizeText->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.SellValue));
	StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));

	if (ItemBeingHovered->NumericData.bisStackable)
	{
		MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
