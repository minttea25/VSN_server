using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace VSNWebServer.GameServer
{
    public class GameServerClientHelper
    {
        /// <summary>
        /// Note: callback.Target is TcpClient
        /// </summary>
        /// <param name="callback"></param>
        public static void Process(Action<GameServerClient> connectedCallback)
        {
            GameServerClient client = new();
            client.ConnectAutoAsync(connectedCallback);
        }
    }

    public class GameServerClient
    {
        const string GAMESERVER_IP = "127.0.0.1";
        const ushort GAMESERVER_PORT = 8070;

        private string _ip = string.Empty;
        private int _port = 0;
        private readonly TcpClient _client = new();
        private NetworkStream? _networkStream = null;
        private Action<GameServerClient>? _connectedCallback = null;

        private byte[] _buffer = new byte[128];

        public string Ip => _ip;
        public int Port => _port;

        public async Task ConnectAsync()
        {
            await _client.ConnectAsync(_ip, _port);
            _networkStream = _client.GetStream();

            _networkStream.BeginRead(_buffer, 0, _buffer.Length, new AsyncCallback(ReadCallback), _client);
        }

        /// <summary>
        /// Find available server and connect to it.
        /// </summary>
        public void ConnectAuto()
        {
            FindProperServer(this);

            // block
            _client.Connect(Ip, Port);

            _networkStream = _client.GetStream();
            _networkStream.BeginRead(_buffer, 0, _buffer.Length, new AsyncCallback(ReadCallback), _client);
        }

        public void ConnectAutoAsync(Action<GameServerClient> connectedCallback)
        {
            FindProperServer(this);

            _connectedCallback = connectedCallback;
            
           _client.BeginConnect(IPAddress.Parse(Ip), Port, new AsyncCallback(ConnectCallback), _client);

        }

        private static void FindProperServer(GameServerClient client)
        {
            // TODO : Get info from server status DB
            // TEMP
            client._ip = GAMESERVER_IP;
            client._port = GAMESERVER_PORT;
        }

        private void ConnectCallback(IAsyncResult ar)
        {
            _networkStream = _client.GetStream();
            _networkStream.BeginRead(_buffer, 0, _buffer.Length, new AsyncCallback(ReadCallback), _client);

            _connectedCallback?.Invoke(this);
        }

        private void ReadCallback(IAsyncResult ar)
        {
            if (_client.Connected == false
                || _networkStream == null) return;

            try
            {
                int bytesRead = _networkStream.EndRead(ar);
                
                if (bytesRead > 0)
                {
                    _networkStream.BeginRead(_buffer, 0, _buffer.Length, new AsyncCallback(ReadCallback), _client);
                    // TODO ? : process data

                    _buffer = new byte[128];
                }
                else
                {
                    Console.WriteLine("Disconnected from game server.");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Disconnected by the server: {ex}");
            }
        }

        public async void SendAsync(byte[] data)
        {
            await _networkStream!.WriteAsync(data);
        }

        // TEMP
        public async Task SendMessageAsync(string message, Encoding encoding)
        {
            if (_networkStream == null)
            {
                throw new InvalidOperationException("Not connected to server.");
            }

            byte[] data = encoding.GetBytes(message);
            await _networkStream.WriteAsync(data);
        }

        private void Dispose()
        {
            _client?.Dispose();
            _networkStream?.Dispose();

            _connectedCallback = null;
            _ip = string.Empty;
            _port = 0;
        }

        public void Close()
        {
            _networkStream?.Close();
            _client?.Close();
        }
    }
}
