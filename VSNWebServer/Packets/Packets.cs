namespace VSNWebServer.Packets
{
    interface IWebPacket
    {
        public bool Verify();
    }

    [System.Serializable]
    public class WebLoginReq : IWebPacket
    {
        public string AccountId { get; set; } = "";
        public string PasswordHash { get; set; } = "";

        public bool Verify()
        {
            return !string.IsNullOrEmpty(AccountId) && !string.IsNullOrEmpty(PasswordHash);
        }
    }

    [System.Serializable]
    public class WebRegisterReq : IWebPacket
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
    public class WebLogoutReq : IWebPacket
    {
        public uint AccountDbId { get; set; }
        public bool Verify() { return AccountDbId != 0; }
    }

    [System.Serializable]
    public class WebRoomInfo : IWebPacket
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
    public class WebRoomStatus()
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
