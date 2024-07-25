﻿using Newtonsoft.Json.Linq;
using VSNWebServer.Packets;
using WebSocketSharp;
using WebSocketSharp.Server;

namespace VSNWebServer.RoomServers
{
    public class WebSession : WebSocketBehavior
    {
        static uint IdIssuer = 1;
        static uint GetId => IdIssuer++;

        public uint SessionId { get; private set; }

        public User? User { get; private set; }
        public Room? Room { get; private set; }

        public bool Joined => User != null && Room != null;

        public void Send(MessageTypes type, string json)
        {
            var text = Utils.Json.Serialize(type, json);
            Console.WriteLine(text);
            Send(text);
        }

        public void SendPlain(string msg)
        {
            Console.WriteLine(msg);
            Send(msg);
        }

        public void SendOk(MessageTypes type)
        {
            Send(Utils.Json.Serialize(type, "Ok"));
        }

        public void SendFail(MessageTypes type)
        {
            Send(Utils.Json.Serialize(type, "Fail"));
        }

        public void SetGameInfo(User player, Room room)
        {
            User = player;
            Room = room;
        }

        public void LeaveRoom() { User = null; Room = null; }

        protected override void OnOpen()
        {
            SessionId = GetId;
            if (RoomServer.AddSession(this) == false)
            {
                // If adding session is failed, disconnect the client.
                Sessions.CloseSession(ID);
            }
            else
            {
                // TEMP
                Console.WriteLine($"A client is connected: {SessionId}, {ID}");
            }
        }

        protected override void OnClose(CloseEventArgs e)
        {
            if (Joined)
            {
                RoomManager.LeaveRoom(Room!.RoomId, User!.UserId, out var info);
                Room.Broadcast(Utils.Json.Serialize(MessageTypes.PlayerLeave,
                new WebUserInfo() { UserId = info!.UserId, UserName = info!.UserName }));
                // TODO: Notify new host to client?
                LeaveRoom();
            }
            RoomServer.RemoveSession(SessionId);
            Console.WriteLine($"Client is disconnected: {SessionId}, {ID} by {e}");
        }

        protected override void OnMessage(MessageEventArgs e)
        {
            Console.WriteLine(e.Data);

            var obj = JObject.Parse(e.Data);
            string? str = (string?)obj["type"];
            string? data = (string?)obj["data"];

            if (str == null
                || int.TryParse(str, out var type) == false
                || data == null)
            {
                // wrong message
                Console.WriteLine("Wrong message: " + e.Data);
                return;
            }
            else
            {
                MessageHandler.Dispatch(type, this, data);
            }
        }


    }
}