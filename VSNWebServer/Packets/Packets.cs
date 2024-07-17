namespace VSNWebServer.Packets
{
    interface IWebRes { }
    interface IWebReq
    {
        public bool Verify();
    }

    [System.Serializable]
    public class WebLoginReq : IWebReq
    {
        public string AccountId { get; set; } = "";
        public string PasswordHash { get; set; } = "";

        public bool Verify()
        {
            return !string.IsNullOrEmpty(AccountId) && !string.IsNullOrEmpty(PasswordHash);
        }
    }

    [System.Serializable]
    public class WebRegisterReq : IWebReq
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
    public class WebLogoutReq : IWebReq
    {
        public uint AccountDbId { get; set; }
        public bool Verify() { return AccountDbId != 0; }
    }

    [System.Serializable]
    public class WebLoginRes : IWebRes
    {
        public uint AccountDbId { get; set; }
        public required string AccountName { get; set; }
    }

    [System.Serializable]
    public class WebRoomInfo : IWebReq
    {
        public uint AccountDbId { get; set; }
        public string UserName { get; set; } = "";
        public uint RoomId { get; set; }
        public bool Verify()
        {
            return AccountDbId != 0 && RoomId != 0 && string.IsNullOrEmpty(UserName);
        }
    }

    [System.Serializable]
    public class WebRoomStatus() : IWebRes
    {
        [System.Serializable]
        public class UserInfo
        {
            public uint UserId { get; set; }
            public required string UserName { get; set; }
            public bool IsReady { get; set; }
        }

        public uint RoomId { get; set; }
        public required string RoomName { get; set; }
        public bool AllReady { get; set; }
        public required List<UserInfo> Players { get; set; }
    }
}
