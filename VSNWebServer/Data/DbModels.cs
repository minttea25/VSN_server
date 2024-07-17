namespace VSNWebServer.Data
{
    public class AccountDb
    {
        public uint DbId { get; set; }
        public required string AccountName { get; set; }
        public required string AccountId { get; set; }
        public required string AccountPasswordHash { get; set; }
    }
}
