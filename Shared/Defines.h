#pragma once
#include <vector>

// For server, Using C++ std
// Do not use UE container

struct Vec3
{
	Vec3(): X(0.0), Y(0.0), Z(0.0)
	{
	}

	Vec3(const double InX, const double InY, const double InZ): X(InX), Y(InY), Z(InZ)
	{
	}

	bool operator==(const Vec3& InOther) const
	{
		return InOther.X == X && InOther.Y == Y && InOther.Z == Z;
	}

	void operator=(const Vec3& InOther)
	{
		X = InOther.X;
		Y = InOther.Y;
		Z = InOther.Z;
	}

	double X, Y, Z;
};

static constexpr int BOAT_SERVER_PORT = 5000;

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
	explicit FCombatCharacterInfo(const int64 InCharacterInfo, const ETeamType InTeamType, const Vec3& InPos,
	                              const std::vector<EPartType>& InOwnedParts)
	: CharacterId(InCharacterInfo), TeamType(InTeamType), Pos(InPos), OwnedParts(InOwnedParts)
	{
	}

	int64 CharacterId;
	ETeamType TeamType;
	Vec3 Pos;
	std::vector<EPartType> OwnedParts;
};

struct FCombatSpawnerInfo
{
	explicit FCombatSpawnerInfo(int64 InSpawnerId, const Vec3& InPos)
		: SpawnerId(InSpawnerId), Pos(InPos)
	{
	}

	int64 SpawnerId;
	Vec3 Pos;
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
