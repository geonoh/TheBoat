#include "pch.h"
#include "BoatServer.h"

namespace
{
    constexpr int32 MaxPlayersPerMatch = 8;
    const std::chrono::milliseconds ServerTickInterval(50);

    std::string ToString(const EMatchPhase InPhase)
    {
        switch (InPhase)
        {
        case EMatchPhase::Waiting:
            return "Waiting";
        case EMatchPhase::Warmup:
            return "Warmup";
        case EMatchPhase::InProgress:
            return "InProgress";
        case EMatchPhase::Finished:
            return "Finished";
        default:
            return "Unknown";
        }
    }
}

ClientSession::ClientSession(boost::asio::ip::tcp::socket InSocket, BoatServer& InOwner, const int64 InSessionId)
    : Socket(std::move(InSocket)), Owner(InOwner), SessionId(InSessionId)
{
}

void ClientSession::Start()
{
    Deliver("WELCOME " + std::to_string(SessionId) + "\n");
    StartRead();
}

void ClientSession::Deliver(const std::string& InMessage)
{
    const bool bShouldStartWrite = PendingWrites.empty();
    PendingWrites.push_back(InMessage);

    if (bShouldStartWrite && !bWriting)
    {
        StartWrite();
    }
}

void ClientSession::Close()
{
    boost::system::error_code ErrorCode;
    Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ErrorCode);
    Socket.close(ErrorCode);
}

int64 ClientSession::GetSessionId() const
{
    return SessionId;
}

std::string ClientSession::GetRemoteAddress() const
{
    boost::system::error_code ErrorCode;
    const boost::asio::ip::tcp::endpoint Endpoint = Socket.remote_endpoint(ErrorCode);

    if (ErrorCode)
    {
        return "unknown";
    }

    return Endpoint.address().to_string() + ":" + std::to_string(Endpoint.port());
}

void ClientSession::StartRead()
{
    const std::shared_ptr<ClientSession> Self = shared_from_this();
    boost::asio::async_read_until(Socket, ReadBuffer, '\n',
        [this, Self](const boost::system::error_code& ErrorCode, const std::size_t)
        {
            if (ErrorCode)
            {
                Owner.OnDisconnected(SessionId);
                return;
            }

            std::istream InputStream(&ReadBuffer);
            std::string Message;
            std::getline(InputStream, Message);

            if (!Message.empty() && Message.back() == '\r')
            {
                Message.pop_back();
            }

            Owner.OnMessage(SessionId, Message);
            StartRead();
        });
}

void ClientSession::StartWrite()
{
    if (PendingWrites.empty())
    {
        bWriting = false;
        return;
    }

    bWriting = true;

    const std::shared_ptr<ClientSession> Self = shared_from_this();
    boost::asio::async_write(Socket, boost::asio::buffer(PendingWrites.front()),
        [this, Self](const boost::system::error_code& ErrorCode, const std::size_t)
        {
            if (ErrorCode)
            {
                Owner.OnDisconnected(SessionId);
                return;
            }

            PendingWrites.pop_front();
            StartWrite();
        });
}

BoatServer::BoatServer(boost::asio::io_context& InIoContext, const int32 InPort)
    : IoContext(InIoContext)
    , Acceptor(InIoContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), InPort))
    , TickTimer(InIoContext)
    , LastTickAt(std::chrono::steady_clock::now())
{
    StartAccept();
    StartTick();
}

void BoatServer::OnMessage(const int64 InSessionId, const std::string& InMessage)
{
    ClientSession* Session = FindSession(InSessionId);
    if (!Session)
    {
        return;
    }

    const std::string_view Command = GetCommand(InMessage);
    if (Command.empty())
    {
        SendToSession(InSessionId, "ERROR EmptyCommand\n");
        return;
    }

    if (Command == "HELLO")
    {
        const std::size_t PayloadOffset = std::min<std::size_t>(InMessage.size(), 6);
        HandleHello(Session, std::string_view(InMessage).substr(PayloadOffset));
        return;
    }

    if (Command == "QUEUE")
    {
        HandleQueueJoin(Session);
        return;
    }

    if (Command == "MOVE")
    {
        const std::size_t PayloadOffset = std::min<std::size_t>(InMessage.size(), 5);
        HandleMove(Session, std::string_view(InMessage).substr(PayloadOffset));
        return;
    }

    if (Command == "FIRE")
    {
        const std::size_t PayloadOffset = std::min<std::size_t>(InMessage.size(), 5);
        HandleFire(Session, std::string_view(InMessage).substr(PayloadOffset));
        return;
    }

    if (Command == "PING")
    {
        HandlePing(Session);
        return;
    }

    SendToSession(InSessionId, "ERROR UnknownCommand\n");
}

void BoatServer::OnDisconnected(const int64 InSessionId)
{
    const std::unordered_map<int64, FPlayerState>::iterator PlayerIt = Players.find(InSessionId);
    if (PlayerIt != Players.end())
    {
        if (PlayerIt->second.MatchId != 0)
        {
            const std::unordered_map<int64, FMatchState>::iterator MatchIt = Matches.find(PlayerIt->second.MatchId);
            if (MatchIt != Matches.end())
            {
                MatchIt->second.PlayerIds.erase(InSessionId);
                BroadcastToPlayers(MatchIt->second.PlayerIds, "PLAYER_LEFT " + std::to_string(InSessionId) + "\n");
            }
        }

        Players.erase(PlayerIt);
    }

    MatchmakingQueue.erase(
        std::remove(MatchmakingQueue.begin(), MatchmakingQueue.end(), InSessionId),
        MatchmakingQueue.end());

    const std::unordered_map<int64, std::shared_ptr<ClientSession>>::iterator SessionIt = Sessions.find(InSessionId);
    if (SessionIt != Sessions.end())
    {
        std::cout << "Client disconnected: " << SessionIt->second->GetRemoteAddress() << '\n';
        SessionIt->second->Close();
        Sessions.erase(SessionIt);
    }
}

void BoatServer::StartAccept()
{
    Acceptor.async_accept(
        [this](const boost::system::error_code& ErrorCode, boost::asio::ip::tcp::socket InSocket)
        {
            if (!ErrorCode)
            {
                const int64 SessionId = NextSessionId++;
                std::shared_ptr<ClientSession> Session = std::make_shared<ClientSession>(std::move(InSocket), *this, SessionId);
                Sessions.emplace(SessionId, Session);
                FPlayerState Player;
                Player.PlayerId = SessionId;
                Players.emplace(SessionId, std::move(Player));

                std::cout << "Client connected: " << Session->GetRemoteAddress() << '\n';
                Session->Start();
            }

            StartAccept();
        });
}

void BoatServer::StartTick()
{
    TickTimer.expires_after(ServerTickInterval);
    TickTimer.async_wait([this](const boost::system::error_code& ErrorCode)
    {
        if (ErrorCode)
        {
            return;
        }

        RunTick();
        StartTick();
    });
}

void BoatServer::RunTick()
{
    LastTickAt = std::chrono::steady_clock::now();

    for (std::pair<const int64, FMatchState>& MatchPair : Matches)
    {
        const int64 MatchId = MatchPair.first;
        FMatchState& Match = MatchPair.second;
        if (Match.PlayerIds.empty())
        {
            continue;
        }

        if (Match.Phase == EMatchPhase::Warmup &&
            LastTickAt - Match.StartedAt > std::chrono::seconds(5))
        {
            Match.Phase = EMatchPhase::InProgress;
            BroadcastToPlayers(Match.PlayerIds, "MATCH_STATE " + std::to_string(MatchId) + " InProgress\n");
        }

        BroadcastMatchState(Match);
    }
}

void BoatServer::HandleHello(ClientSession* InSession, std::string_view InPayload)
{
    FPlayerState* Player = FindPlayer(InSession->GetSessionId());
    if (!Player)
    {
        return;
    }

    std::string PlayerName(InPayload);
    if (PlayerName.empty())
    {
        PlayerName = "Player" + std::to_string(InSession->GetSessionId());
    }

    Player->PlayerName = PlayerName;
    SendToSession(InSession->GetSessionId(), "HELLO_ACK " + Player->PlayerName + "\n");
}

void BoatServer::HandleQueueJoin(ClientSession* InSession)
{
    const int64 SessionId = InSession->GetSessionId();
    FPlayerState* Player = FindPlayer(SessionId);
    if (!Player)
    {
        return;
    }

    if (Player->MatchId != 0)
    {
        SendToSession(SessionId, "ERROR AlreadyInMatch\n");
        return;
    }

    const bool bAlreadyQueued = std::find(MatchmakingQueue.begin(), MatchmakingQueue.end(), SessionId) != MatchmakingQueue.end();
    if (!bAlreadyQueued)
    {
        MatchmakingQueue.push_back(SessionId);
        SendToSession(SessionId, "QUEUE_ACK\n");
    }

    TryCreateMatch();
}

void BoatServer::HandleMove(ClientSession* InSession, std::string_view InPayload)
{
    FPlayerState* Player = FindPlayer(InSession->GetSessionId());
    if (!Player)
    {
        return;
    }

    std::stringstream Stream{std::string(InPayload)};
    Stream >> Player->Position.X >> Player->Position.Y >> Player->Position.Z;

    if (Stream.fail())
    {
        SendToSession(InSession->GetSessionId(), "ERROR InvalidMove\n");
        return;
    }

    if (Player->MatchId != 0)
    {
        const std::unordered_map<int64, FMatchState>::iterator MatchIt = Matches.find(Player->MatchId);
        if (MatchIt != Matches.end())
        {
            BroadcastToPlayers(
                MatchIt->second.PlayerIds,
                "PLAYER_MOVED " + std::to_string(Player->PlayerId) + " " +
                std::to_string(Player->Position.X) + " " +
                std::to_string(Player->Position.Y) + " " +
                std::to_string(Player->Position.Z) + "\n");
        }
    }
}

void BoatServer::HandleFire(ClientSession* InSession, std::string_view InPayload)
{
    FPlayerState* Player = FindPlayer(InSession->GetSessionId());
    if (!Player || Player->MatchId == 0)
    {
        SendToSession(InSession->GetSessionId(), "ERROR NotInMatch\n");
        return;
    }

    const std::unordered_map<int64, FMatchState>::iterator MatchIt = Matches.find(Player->MatchId);
    if (MatchIt == Matches.end())
    {
        return;
    }

    BroadcastToPlayers(
        MatchIt->second.PlayerIds,
        "PLAYER_FIRED " + std::to_string(Player->PlayerId) + " " + std::string(InPayload) + "\n");
}

void BoatServer::HandlePing(ClientSession* InSession)
{
    SendToSession(InSession->GetSessionId(), "PONG\n");
}

void BoatServer::TryCreateMatch()
{
    if (MatchmakingQueue.size() < 2)
    {
        return;
    }

    FMatchState Match;
    Match.MatchId = NextMatchId++;
    Match.Phase = EMatchPhase::Warmup;
    Match.StartedAt = std::chrono::steady_clock::now();

    while (!MatchmakingQueue.empty() && Match.PlayerIds.size() < MaxPlayersPerMatch)
    {
        const int64 SessionId = MatchmakingQueue.front();
        MatchmakingQueue.pop_front();

        FPlayerState* Player = FindPlayer(SessionId);
        if (!Player)
        {
            continue;
        }

        Player->MatchId = Match.MatchId;
        Player->Position = Vec3(0.0, 0.0, 0.0);
        Match.PlayerIds.insert(SessionId);
    }

    const int64 MatchId = Match.MatchId;
    Matches.emplace(MatchId, Match);

    BroadcastToPlayers(
        Match.PlayerIds,
        "MATCH_CREATED " + std::to_string(MatchId) + " " + ToString(Match.Phase) + "\n");
}

void BoatServer::BroadcastMatchState(const FMatchState& InMatch)
{
    std::ostringstream Stream;
    Stream << "SNAPSHOT " << InMatch.MatchId << " " << ToString(InMatch.Phase) << " " << InMatch.PlayerIds.size();

    for (const int64 PlayerId : InMatch.PlayerIds)
    {
        const std::unordered_map<int64, FPlayerState>::const_iterator PlayerIt = Players.find(PlayerId);
        if (PlayerIt == Players.end())
        {
            continue;
        }

        const FPlayerState& Player = PlayerIt->second;
        Stream << " | " << Player.PlayerId
               << " " << Player.PlayerName
               << " " << Player.Position.X
               << " " << Player.Position.Y
               << " " << Player.Position.Z
               << " " << Player.Health
               << " " << (Player.bAlive ? 1 : 0);
    }

    Stream << "\n";
    BroadcastToPlayers(InMatch.PlayerIds, Stream.str());
}

void BoatServer::BroadcastToPlayers(const std::unordered_set<int64>& InPlayerIds, const std::string& InMessage)
{
    for (const int64 PlayerId : InPlayerIds)
    {
        SendToSession(PlayerId, InMessage);
    }
}

void BoatServer::SendToSession(const int64 InSessionId, const std::string& InMessage)
{
    ClientSession* Session = FindSession(InSessionId);
    if (Session)
    {
        Session->Deliver(InMessage);
    }
}

ClientSession* BoatServer::FindSession(const int64 InSessionId)
{
    const std::unordered_map<int64, std::shared_ptr<ClientSession>>::iterator It = Sessions.find(InSessionId);
    return It != Sessions.end() ? It->second.get() : nullptr;
}

FPlayerState* BoatServer::FindPlayer(const int64 InSessionId)
{
    const std::unordered_map<int64, FPlayerState>::iterator It = Players.find(InSessionId);
    return It != Players.end() ? &It->second : nullptr;
}

std::string_view BoatServer::GetCommand(const std::string_view InMessage)
{
    const std::size_t DelimiterIndex = InMessage.find(' ');
    if (DelimiterIndex == std::string_view::npos)
    {
        return InMessage;
    }

    return InMessage.substr(0, DelimiterIndex);
}
