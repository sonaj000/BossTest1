// Fill out your copyright notice in the Description page of Project Settings.


#include "FCombatAnimationHandler.h"

FCombatAnimationHandler::FCombatAnimationHandler()
{
}

FCombatAnimationHandler::~FCombatAnimationHandler()
{
}

void FCombatAnimationHandler::OnAnimationBegin()
{
	UE_LOG(LogTemp, Log, TEXT("NyraCombatAnimationHandler::OnAnimationBegin"));
}

void FCombatAnimationHandler::OnAttackBegin()
{
	UE_LOG(LogTemp, Log, TEXT("NyraCombatAnimationHandler::OnAttackBegin"));
}

void FCombatAnimationHandler::OnAttackEnd()
{
	UE_LOG(LogTemp, Log, TEXT("NyraCombatAnimationHandler::OnAttackEnd"));
}

void FCombatAnimationHandler::OnAnimationEnd()
{
	UE_LOG(LogTemp, Log, TEXT("NyraCombatAnimationHandler::OnAnimationEnd"));
}

void FCombatAnimationHandler::OnAnimationInterrupted()
{
}

