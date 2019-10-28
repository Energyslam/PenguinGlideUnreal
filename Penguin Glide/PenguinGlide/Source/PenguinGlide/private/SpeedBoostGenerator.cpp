// Fill out your copyright notice in the Description page of Project Settings.
#include "SpeedBoostGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "TempPlayer.h"
#include <chrono>

// Sets default values
ASpeedBoostGenerator::ASpeedBoostGenerator()
{
	speedboostMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SpeedboostMesh"));
	rampMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("RampMesh"));

	RootComponent = speedboostMesh;

	speedboostMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	speedboostMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	speedboostMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	speedboostMesh->SetGenerateOverlapEvents(true);
	speedboostMesh->SetNotifyRigidBodyCollision(true);

	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;

}

void ASpeedBoostGenerator::GenerateSpeedBoost(TArray<FVector> trackVertices, TArray<FVector> trackNormals, int trackVertAmount, int counter, FVector trackScale, int engagedVert)
{
	int arrowThickness = 70;
	TArray<FVector> vertices;
	TArray<int32> triangles;
	int arrowVertices = 6;
	int arrowAmount = FMath::RandRange(1, 3);
	int position;

	do {
		position = FMath::RandRange(2, 25);
	} while (position <= engagedVert + 5 && position >= engagedVert - 2);

	for (int i = 0; i < arrowAmount; i++) {

		//vertices.Append(CalculateSpeedboostVertices(trackVertices, position, i, trackVertAmount, trackScale, arrowVertices, arrowThickness));

		UE_LOG(LogTemp, Error, TEXT("size of verts 1: %f"), (float)vertices.Num());

		vertices.Add(trackVertices[(trackVertAmount * (position + i)) - (trackVertAmount / 2)] * trackScale + FVector(0, 0, 1));
		vertices.Add((trackVertices[(trackVertAmount * (position - 1 + i)) - (trackVertAmount / 2) - 1] * trackScale + FVector(0, 0, 1)));
		vertices.Add((trackVertices[(trackVertAmount * (position - 1 + i)) - (trackVertAmount / 2) + 1] * trackScale + FVector(0, 0, 1)));

		UE_LOG(LogTemp, Error, TEXT("size of verts 2: %f"), (float)vertices.Num());

		FVector length = CalculateNormal(vertices[arrowVertices * i], trackVertices[(trackVertAmount * (position - 1 + i)) - (trackVertAmount / 2)] * trackScale + FVector(0, 0, 1));
		FVector width = CalculateNormal(vertices[arrowVertices * i + 1], vertices[arrowVertices * i + 2]);
		vertices.Add(vertices[arrowVertices * i + 1] - width * arrowThickness);
		vertices.Add(vertices[arrowVertices * i + 2] + width * arrowThickness);
		vertices.Add(vertices[arrowVertices * i] - length * arrowThickness);

		triangles.Append(CreateTriangle(2, 4, 0, arrowVertices, i));
		triangles.Append(CreateTriangle(4, 5, 0, arrowVertices, i));
		triangles.Append(CreateTriangle(3, 1, 0, arrowVertices, i));
		triangles.Append(CreateTriangle(5, 3, 0, arrowVertices, i));

	}

	TArray<FVector2D> speedboostUvs;
	speedboostUvs.Add(FVector2D(1, 0));
	speedboostUvs.Add(FVector2D(0, 0));
	speedboostUvs.Add(FVector2D(0, 1));

	speedboostMesh->CreateMeshSection_LinearColor(counter, vertices, triangles, {}, speedboostUvs, {}, {}, true);
	speedboostMesh->SetMaterial(counter, speedBoostMaterial);
	speedboostMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	int spawnRamp = FMath::RandRange(1, 2);
	if (spawnRamp == 1) {
		GenerateRamp(arrowAmount, position, trackVertices, trackNormals, trackVertAmount, counter, trackScale);
	}
}

// Called when the game starts or when spawned
void ASpeedBoostGenerator::BeginPlay()
{
	//TIJDELIJK EEN TIMER ZETTEN DIE WEER BOOLEAN OP FALSE ZET OM DUBBELE OVERLAP TE VOORKOMEN?
	speedboostMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpeedBoostGenerator::Notify);

	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoundManager::StaticClass(), FoundActors);
	for (AActor* TActor : FoundActors)
	{
		soundManager = Cast<ASoundManager>(TActor);

		if (soundManager != nullptr) {
			return;
		}
		// Do what ever you need with your MyActor
	}
}

void ASpeedBoostGenerator::Notify(class UPrimitiveComponent *ThisComp, AActor *OtherActor, class UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->GetName() == "Player") {
		if (!isSpeedUp) {
			isSpeedUp = true;
			player->constantForwardSpeedPerFrame *= 1.2f;
			UE_LOG(LogTemp, Error, TEXT("WOWIEEEEEEEEEEEEE!!!11 :)"));
			soundManager->PlaySpeedUpSound();
			for (TActorIterator<ATempPlayer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				ActorItr->SetFieldOfViewTo(120, 2.0f);
			}
			GetWorldTimerManager().SetTimer(timehandle, this, &ASpeedBoostGenerator::SpeedUp, 1.0f, false);
		}
	}
}

void ASpeedBoostGenerator::SpeedUp()
{	
	while (player->constantForwardSpeedPerFrame > 75000) {
	//UE_LOG(LogTemp, Error, TEXT("Slow Down %f"), player->constantForwardSpeedPerFrame);
		player->constantForwardSpeedPerFrame -= 500;
	}
	player->constantForwardSpeedPerFrame = 75000;
	isSpeedUp = false;
}

FVector ASpeedBoostGenerator::CalculateNormal(FVector firstPoint, FVector secondPoint)
{
	FVector distance = firstPoint - secondPoint;
	float normalizedDist = sqrtf(powf(distance.X, 2) + powf(distance.Y, 2) + powf(distance.Z, 2));

	return distance /= normalizedDist;
}

void ASpeedBoostGenerator::GenerateRamp(int arrowAmount, int position, TArray<FVector> trackVertices, TArray<FVector> trackNormals, int trackVertAmount, int counter, FVector trackScale)
{
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	int rampLength = 2;
	int rampWidth = 3;
	const int vertexSize = 4;
	float rampHeight = 150;

	for (int i = 0; i < rampLength; i++) {
		UE_LOG(LogTemp, Error, TEXT("Creating ramp verts"));
		vertices.Add(trackVertices[(trackVertAmount * (position + arrowAmount + i)) - (trackVertAmount / 2 - 1)] * trackScale + FVector(0, 0, 1));
		vertices.Add(trackVertices[(trackVertAmount * (position + arrowAmount + i)) - (trackVertAmount / 2)] * trackScale + FVector(0, 0, 1));
		vertices.Add(trackVertices[(trackVertAmount * (position + arrowAmount + i)) - (trackVertAmount / 2 + 1)] * trackScale + FVector(0, 0, 1));

		normals.Add(trackNormals[(trackVertAmount * (position + arrowAmount + i)) - (trackVertAmount / 2 - 1)]);
		normals.Add(trackNormals[(trackVertAmount * (position + arrowAmount + i)) - (trackVertAmount / 2)]);
		normals.Add(trackNormals[(trackVertAmount * (position + arrowAmount + i)) - (trackVertAmount / 2 + 1)]);
	}

	int num = vertices.Num() - 1;

	for (int i = 0; i < 3; i++) {
		vertices.Add(vertices[num - i] + (normals[num - i] * rampHeight));
	}
	UE_LOG(LogTemp, Error, TEXT("num new: %f"), (float)vertices.Num());

	num = vertices.Num() - 1;

	for (int i = 0; i < 2; i++) {
		triangles.Add(0 + i);
		triangles.Add(1 + i);
		triangles.Add(num - i);

		triangles.Add(1 + i);
		triangles.Add(num - 1 - i);
		triangles.Add(num - i);
	}

	for (int i = 0; i < 2; i++) {
		triangles.Add(3 + i);
		triangles.Add(num - i);
		triangles.Add(4 + i);
		
		triangles.Add(4 + i);
		triangles.Add(num - i);
		triangles.Add(num - 1 - i);
	}

	triangles.Add(num);
	triangles.Add(3);
	triangles.Add(0);

	triangles.Add(num - 2);
	triangles.Add(2);
	triangles.Add(5);

	for (int i = 0; i < rampWidth * 2; i++) {
		uvs.Add(FVector2D(0, 0));
		uvs.Add(FVector2D(0, 1));
		uvs.Add(FVector2D(1, 1));

		uvs.Add(FVector2D(1, 0));
		uvs.Add(FVector2D(0, 1));
		uvs.Add(FVector2D(1, 1));
	}


	rampMesh->CreateMeshSection_LinearColor(counter, vertices, triangles, {}, uvs, {}, {}, true);
	rampMesh->SetMaterial(counter, rampMaterial);



}

TArray<int32> ASpeedBoostGenerator::CreateTriangle(int firstVertex, int secondVertex, int thirdVertex, int vertexAmount, int arrowIndex)
{
	TArray<int32> triangles;
	triangles.Add(firstVertex + vertexAmount * arrowIndex);
	triangles.Add(secondVertex + vertexAmount * arrowIndex);
	triangles.Add(thirdVertex + vertexAmount * arrowIndex);
	return triangles;
}

TArray<FVector> ASpeedBoostGenerator::CalculateSpeedboostVertices(TArray<FVector> trackVertices, int position, int i, int trackVertAmount, FVector trackScale, int arrowVertices, int arrowThickness)
{
	static TArray<FVector> vertices;

	/*vertices.Add(trackVertices[(trackVertAmount * (position + i)) - (trackVertAmount / 2)] * trackScale + FVector(0, 0, 1));
	vertices.Add((trackVertices[(trackVertAmount * (position - 1 + i)) - (trackVertAmount / 2) - 1] * trackScale + FVector(0, 0, 1)));
	vertices.Add((trackVertices[(trackVertAmount * (position - 1 + i)) - (trackVertAmount / 2) + 1] * trackScale + FVector(0, 0, 1)));

	FVector length = CalculateNormal(vertices[arrowVertices * i], trackVertices[(trackVertAmount * (position - 1 + i)) - (trackVertAmount / 2)] * trackScale + FVector(0, 0, 1));
	FVector width = CalculateNormal(vertices[arrowVertices * i + 1], vertices[arrowVertices * i + 2]);
	vertices.Add(vertices[arrowVertices * i + 1] - width * arrowThickness);
	vertices.Add(vertices[arrowVertices * i + 2] + width * arrowThickness);
	vertices.Add(vertices[arrowVertices * i] - length * arrowThickness);*/

	return vertices;
}

