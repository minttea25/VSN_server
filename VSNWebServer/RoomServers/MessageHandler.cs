﻿using VSNWebServer.Packets;
using VSNWebServer.Utils;

namespace VSNWebServer.RoomServers
{
    public enum MessageTypes : int
    {
        // routes
        ReqJoinRoom = 1,
        ReqLeaveRoom = 2,
        ReqReady = 3,
        ReqSendChat = 4,
        PlayerJoin = 5,
        PlayerLeave = 6,

        GetRoomName = 11,
        GetRoomInfo = 12,
        GetPlayerInfo = 13,

        ChangeRoomName = 101,

        // for clients
        RecvChat = 1001,
        PlayerReady = 1002,

        Announcement = 10001,

        Ok = 100001,
        Bad = 100002,

        GameStart = 9999999,
    }

    /// <summary>
    /// Note: If request is successful, the response will be sent in this class, 
    /// but not successful, it returns only false.
    /// </summary>
    public static class MessageHandler
    {
        public static IReadOnlyDictionary<int, Func<WebSession, string, bool>> Handlers => _handlers;
        static readonly Dictionary<int, Func<WebSession, string, bool>> _handlers = new();

        public static void Init()
        {
            _handlers.Add((int)MessageTypes.ReqJoinRoom, JoinRoom);
            _handlers.Add((int)MessageTypes.ReqLeaveRoom, LeaveRoom);
            _handlers.Add((int)MessageTypes.ReqReady, UserReady);
            _handlers.Add((int)MessageTypes.GetRoomName, GetRoomName);
            _handlers.Add((int)MessageTypes.GetRoomInfo, GetRoomInfo);
            _handlers.Add((int)MessageTypes.GetPlayerInfo, GetPlayerInfo);
            _handlers.Add((int)MessageTypes.ChangeRoomName, ChangeRoomName);
            _handlers.Add((int)MessageTypes.ReqSendChat, SendChat);
            // TODO : add more
        }

        public static void Dispatch(int type, WebSession session, string json)
        {
            if (_handlers.TryGetValue(type, out var func))
            {
                func.Invoke(session, json);
            }
            else
            {
                // error
                Console.WriteLine("Wrong type: " + type);
            }
        }

        public static bool JoinRoom(WebSession session, string json)
        {
            if (Utils.Json.TryDeserialize(json, out WebRoomReq? req) == false)
            {
                Console.WriteLine($"[JoinRoom] Failed to deserialize data: {json}");
                session.SendFail(MessageTypes.ReqJoinRoom);
                return false;
            }

            if (req!.Verify() == false)
            {
                Console.WriteLine($"[JoinRoom] Req is not valid: {json}");
                session.SendFail(MessageTypes.ReqJoinRoom);
                return false;
            }

            var room = RoomManager.GetRoom(req.RoomId, true);

            if (RoomManager.JoinRoom(req.RoomId, req.AccountDbId, req.UserName, session) == false)
            {
                Console.WriteLine($"[JoinRoom]Failed to join room: {req.RoomId}");
                session.SendFail(MessageTypes.ReqJoinRoom);
                return false;
            }

            var player = room!.GetPlayer(req.AccountDbId);
            if (player == null)
            {
                // internal error
                Console.WriteLine($"[JoinRoom] Internal Error.");
                session.SendFail(MessageTypes.ReqJoinRoom);
                return false;
            }
            session.SetGameInfo(player!, room!);

            // Send to ok to sender
            session.SendOk(MessageTypes.ReqJoinRoom);

            // TODO: Change to config text. (now it is hard coded)
            // Send msg to users
            //room!.Announce($"{req.UserName} joined the room.");
            room!.Broadcast(Utils.Json.Serialize(MessageTypes.PlayerJoin,
                new WebUserInfo()
                {
                    UserId = req.AccountDbId,
                    UserName = req.UserName
                }));

            return true;
        }

        public static bool LeaveRoom(WebSession session, string json)
        {
            if (session.Joined == false)
            {
                Console.WriteLine($"[LeaveRoom] This session is not in room.");
                session.SendFail(MessageTypes.ReqLeaveRoom);
                return false;
            }

            if (Utils.Json.TryDeserialize(json, out WebRoomReq? req) == false)
            {
                Console.WriteLine($"[LeaveRoom] Failed to deserialize data: {json}");
                session.SendFail(MessageTypes.ReqLeaveRoom);
                return false;
            }

            if (req!.Verify() == false)
            {
                Console.WriteLine($"[LeaveRoom] Req is not valid: {json}");
                session.SendFail(MessageTypes.ReqLeaveRoom);
                return false;
            }

            if (RoomManager.LeaveRoom(req!.RoomId, req.AccountDbId, out var info) == false)
            {
                Console.WriteLine($"[LeaveRoom] Failed to leave room: {req.RoomId}");
                session.SendFail(MessageTypes.ReqLeaveRoom);
                return false;
            }

            // TODO: Change to config text. (now it is hard coded)
            // Send msg to users
            //session.Room!.Announce($"{req.UserName} leaved the room.");
            session.Room!.Broadcast(Utils.Json.Serialize(MessageTypes.PlayerLeave,
                new WebUserInfo() { UserId = info!.UserId, UserName = info!.UserName }));
            // TODO: Notify new host to client?
            session.LeaveRoom();

            // Send to ok to sender
            session.SendOk(MessageTypes.ReqLeaveRoom);

            return true;
        }

        public static bool UserReady(WebSession session, string json)
        {
            if (session.Joined == false)
            {
                Console.WriteLine($"[UserReady] This session is not in room.");
                return false;
            }
            if (Utils.Json.TryDeserialize(json, out WebRoomReq? req) == false)
            {
                Console.WriteLine($"[UserReady] Failed to deserialize data: {json}");
                return false;
            }

            if (req!.Verify() == false)
            {
                Console.WriteLine($"[UserReady] Req is not valid: {json}");
                return false;
            }

            if (session.Room!.RoomId != req.RoomId)
            {
                Console.WriteLine($"[UserReady] The request does not match room info.");
                return false;
            }

            if (session.Room.UserReady(req.AccountDbId, true) == false)
            {
                Console.WriteLine($"[UserReady] Failed to ready in room: {req.RoomId}");
                return false;
            }

            // Response to sender
            session.SendOk(MessageTypes.ReqReady);

            // Send changes to all
            session.Room.Broadcast(Utils.Json.Serialize(MessageTypes.PlayerReady,
                new WebUserReady()
                {
                    UserId = session.User!.UserId,
                    IsReady = true,
                    RoomId = session.Room!.RoomId,
                }));

            // Check all players are ready and send game start
            if (session.Room!.UsersAllReady() == true)
            {
                // TEMP
                Console.WriteLine($"All players in Room [id={session.Room!.RoomId}] are ready.");
                session.Room!.Broadcast(Utils.Json.Serialize(MessageTypes.GameStart, string.Empty));
            }

            return true;
        }

        public static bool GetRoomName(WebSession session, string json)
        {
            // Data: { "{roomid}" }
            if (uint.TryParse(json, out uint roomId) == false)
            {
                Console.WriteLine($"[GetRoomName] Failed to get room name with wrong data: {json}");
                return false;
            }

            Room? room = RoomManager.GetRoom(roomId, false);
            if (room == null)
            {
                Console.WriteLine($"[GetRoomName] Failed to get room name with no_such_room({roomId}.)"); ;
                return false;
            }

            // send roomname to the client
            session.Send(MessageTypes.GetRoomName, room.RoomName);
            return true;
        }

        public static bool GetRoomInfo(WebSession session, string json)
        {
            if (Utils.Json.TryDeserialize(json, out WebRoomReq? req) == false)
            {
                Console.WriteLine($"[GetRoomInfo] Failed to deserialize data: {json}");
                return false;
            }

            if (req!.Verify() == false)
            {
                Console.WriteLine($"[GetRoomInfo] Req is not valid: {json}");
                return false;
            }

            Room? room = RoomManager.GetRoom(req!.RoomId, false);
            if (room == null)
            {
                Console.WriteLine($"[GetRoomInfo] There is no room: {req.RoomId}");
                return false;
            }

            // send room info to client
            session.Send(MessageTypes.GetRoomInfo, Utils.Json.Serialize(room.GetRoomInfo()));
            return true;
        }

        public static bool GetPlayerInfo(WebSession session, string json)
        {
            // Data: { room_id: "{RoomId}", PlayerIndex: "{index}" }
            if (Utils.Json.TryDeserialize(json, out WebRoomPlayerInfoReq? req) == false)
            {
                Console.WriteLine($"[GetPlayerInfo] Failed to deserialize data: {json}");
                return false;
            }

            if (req!.Verify() == false)
            {
                Console.WriteLine($"[GetPlayerInfo] Req is not valid: {json}");
                return false;
            }

            Room? room = RoomManager.GetRoom(req.RoomId, false);
            if (room == null)
            {
                Console.WriteLine($"[GetRoomInfo] There is no room: {req.RoomId}");
                return false;
            }

            // Send player info to the client
            User user = room.Users[req.PlayerIndex];
            session.Send(MessageTypes.GetPlayerInfo, Utils.Json.Serialize(new WebUserInfo(user)));
            return true;
        }

        public static bool ChangeRoomName(WebSession session, string json)
        {
            // Data:  "{roomName}" 

            if (session.Joined == false)
            {
                Console.WriteLine($"[ChangeRoomName] This session is not in room.");
                return false;
            }

            if (Verification.VerifyRoomName(json) == false)
            {
                Console.WriteLine($"[ChangeRoomName] Invalid room name or request. req: {json}");
                return false;
            }

            if (session.User!.IsHost == false)
            {
                Console.WriteLine($"[ChangeRoomName] Only host player can change the room name");
                return false;
            }

            // Change the room name
            session.Room!.ChangeRoomName(json);

            // Notify the changes to clients
            session.Room!.Broadcast(Utils.Json.Serialize(MessageTypes.ChangeRoomName, json));

            return true;
        }

        public static bool SendChat(WebSession session, string json)
        {
            if (session.Joined == false)
            {
                Console.WriteLine($"[SendChat] This session is not in room.");
                return false;
            }

            if (Utils.Json.TryDeserialize<WebChatData>(json, out var req) == false)
            {
                Console.WriteLine($"[SendChat] Failed to deserialize data: {json}");
                return false;
            }

            if (req!.Verify() == false)
            {
                Console.WriteLine($"[SendChat] Request is not valid: {json}");
                return false;
            }

            // Add chats at room
            session.Room!.AddChat(session.User!.UserId, DateTime.Now.ToFileTimeUtc(), req.ChatContent);

            // Broadcast recvd chat to clients
            session.Room!.Broadcast(Utils.Json.Serialize(MessageTypes.RecvChat,
                new WebChatData() { UserName = session.User!.UserName, ChatContent = req.ChatContent }));

            return true;
        }
    }
}