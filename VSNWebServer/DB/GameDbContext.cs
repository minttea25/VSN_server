using StackExchange.Redis;

namespace VSNWebServer.DB
{
    public class GameDbContext
    {
        public IDatabase Database => _dataBase;
        private readonly IDatabase _dataBase;

        public GameDbContext(IConfiguration configuration)
        {
            var conn = configuration.GetConnectionString("GameConnection") ?? throw new Exception("Can not get GameConnection string.");
            var redis = ConnectionMultiplexer.Connect(conn);

            _dataBase = redis.GetDatabase();
        }
    }
}
