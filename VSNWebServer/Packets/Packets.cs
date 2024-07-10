namespace VSNWebServer.Packets
{
    interface IWebPacket
    {
        public bool Verify();
    }

    [System.Serializable]
    public class WebLogoutReqPacket : IWebPacket
    {
        public uint AccountDbId { get; set; }
        public bool Verify() { return AccountDbId != 0; }
    }

    [System.Serializable]
    public class WebRoomInfo : IWebPacket
    {
        public uint AccountDbId { get; set; }
        public uint RoomId { get; set; }
        public bool Verify()
        {
            return AccountDbId != 0 && RoomId != 0;
        }
    }
}
