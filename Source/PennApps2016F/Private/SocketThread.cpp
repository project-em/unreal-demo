// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "SocketThread.h"
//***********************************************************
//Thread Worker Starts as NULL, prior to being instanced
//		This line is essential! Compiler error without it
FSocketThread* FSocketThread::Runnable = NULL;
//***********************************************************

FSocketThread::FSocketThread(FSocket* socket)
	: socket(socket),
	  StopTaskCounter(0)
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
	//Init the Data 
	UE_LOG(LogTemp, Warning, TEXT("Prime Number Thread Started!"));
	return true;
}

uint32 FSocketThread::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);
	TArray<uint8> buf;
	int32 bytesRead = 0;
	while (StopTaskCounter.GetValue() == 0)
	{
		// block till you get some shit
		if (!socket) return 1;
		socket.recv(buf.GetData(), buf.Num(), bytesRead);
		if (bytesRead < 2) {
			UE_LOG(LogTemp, Error, TEXT("Socket did not receive enough data."));
			return 1;
		}
		int32 command = buf[0];
		// call custom event with number here
	}
	return 0;
}

//stop
void FSocketThread::Stop()
{
	StopTaskCounter.Increment();
}

FString FSocketThread::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//Create a string from a byte array!
	const std::string cstr( reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num() );
 
	//FString can take in the c_str() of a std::string
	return FString(cstr.c_str());
}

FSocketThread* FSocketThread::JoyInit(FSocket* socket)
{
	//Create new instance of thread if it does not exist
	//		and the platform supports multi threading!
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FSocketThread(socket);
	}
	return Runnable;
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

//bool FSocketThread::IsThreadFinished()
//{
//	if (Runnable) return Runnable->IsFinished();
//	return true;
//}