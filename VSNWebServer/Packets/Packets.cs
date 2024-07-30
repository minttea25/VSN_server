namespace VSNWebServer.Packets
{
    public interface IWebResponse { }
    public interface IWebRequest
    {
        public bool Verify();
    }

    public class UserInfo
    {
        public uint UserId = 0;
        public string UserName = string.Empty;
    }

    #region Login Packets
    [System.Serializable]
    public class WebLoginReq : IWebRequest
    {
        public string AccountId { get; set; } = "";
        public string PasswordHash { get; set; } = "";

        public bool Verify()
        {
            return !string.IsNullOrEmpty(AccountId) && !string.IsNullOrEmpty(PasswordHash);
        }
    }

    [System.Serializable]
    public class WebRegisterReq : IWebRequest
    {
        public string AccountId { get; set; } = "";
        public string AccountName { get; set; } = "";
        public string PasswordHash { get; set; } = "";

        public bool Verify()
        {
            return !string.IsNullOrEmpty(AccountId) && !string.IsNullOrEmpty(AccountName) && !string.IsNullOrEmpty(PasswordHash);
        }
    }


    [System.Serializable]
    public class WebLogoutReq : IWebRequest
    {
        public uint AccountDbId { get; set; }
        public bool Verify() { return AccountDbId != 0; }
    }

    [System.Serializable]
    public class WebLoginRes : IWebResponse
    {
        public uint AccountDbId { get; set; }
        public required string AccountName { get; set; }
    }
    #endregion

    [System.Serializable]
    public class WebRoomReq : IWebRequest
    {
        public uint AccountDbId { get; set; }
        public string UserName { get; set; } = "";
        public uint RoomId { get; set; }
        public bool Verify()
        {
            return AccountDbId != 0 && RoomId != 0 && !string.IsNullOrEmpty(UserName);
        }
    }

    [System.Serializable]
    public class WebRoomPlayerInfoReq : IWebRequest
    {
        public uint RoomId { get; set; }
        public uint PlayerIndex { get; set; }

        public bool Verify()
        {
            if (RoomId == 0 || PlayerIndex > RoomManager.MAX_PLAYER_IN_A_ROOM) return false;
            return true;
        }
    }

    [System.Serializable]
    public class WebPlayerJoin : IWebResponse
    {
        public uint RoomId { get; set; }
        public uint UserId { get; set; }
        public string UserName { get; set; } = string.Empty;
    }

    [System.Serializable]
    public class WebRoomInfoRes(Room room) : IWebResponse
    {
        public uint RoomId { get; private set; } = room.RoomId;
        public string RoomName { get; private set; } = room.RoomName;
        public WebUserInfo[] Users { get; private set; } = room.GetUserInfo();
        public uint PlayerCount { get; private set; } = room.PlayerCount;
        public uint MaxPlayerCount { get; private set; } = room.MaxPlayerCount;
    }

    [System.Serializable]
    public class WebUserReady : IWebResponse
    {
        public uint RoomId { get; set; }
        public uint UserId { get; set; }
        public bool IsReady { get; set; }
    }


    [Serializable]
    public class WebUserInfo : IWebResponse
    {
        public WebUserInfo() { }
        public WebUserInfo(User user)
        {
            UserId = user.UserId;
            UserName = user.UserName;
            IsReady = user.IsReady;
        }
        public uint UserId { get; set; }
        public string UserName { get; set; } = string.Empty;
        public bool IsReady { get; set; }
    }

    [System.Serializable]
    public class WebRoomInfoChanges : IWebResponse
    {
        public uint RoomId { get; set; } = 0;
        public uint LastUpdate { get; set; } = 0;
        public bool RoomNameChanged { get; set; } = false;
        public uint[] Players { get; set; } = [0, 0, 0, 0]; // 0이면 없는 것. leave한 플레이어 포함
        public bool[] PlayerReady { get; set; } = [false, false, false, false];
    }

    [System.Serializable]
    public class WebChatData : IWebRequest, IWebResponse
    {
        public string UserName { get; set; } = "";
        public string ChatContent { get; set; } = "";

        public bool Verify()
        {
            return !string.IsNullOrEmpty(UserName) && !string.IsNullOrEmpty(ChatContent);
        }
    }

    [System.Serializable]
    public class WebChatDataRes : IWebResponse
    {
        public uint LastUpdate { get; set; } = 0;
        public List<WebChatData> NewChats { get; set; } = new();
    }

    [System.Serializable]
    public class WebGameStart : IWebResponse
    {
        public uint RoomId { get; set; }
        public uint GameId { get; set; }
        public string GameServerIp { get; set; } = string.Empty;
        public int GameServerPort { get; set; }
        public string AuthTokenKey { get; set; } = string.Empty;
    }
}