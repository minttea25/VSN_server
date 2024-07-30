using Google.FlatBuffers;
using Newtonsoft.Json.Linq;
using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
using VSN;
using VSNWebServer.GameServer;
using VSNWebServer.Packets;
using VSNWebServer.RoomServers;
using WebSocketSharp;

namespace VSNWebServer
{

    public class User(int index)
    {
        public const uint INVALID_USER_ID = 0;

        public uint UserId { get; private set; } = INVALID_USER_ID;
        public string UserName { get; private set; } = "";
        public bool IsReady { get; private set; } = false;
        public bool Valid => UserId != INVALID_USER_ID;
        public int Index { get; private set; } = index;
        public bool IsHost { get; private set; } = false;
        public WebSession? Session { get; private set; } = null;

        public bool SetUser(uint userId, string userName, bool host, WebSession session)
        {
            if (UserId != INVALID_USER_ID) return false;
            UserId = userId;
            UserName = userName;
            IsReady = false;
            IsHost = host;
            Session = session;
            return true;
        }

        public void ToHost()
        {
            IsHost = true;
        }

        public bool Reset()
        {
            UserId = INVALID_USER_ID;
            IsReady = false;
            IsHost = false;
            Session = null;
            return true;
        }

        public bool Ready(bool ready)
        {
            if (UserId == INVALID_USER_ID) return false;
            IsReady = ready;
            return true;
        }
    }

    // TODO : clear the chats when all clients received them.
    public class Room
    {
        struct Chat
        {
            public long Time;
            public string Content;
        }

        public Room(uint roomId, string roomName = RoomManager.DEFAULT_ROOM_NAME, uint maxPlayerCount = RoomManager.MAX_PLAYER_IN_A_ROOM)
        {
            MaxPlayerCount = maxPlayerCount;

            RoomId = roomId;
            RoomName = roomName;
            Users = new User[maxPlayerCount];
            _chats = new List<Chat>[maxPlayerCount];
            for (int i = 0; i < maxPlayerCount; i++)
            {
                Users[i] = new User(i);
                _chats[i] = [];
            }
        }

        public uint RoomId { get; private set; }
        public string RoomName { get; private set; }
        public uint PlayerCount { get; private set; }
        public User[] Users { get; private set; }

        public readonly uint MaxPlayerCount;

        private readonly List<Chat>[] _chats;

        /// <summary>
        /// Broadcast message serialized with MessageTypes.RecvAnnouncement
        /// </summary>
        /// <param name="msg">Plain text to show at chat ui of client</param>
        public void Announce(string msg)
        {
            var announcement = Utils.Json.Serialize(MessageTypes.Announcement, msg);
            foreach (var player in Users)
            {
                if (player.Valid) player.Session?.SendPlain(announcement);
            }
        }

        /// <summary>
        /// Broadcast the text verbatim.
        /// </summary>
        /// <param name="text"></param>
        public void Broadcast(string text)
        {
            foreach (var player in Users)
            {
                if (player.Valid) player.Session?.SendPlain(text);
            }
        }

        public void Clear()
        {
            foreach (var u in Users) u.Reset();
            foreach (var c in _chats) c.Clear();
            RoomId = 0;
            RoomName = string.Empty;
            PlayerCount = 0;
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public WebRoomInfoRes GetRoomInfo()
        {
            return new(this);
        }

        public User? GetPlayer(uint playerId)
        {
            return Users.FirstOrDefault(p => p.UserId == playerId);
        }

        public User? GetPlayerIndex(uint idx)
        {
            if (idx >= MaxPlayerCount) return null;
            else return Users[idx];
        }

        public void ChangeRoomName(string roomName)
        {
            RoomName = roomName;
        }

        public bool AddPlayer(uint userId, string userName, WebSession session)
        {
            if (PlayerCount >= MaxPlayerCount) return false;

            var user = Users.FirstOrDefault(u => u.Valid == false && userId != u.UserId);
            if (user == null) return false;
            else
            {
                user.SetUser(userId, userName, PlayerCount == 0, session);
                PlayerCount++;
                return true;
            }
        }

        public bool ChangeHostNext()
        {
            // move host to next index user
            for (int i = 0; i < MaxPlayerCount; ++i)
            {
                if (Users[i].Valid && Users[i].IsHost == false)
                {
                    Users[i].ToHost();
                    return true;
                }
            }

            // It means there is no more player to set host.
            return false;
        }

        public bool RemovePlayer(uint userId, out UserInfo? info)
        {
            var user = Users.SingleOrDefault(u => u.UserId == userId);
            if (user == null)
            {
                info = null;
                return false;
            }
            else
            {
                _ = ChangeHostNext();

                info = new()
                {
                    UserId = userId,
                    UserName = user.UserName,
                };
                user.Reset();
                PlayerCount--;

                return true;
            }
        }

        public bool UserReady(uint userId, bool ready = true)
        {
            var user = Users.SingleOrDefault(u => u.UserId == userId);
            if (user == null) return false;
            else
            {
                return user.Ready(ready);
            }
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public bool UsersAllReady()
        {
            return Users.All(u => u.IsReady);
        }

        public bool AddChat(uint playerId, long time, string chat)
        {
            var user = Users.FirstOrDefault(p => p.UserId == playerId);
            if (user == null) return false;

            _chats[user.Index].Add(new Chat() { Time = time, Content = chat });
            return true;
        }

        public WebUserInfo[] GetUserInfo()
        {
            return Users.Select(u => new WebUserInfo(u)).ToArray();
        }
    
        public void GameStart()
        {
            // TODO : Check condition

            uint game_id = RoomManager.GetGameId();
            uint map_type_id = 1;
            uint[] spawnable_items = [1, 2, 3, 4, 5];
            byte difficulty = (byte)1;
            uint[] pid = new uint[MaxPlayerCount];
            uint[] pcid = new uint[MaxPlayerCount];
            string[] auth_tokens = new string[MaxPlayerCount];
            int i = 0;
            foreach (var user in Users)
            {
                pid[i] = user.UserId;

                pcid[i] = 1;// TODO

                auth_tokens[i] = Security.PlayerAuthToken("127.0.0.1", user.UserId);


                i++;
            }

            i = 0;
            FlatBufferBuilder fb = new(1024);
            StringOffset[] auth_string_ofs = new StringOffset[MaxPlayerCount];
            foreach (var token in auth_tokens)
            {
                auth_string_ofs[i++] = fb.CreateString(token);
            }
            var item_ofs = WebGameInfoData.CreateSpawnableItemsVector(fb, spawnable_items);
            var pid_ofs = WebGameInfoData.CreatePlayerAccountIdVector(fb, pid);
            var pcid_ofs = WebGameInfoData.CreatePlayerCharacterTypeVector(fb, pcid);
            var auth_ofs = WebGameInfoData.CreatePlayerAuthTokenVector(fb, auth_string_ofs);
            var data = WebGameInfoData.CreateWebGameInfoData(
                fb, game_id: game_id, map_type_id: map_type_id,
                spawnable_itemsOffset: item_ofs,
                difficulty: difficulty,
                player_account_idOffset: pid_ofs,
                player_character_typeOffset: pcid_ofs,
                player_auth_tokenOffset: auth_ofs);
            fb.Finish(data.Value);
            GameServerClientHelper.Instance?.SendAsync(fb.SizedByteArray());
        }
    
        public void TestGameStart()
        {
            uint game_id = RoomManager.GetGameId();
            uint map_type_id = 1;
            uint[] spawnable_items = [1, 2, 3, 4, 5];
            byte difficulty = 1;
            uint[] pid = new uint[1];
            uint[] pcid = new uint[1];
            string[] auth_tokens = new string[1];
            pid[0] = Users[0].UserId;
            pcid[0] = 1;// TODO
            auth_tokens[0] = Security.PlayerAuthToken("127.0.0.1", Users[0].UserId);

            FlatBufferBuilder fb = new(1024);
            StringOffset[] auth_string_ofs = new StringOffset[1];
            auth_string_ofs[0] = fb.CreateString(auth_tokens[0]);

            var item_ofs = WebGameInfoData.CreateSpawnableItemsVector(fb, spawnable_items);
            var pid_ofs = WebGameInfoData.CreatePlayerAccountIdVector(fb, pid);
            var pcid_ofs = WebGameInfoData.CreatePlayerCharacterTypeVector(fb, pcid);
            var auth_ofs = WebGameInfoData.CreatePlayerAuthTokenVector(fb, auth_string_ofs);
            var data = WebGameInfoData.CreateWebGameInfoData(
                fb, game_id: game_id, map_type_id: map_type_id,
                difficulty: difficulty,
                spawnable_itemsOffset: item_ofs,
                player_account_idOffset: pid_ofs,
                player_character_typeOffset: pcid_ofs,
                player_auth_tokenOffset: auth_ofs);
            fb.Finish(data.Value);
            GameServerClientHelper.Instance?.SendAsync(fb.SizedByteArray());

            // send to client
            Broadcast(Utils.Json.Serialize(MessageTypes.GameStart,
                new WebGameStart()
                {
                    RoomId = RoomId,
                    GameId = game_id,
                    GameServerIp = GameServerClientHelper.Instance!.Ip,
                    GameServerPort = GameServerClientHelper.Instance!.Port,
                    AuthTokenKey = /*TEMP*/"a1b2c3",
                }));
        }
    }

    public class RoomManager
    {
        public const int MAX_PLAYER_IN_A_ROOM = 4;
        public const string DEFAULT_ROOM_NAME = "Room";

        private static readonly ConcurrentDictionary<uint, Room> Rooms = new();

        public static Room? GetRoom(uint roomId, bool createIfNoSuchRoom = true)
        {
            if (roomId == 0) return null;

            if (!Rooms.TryGetValue(roomId, out Room? value))
            {
                if (!createIfNoSuchRoom) return null;
                value = new Room(roomId);
                Rooms[roomId] = value;
            }
            return value;
        }

        public static bool JoinRoom(uint roomId, uint userId, string userName, WebSession session)
        {
            if (Rooms.TryGetValue(roomId, out Room? room))
            {
                return room.AddPlayer(userId, userName, session);
            }
            else return false;
        }

        public static bool LeaveRoom(uint roomId, uint userId, out UserInfo? info)
        {
            if (Rooms.TryGetValue(roomId, out Room? room))
            {
                bool ret = room.RemovePlayer(userId, out info);
                if (room.PlayerCount == 0)
                {
                    _ = Rooms.Remove(roomId, out var r);
                    Console.WriteLine($"Room[id={roomId}] is removed.");
                    r?.Clear();
                }
                return ret;
            }
            
            else
            {
                info = null;
                return false;
            }
        }


        static uint gid = 1000;
        public static uint GetGameId()
        {
            // TODO
            return gid++;
        }
    }

}