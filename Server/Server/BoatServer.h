#pragma once

#include "../../Shared/Defines.h"

#include <chrono>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/asio.hpp>

class BoatServer;

enum class EServerPacketType : uint8_t
{
    Unknown,
    Hello,
    QueueJoin,
    Move,
    Fire,
    Ping
};

enum class EMatchPhase : uint8_t
{
    Waiting,
    Warmup,
    InProgress,
    Finished
};

struct FPlayerState
{
    int64 PlayerId = 0;
    std::string PlayerName = "Player";
    Vec3 Position;
    int32 Health = 100;
    bool bAlive = true;
    bool bReady = false;
    int64 MatchId = 0;
};

struct FMatchState
{
    int64 MatchId = 0;
    EMatchPhase Phase = EMatchPhase::Waiting;
    std::unordered_set<int64> PlayerIds;
    std::chrono::steady_clock::time_point StartedAt{};
};

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
    ClientSession(boost::asio::ip::tcp::socket InSocket, BoatServer& InOwner, int64 InSessionId);

    void Start();
    void Deliver(const std::string& InMessage);
    void Close();

    int64 GetSessionId() const;
    std::string GetRemoteAddress() const;

private:
    void StartRead();
    void StartWrite();

    boost::asio::ip::tcp::socket Socket;
    boost::asio::streambuf ReadBuffer;
    std::deque<std::string> PendingWrites;
    BoatServer& Owner;
    int64 SessionId = 0;
    bool bWriting = false;
};

class BoatServer
{
public:
    BoatServer(boost::asio::io_context& InIoContext, int32 InPort);

    void OnMessage(int64 InSessionId, const std::string& InMessage);
    void OnDisconnected(int64 InSessionId);

private:
    void StartAccept();
    void StartTick();
    void RunTick();

    void HandleHello(ClientSession* InSession, std::string_view InPayload);
    void HandleQueueJoin(ClientSession* InSession);
    void HandleMove(ClientSession* InSession, std::string_view InPayload);
    void HandleFire(ClientSession* InSession, std::string_view InPayload);
    void HandlePing(ClientSession* InSession);

    void TryCreateMatch();
    void BroadcastMatchState(const FMatchState& InMatch);
    void BroadcastToPlayers(const std::unordered_set<int64>& InPlayerIds, const std::string& InMessage);
    void SendToSession(int64 InSessionId, const std::string& InMessage);

    ClientSession* FindSession(int64 InSessionId);
    FPlayerState* FindPlayer(int64 InSessionId);
    static std::string_view GetCommand(std::string_view InMessage);

    boost::asio::io_context& IoContext;
    boost::asio::ip::tcp::acceptor Acceptor;
    boost::asio::steady_timer TickTimer;

    std::unordered_map<int64, std::shared_ptr<ClientSession>> Sessions;
    std::unordered_map<int64, FPlayerState> Players;
    std::unordered_map<int64, FMatchState> Matches;
    std::deque<int64> MatchmakingQueue;

    int64 NextSessionId = 1;
    int64 NextMatchId = 1;
    std::chrono::steady_clock::time_point LastTickAt;
};
