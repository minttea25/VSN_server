using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using VSNWebServer.DB;
using VSNWebServer.Packets;

namespace VSNWebServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class RoomController : ControllerBase
    {
        [HttpPost("join")]
        public IActionResult JoinRoom([FromBody] WebRoomInfo req)
        {
            _ = RoomManager.GetRoom(req.RoomId, true);

            if (RoomManager.JoinRoom(req.RoomId, req.AccountDbId))
            {
                RoomManager.NotifyChangesToClients(req.RoomId);
                return Ok();
            }

            else return BadRequest();
        }

        [HttpPost("leave")]
        public IActionResult LeaveRoom([FromBody] WebRoomInfo req)
        {
            if (RoomManager.LeaveRoom(req.RoomId, req.AccountDbId))
            {
                RoomManager.NotifyChangesToClients(req.RoomId);
                return Ok();
            }
            else return BadRequest();
        }

        [HttpPost("ready")]
        public IActionResult SetReady([FromBody] WebRoomInfo req)
        {
            Room? room = RoomManager.GetRoom(req.RoomId, false);
            if (room == null) return BadRequest();

            if (room.UserReady(req.AccountDbId, true) == false)
            {
                return BadRequest();
            }

            RoomManager.NotifyChangesToClients(req.RoomId);
            return Ok();
        }

        [HttpPost("status/{roomId}")]
        public async Task<IActionResult> GetRoomStatus(uint roomId)
        {
            Room? room = RoomManager.GetRoom(roomId, false);
            if (room == null) return BadRequest();

            await room.WaitRoomStatusChanged();

            // 1. Client에서 해당 Post 요청 
            // 2. 해당 room에 대한 다른 변화(changes)가 있을 때까지 await (await room.WaitRoomStatusChanged)
            // 3. 변경 있을 경우 해당 요청에 대한 응답 전송
            // 응답에는 플레이어의 상태(현재 방의 플레이어, ready 상태, 이름), 방 이름, 모두 ready인지 포함
            return Ok(new WebRoomStatus()
            {
                Players = room.GetUserInfo(),
                RoomId = room.RoomId,
                RoomName = room.RoomName,
                AllReady = room.UsersAllReady(),
            });
        }
    }
}
