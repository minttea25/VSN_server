namespace VSNWebServer.Data
{
    public class AccountDb
    {
        public int DbId { get; set; }
        public required string UserName { get; set; }
        public required string AccountId { get; set; }
        public required string AccountPasswordHash { get; set; }
    }
}
