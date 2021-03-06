// Shoot Them Up Game. All rights reserved.


#include "AI/Services/STUChangeWeaponService.h"

#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const auto Controller = OwnerComp.GetAIOwner();
	
	if (Controller && Probability > 0 &&  FMath::FRand() <= Probability)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());

		if (WeaponComponent)
		{
			WeaponComponent->NextWeapon();
		}
	}
}
