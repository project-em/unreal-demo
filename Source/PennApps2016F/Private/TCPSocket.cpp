// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
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
	CreateSocket();
	
}

// Called every frame
void ATCPSocket::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ATCPSocket::CreateSocket()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);

	address = TEXT("158.130.163.72");
	// local ip address: 158.130.160.251
	// port 5000
	//int32 port = 19834;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);


	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	port = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->BindNextPort(Socket, *addr, 1000, 1);
	addr->SetPort(port);
	bool connected = Socket->Connect(*addr);
	UE_LOG(LogTemp, Warning, TEXT("Port: %i, connected: %d"), port, connected);
	//SendMessage();
}

void ATCPSocket::SendMessage()
{
	FString serialized = TEXT("loadPlayer|1");
	TCHAR *serializedChar = serialized.GetCharArray().GetData();
	int32 size = FCString::Strlen(serializedChar);
	int32 sent = 0;

	bool successful = Socket->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
}

