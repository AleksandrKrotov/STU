// Shoot Them Up Game. All rights reserved.


#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(MaxHealth > 0, TEXT("Health component: Max health must be more than 0"))
	
	SetHealth(MaxHealth);
	
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
	}
}

void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld())
		return;

	SetHealth(Health - Damage);
	
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	
	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
	}
}

void USTUHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);
	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
}

bool USTUHealthComponent::TryToAddHealth(int32 HealthAmount)
{
	if (IsHealthFull() || IsDead()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool USTUHealthComponent::IsHealthFull()
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

