// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "SocketThread.h"

// Singleton
FSocketThread* FSocketThread::Runnable = NULL;
FSocketThread* FSocketThread::Singleton(FSocket* socket, FAlexaEvent* alexaEvent)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FSocketThread(socket, alexaEvent);
	}
	return Runnable;
}

FSocketThread::FSocketThread(FSocket* socket, FAlexaEvent* alexaEvent)
	: socket(socket), alexaEvent(alexaEvent), clientSocket(nullptr), StopTaskCounter(0)
{
	Thread = FRunnableThread::Create(this, TEXT("FSocketThread"), 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
}

FSocketThread::~FSocketThread()
{
	delete Thread;
	Thread = NULL;
}

bool FSocketThread::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Socket running on %d"), socket->GetPortNo());
	return true;
}

uint32 FSocketThread::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);
	TArray<uint8> buf;
	int32 bytesRead = 0;
	uint32 pendingDataSize = 0;
	bool hasConnectionPending;
	while (StopTaskCounter.GetValue() == 0)
	{
		do {
			// Sleep untill a client is available
			socket->HasPendingConnection(hasConnectionPending);
			Sleep(1);
		} while (!hasConnectionPending && StopTaskCounter.GetValue() == 0);
		// at this point there is a client waiting
		clientSocket = socket->Accept(TEXT("Remote Python Connection"));
		if (clientSocket == NULL) continue; // sanity check
		do {
			// Sleep until data is available 
			Sleep(1);
		} while (!clientSocket->HasPendingData(pendingDataSize) && StopTaskCounter.GetValue() == 0);
		buf.Init(0, pendingDataSize / 8); // allocate umber of chars available
		clientSocket->Recv(buf.GetData(), buf.Num(), bytesRead);	
		if (bytesRead < 1) {
			UE_LOG(LogTemp, Error, TEXT("Socket did not receive enough data: %d"), bytesRead);
			clientSocket->Close();
			continue;	
		}
		int32 command = FCString::Atoi((wchar_t *) buf.GetData());
		alexaEvent->Broadcast(command);
		clientSocket->Close();
	}
	return 0;
}

void FSocketThread::Stop()
{
	StopTaskCounter.Increment();
	if (socket)
		socket->Close();
	if (clientSocket)
		clientSocket->Close();
}

void FSocketThread::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FSocketThread::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = NULL;
	}
}
