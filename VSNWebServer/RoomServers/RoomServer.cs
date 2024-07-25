﻿using VSNWebServer.Packets;
using WebSocketSharp;
using WebSocketSharp.Server;

namespace VSNWebServer.RoomServers
{
    public class RoomServer
    {
        public const string DEFAULT_URL = "ws://127.0.0.1:8071";
        public const string DEFAULT_PATH = "/room";

        public static IReadOnlyDictionary<uint, WebSession> Sessions => _sessions;
        private readonly static Dictionary<uint, WebSession> _sessions = new();
        private static WebSocketServer? wssv = null;
        public static WebSocketServer? Start(string url, string path)
        {
            try
            {
                MessageHandler.Init();

                wssv = new WebSocketServer(url);
                wssv.AddWebSocketService<WebSession>(path);
                wssv.Start();

            }
            catch (Exception ex)
            {
                Console.WriteLine("Exception occured during starting websocket: " + ex);
            }
            finally
            {

            }
            return wssv;
        }

        public static bool AddSession(WebSession session)
        {
            if (_sessions.TryAdd(session.SessionId, session)) return true;
            else return false;
        }

        public static void RemoveSession(uint sessionId)
        {
            _ = _sessions.Remove(sessionId);
        }

        public static void Broadcast(string message)
        {
            if (wssv == null) return;

            foreach (WebSession session in Sessions.Values)
            {
                session.SendPlain(message);
            }
        }
    }
}