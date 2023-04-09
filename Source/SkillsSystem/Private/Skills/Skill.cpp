// Copyright shenkns Skills System Developed With Unreal Engine. All Rights Reserved 2023.

#include "Skills/Skill.h"

#include "Module/SkillsSystemModule.h"
#include "Components/SkillsComponent.h"
#include "LogSystem.h"
#include "Data/SkillTypeData.h"
#include "Net/UnrealNetwork.h"

USkill::USkill()
{
}

void USkill::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USkill, SkillType)

	DOREPLIFETIME(USkill, CurrentEnergy)
	DOREPLIFETIME(USkill, bCooldownActive)
	
	if(const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

UWorld* USkill::GetWorld() const
{
	return GetOuter() ? GetOuter()->GetWorld() : nullptr;
}

void USkill::InitSkill()
{
	if(!SkillType) return;

	CurrentEnergy = SkillType->bStartCharged ? SkillType->MaxEnergy : SkillType->StartEnergy;

	if(SkillType->bEnableCooldown && SkillType->bEnableStartCooldown)
	{
		StartCooldown();
	}

	BindSkillCharge();
}

void USkill::TryUseSkill()
{
	if(CurrentEnergy < SkillType->UseEnergy) return;

	if(UseSkill())
	{
		CurrentEnergy -= SkillType->UseEnergy;

		if(SkillType->bEnableCooldown)
		{
			StartCooldown();
		}
	}
}

bool USkill::UseSkill_Implementation()
{
	return false;
}

USkillsComponent* USkill::GetSkillsComponent() const
{
	return GetTypedOuter<USkillsComponent>();
}

AActor* USkill::GetOwner() const
{
	return GetSkillsComponent() ? GetSkillsComponent()->GetOwner() : nullptr;
}

float USkill::GetRemainingCooldown() const
{
	return GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimer);
}

void USkill::ChargeSkill(int ChargeEnergy)
{
	CurrentEnergy = FMath::Clamp(CurrentEnergy + FMath::Max(0, ChargeEnergy), 0, SkillType->MaxEnergy);
}

void USkill::OnRepCurrentEnergy()
{
	OnSkillCharged.Broadcast(this, CurrentEnergy, CurrentEnergy - OldEnergy);

	OldEnergy = CurrentEnergy;
}

void USkill::OnCooldownFinished()
{
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimer);
	
	bCooldownActive = false;
}

void USkill::StartCooldown()
{
	bCooldownActive = true;

	GetWorld()->GetTimerManager().SetTimer(CooldownTimer, this, &USkill::OnCooldownFinished, SkillType->Cooldown);
}

void USkill::BindSkillCharge_Implementation()
{
	return;
}
