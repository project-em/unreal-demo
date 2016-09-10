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

//Init
bool FSocketThread::Init()
{
	//Init the Data 
	UE_LOG(LogTemp, Warning, TEXT("Prime Number Thread Started!"));
	return true;
}

//Run
uint32 FSocketThread::Run()
{
	//Initial wait before starting
	FPlatformProcess::Sleep(0.03);

	int32 num = 0;
	while (StopTaskCounter.GetValue() == 0)
	{
		//PrimeNumbers->Add(FindNextPrimeNumber());
		//PrimesFoundCount++;
		num++;
		if (num % 10000 == 0)
			UE_LOG(LogTemp, Warning, TEXT("num %i"), num);
		//ReceivedAlexaEvent.Broadcast(num);
	}

	return 0;
}

//stop
void FSocketThread::Stop()
{
	StopTaskCounter.Increment();
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