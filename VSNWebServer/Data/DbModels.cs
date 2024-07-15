namespace VSNWebServer.Data
{
    public class AccountDb
    {
        public int DbId { get; set; }
        public required string AccountName { get; set; }
        public required string AccountId { get; set; }
        public required string AccountPasswordHash { get; set; }
    }
}
