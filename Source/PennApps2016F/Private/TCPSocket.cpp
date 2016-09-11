// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "SocketThread.h"
#include "TCPSocket.h"

// Sets default values
ATCPSocket::ATCPSocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATCPSocket::BeginPlay()
{
	Super::BeginPlay();

	ReceivedAlexaEvent.Broadcast(0);
}

// Called when the game starts or when spawned
void ATCPSocket::BeginDestroy()
{
	Super::BeginDestroy();
	ShutdownSocket();
}

// Called every frame
void ATCPSocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ATCPSocket::CreateSocket()
{
	socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	address = TEXT("158.130.163.72");
	bool canBind;
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	port = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->BindNextPort(socket, *addr, 1000, 1);
	addr->SetPort(port);
	socket->Bind(*addr);
	socket->Listen(1);
	address = addr->ToString(false);
	UE_LOG(LogTemp, Warning, TEXT("Host: %s, Port: %i"), *(addr->ToString(true)), port);
}

void ATCPSocket::AcceptClient()
{
	socketThread = FSocketThread::Singleton(socket, &ReceivedAlexaEvent);
}

void ATCPSocket::ShutdownSocket()
{
	FSocketThread::Shutdown();
}