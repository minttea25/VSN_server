using System.Net.Sockets;
using System.Text;

namespace VSNWebServer.GameServer
{
    public class GameServerClientHelper(string ip, int port)
    {
        public static void Init(string ip, int port)
        {
            Instance = new(ip, port);
        }

        public static GameServerClientHelper? Instance { get; private set; } = null;

        private readonly string _ip = ip;
        private readonly int _port = port;
        private readonly TcpClient _client = new();
        private NetworkStream? _networkStream = null;

        public string Ip => _ip;
        public int Port => _port;

        public async Task ConnectAsync()
        {
            await _client.ConnectAsync(_ip, _port);
            _networkStream = _client.GetStream();
        }

        public async void SendAsync(byte[] data)
        {
            await _networkStream!.WriteAsync(data);
        }

        // TEMP
        public async Task SendMessageAsync(string message)
        {
            if (_networkStream == null)
            {
                throw new InvalidOperationException("Not connected to server.");
            }

            byte[] data = Encoding.ASCII.GetBytes(message);
            await _networkStream.WriteAsync(data);
        }

        public void Close()
        {
            _networkStream?.Close();
            _client?.Close();
        }
    }
}
