// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArenaBattle.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

public:
	UPROPERTY(VisibleAnywhere, Category=Box)
	class UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category=Box)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category=Effect)
	class UParticleSystemComponent* Effect;

	UPROPERTY(EditInstanceOnly, Category=Box)
	TSubclassOf<class AABWeapon> WeaponItemClass; // TSubclassOf는 특정 클래스와 상속받은 클래스들로 목록을 한정한다.
};
