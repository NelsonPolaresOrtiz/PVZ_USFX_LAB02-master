// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"
#include "PVZ_USFX_LAB02Projectile.h"
#include "Zombie.h"

// Sets default values

//
int32 APlant::TotalProyectilesDisparados = 0;

APlant::APlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Registra la funci�n para la detecci�n de colisiones
	//OnActorBeginOverlap.AddDynamic(this, &APlant::OnOverlapBegin);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlantMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));

	PlantMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlantMesh"));
	//PlantMeshComponent->SetStaticMesh(PlantMesh.Object);
	//PlantMeshComponent->SetCollisionProfileName(TEXT("Ignore"));
	PlantMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	PlantMeshComponent->SetSimulatePhysics(false);
	PlantMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = PlantMeshComponent;

	bCanFire = true;
	GunOffset = FVector(90.f, 0.f, 0.f);
	FireRate = 0.2f;
	TiempoTranscurrido = 0.0f;
	TiempoEntreDisparos = 0.01f;
	Tags.Add(TEXT("Plant"));

	if (TotalProyectilesDisparados == 0)
	{
		TotalProyectilesDisparados = 0;
	}
}

//void APlant::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
//{
//	// Verifica si el otro actor es un zombie
//	AZombie* ActualZombie = Cast<AZombie>(OtherActor);
//
//	if (ActualZombie)
//	{
//		TakeDamage(ActualZombie->DamageGenerates, FDamageEvent(), nullptr, this);
//
//		// Reduce la energ�a de la planta cuando un zombie est� cerca
//		// Puedes implementar tu propia l�gica para reducir la energ�a aqu�
//		// Por ejemplo, disminuir una variable que represente la energ�a de la planta
//		// Tambi�n puedes programar el tiempo entre las reducciones de energ�a
//	}
//}



// Called when the game starts or when spawned
void APlant::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Health <= 0)
	{
		Destroy();
	}

	TiempoTranscurrido += DeltaTime;
	if (TiempoTranscurrido >= TiempoEntreDisparos){

		FireShot(FVector(0.0f, 1.0f, 0.0f));
		TiempoTranscurrido = 0.0f;
	}
	
	// Configura un temporizador para mostrar el n�mero total de proyectiles disparados cada 5 segundos.
	/*if (TiempoTranscurrido >= 5.0f)
	{
		MostrarNumeroProyectiles();
		TiempoTranscurrido = 0.0f;*/
		/*MostrarNumeroProyectiles();*/
	//	// Muestra el n�mero total de proyectiles disparados en la consola (puedes personalizar esto seg�n tus necesidades).
	//	UE_LOG(LogTemp, Warning, TEXT("N�mero total de proyectiles disparados: %d"), TiempoTranscurrido);

	/*	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("N�mero total de proyectiles disparados: %d"), TotalProyectilesDisparados));
	}*/
}
//funci�n para mostrar el n�mero total de proyectiles disparados.
//void APlant::MostrarNumeroProyectiles()
//{
//	// Muestra el n�mero total de proyectiles disparados en la consola (puedes personalizar esto seg�n tus necesidades).
//	UE_LOG(LogTemp, Warning, TEXT("N�mero total de proyectiles disparados: %d"), TotalProyectilesDisparados);
//
//	// Reinicia el contador de proyectiles.
//	
//
//	//// Construye el mensaje de depuraci�n con el formato deseado.
//	FString Mensaje = FString::Printf(TEXT("N�mero total de proyectiles disparados: %d"), TotalProyectilesDisparados);
//
//	//// Agrega el mensaje de depuraci�n en la pantalla con color rojo.
//	FColor ColorRojo = FColor::Red;
//	float Duracion = 100.0f; // Duraci�n del mensaje en pantalla (en segundos).
//	GEngine->AddOnScreenDebugMessage(-1, Duracion, ColorRojo, Mensaje);
//
//
//}

void APlant::FireShot(FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire == true)
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				// spawn the projectile
				World->SpawnActor<APVZ_USFX_LAB02Projectile>(SpawnLocation, FireRotation);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlant::ShotTimerExpired, FireRate);

			// Incrementa el n�mero total de proyectiles disparados
			 // Aumentar el contador de proyectiles disparados
			 // Aumentar el contador de proyectiles disparados
			TotalProyectilesDisparados++;

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &APlant::ShotTimerExpired, FireRate);
		}
	}
}

void APlant::ShotTimerExpired()
{
	bCanFire = true;
}

float APlant::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Aqu� puedes manejar el da�o como desees, por ejemplo, actualizando la salud del actor.
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health Zombie: %f"), this->Health));

	Health -= Damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Health Zombie: %f"), this->Health));

	// Devuelve la cantidad de da�o que se aplic� realmente.
	return Health;
}
