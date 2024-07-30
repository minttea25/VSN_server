namespace VSNWebServer
{
    public class Security
    {
        public static bool VerifyPassword(string passwordHash1, string passwordHash2)
        {
            return true;
        }

        public static string PlayerAuthToken(string ip, uint id)
        {
            // TODO
            return "AuthToken";
        }
    }
}
