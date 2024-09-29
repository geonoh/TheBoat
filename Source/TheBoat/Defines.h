#pragma once
#include <vector>

// Gunny TODO
// Tempcode
enum class EPartType
{
	None,
	One,
	Two,
	Three,
	Four,
	Five,
	Max,
};

enum class ETeamType
{
	None,
	First,
	Second,
	Max,
};

struct FCombatCharacterInfo
{
	explicit FCombatCharacterInfo(int64 InCharacterInfo, ETeamType InTeamType,
	                              const std::vector<EPartType>& InOwnedTypes)
	: CharacterId(InCharacterInfo), TeamType(InTeamType), OwnedParts(InOwnedTypes)
	{
	}

	int64 CharacterId;
	ETeamType TeamType;
	std::vector<EPartType> OwnedParts;
};

struct FCombatSpawnerInfo
{
	explicit FCombatSpawnerInfo(int64 InSpawnerId, double InXPos, double InYPos, double InZPos)
		: SpawnerId(InSpawnerId), XPos(InXPos), YPos(InYPos), ZPos(InZPos)
	{
	}

	int64 SpawnerId;
	double XPos;
	double YPos;
	double ZPos;
};

struct FCombatEnterInfo
{
	explicit FCombatEnterInfo(const int64 InMatchId,
	                          const std::vector<FCombatSpawnerInfo>& InSpawnerInfos,
	                          const std::vector<FCombatCharacterInfo>& InCharacterInfos)
		: MatchId(InMatchId), SpawnerInfos(InSpawnerInfos),
		  CharacterInfos(InCharacterInfos)
	{
	}

	int64 MatchId;

	std::vector<FCombatSpawnerInfo> SpawnerInfos;
	std::vector<FCombatCharacterInfo> CharacterInfos;
};

struct FCombatDuringInfo
{
};
