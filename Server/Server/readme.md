TheBoat server

C++ 20  
boost.asio

Current outline

- `ClientSession`: one TCP connection, async line-based read/write
- `BoatServer`: accept loop, matchmaking queue, match registry, 20Hz tick
- `FPlayerState`: authoritative player state placeholder
- `FMatchState`: waiting/warmup/in-progress phase container

Current text protocol

- `HELLO <name>`
- `QUEUE`
- `MOVE <x> <y> <z>`
- `FIRE <payload>`
- `PING`

Server responses

- `WELCOME <sessionId>`
- `HELLO_ACK <name>`
- `QUEUE_ACK`
- `MATCH_CREATED <matchId> <phase>`
- `MATCH_STATE <matchId> <phase>`
- `PLAYER_MOVED ...`
- `PLAYER_FIRED ...`
- `SNAPSHOT ...`

Next steps

- replace text protocol with binary packets and sequence numbers
- split lobby, match, combat simulation into separate files
- add authentication, reconnect, interest management, lag compensation
- make movement/fire handlers validate client input against server rules
