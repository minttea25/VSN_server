using System.Collections.Concurrent;
using VSNWebServer.Packets;

namespace VSNWebServer
{
    public class User
    {
        const uint INVALID_USER_ID = 0;

        public uint UserId { get; private set; } = INVALID_USER_ID;
        public string UserName { get; private set; } = "";
        public bool IsReady { get; private set; } = false;
        public bool Valid => UserId != INVALID_USER_ID;

        public bool SetUser(uint userId)
        {
            if (UserId != INVALID_USER_ID) return false;
            UserId = userId;
            IsReady = false;
            return true;
        }

        public bool Reset()
        {
            if (UserId == INVALID_USER_ID) return false;
            UserId = INVALID_USER_ID;
            IsReady = false;
            return true;
        }

        public bool Ready(bool ready)
        {
            if (UserId == INVALID_USER_ID) return false;
            IsReady = ready;
            return true;
        }
    }

    public class Room
    {
        public Room(uint roomId, string roomName = RoomManager.DEFAULT_ROOM_NAME, uint maxPlayerCount = RoomManager.MAX_PLAYER_IN_A_ROOM)
        {
            MaxPlayerCount = maxPlayerCount;

            RoomId = roomId;
            RoomName = roomName;
            Users = new User[maxPlayerCount];
            for (int i = 0; i < Users.Length; i++) Users[i] = new User();
        }


        public uint RoomId { get; private set; }
        public string RoomName { get; private set; }
        public User[] Users { get; private set; }
        public uint PlayerCount { get; private set; }
        public uint MaxPlayerCount { get; private set; }

        private TaskCompletionSource tcs = new();

        public bool AddPlayer(uint userId)
        {
            if (PlayerCount >= MaxPlayerCount) return false;

            var user = Users.FirstOrDefault(u => u.Valid == false);
            if (user == null) return false;
            else
            {
                user.SetUser(userId);
                PlayerCount++;
                return true;
            }
        }

        public bool RemovePlayer(uint userId)
        {
            var user = Users.SingleOrDefault(u => u.UserId == userId);
            if (user == null ) return false;
            else
            {
                user.Reset();
                PlayerCount--;
                return true;
            }
        }

        public bool UserReady(uint userId, bool ready)
        {
            var user = Users.SingleOrDefault(u => u.UserId == userId);
            if (user == null) return false;
            else
            {
                return user.Ready(ready);
            }
        }

        public List<WebRoomStatus.UserInfo> GetUserInfo()
        {
            List<WebRoomStatus.UserInfo> info = [];
            foreach (var user in Users)
            {
                if (user.Valid)
                {
                    info.Add(new WebRoomStatus.UserInfo { UserId = user.UserId, UserName = user.UserName, IsReady = user.IsReady });
                }
                else
                {
                    info.Add(new WebRoomStatus.UserInfo { UserId = 0, UserName = "", IsReady = false });
                }
            }

            return info;
        }
    
        public bool UsersAllReady()
        {
            return Users.All(u => u.IsReady);
        }
        public async Task WaitRoomStatusChanged()
        {
            await tcs.Task;
        }

        public void NotifyToClients()
        {
            tcs.TrySetResult();
        }
    }

    public class RoomManager
    {
        public const int MAX_PLAYER_IN_A_ROOM = 4;
        public const string DEFAULT_ROOM_NAME = "Room";

        private static readonly ConcurrentDictionary<uint, Room> Rooms = new();

        public static Room? GetRoom(uint roomId, bool createIfNoSuchRoom = true)
        {
            if (!Rooms.TryGetValue(roomId, out Room? value))
            {
                if (!createIfNoSuchRoom) return null;
                value = new Room(roomId);
                Rooms[roomId] = value;
            }
            return value;
        }

        public static bool JoinRoom(uint roomId, uint userId)
        {
            if (Rooms.TryGetValue(roomId,out Room? room))
            {
                return room.AddPlayer(userId);
            }
            else return false;
        }

        public static bool LeaveRoom(uint roomId, uint userId)
        {
            if (Rooms.TryGetValue(roomId, out Room? room))
            {
                return room.RemovePlayer(userId);
            }
            else return false;
        }

        public static void NotifyChangesToClients(uint roomId)
        {
            GetRoom(roomId)?.NotifyToClients();
        }
    }

}
