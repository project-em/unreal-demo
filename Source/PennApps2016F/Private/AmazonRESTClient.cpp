// Fill out your copyright notice in the Description page of Project Settings.

#include "PennApps2016F.h"
#include "AmazonRESTClient.h"

FString UAmazonRESTClient::AMAZON_URL = TEXT("https://amazon.com");

TSharedPtr<TArray *> UAmazonRESTClient::searchItems(const FString *search)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UAmazonRESTClient::OnResponseReceived);
	//This is the url on which to process the request
	Request->SetURL(UAmazonRESTClient::AMAZON_URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/xml"));
	Request->ProcessRequest();
}