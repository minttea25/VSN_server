using Microsoft.AspNetCore.Mvc;
using VSNWebServer.Packets;

namespace VSNWebServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class RoomController : ControllerBase
    {

        [HttpPost("info")]
        public IActionResult ReqRoomInfo([FromBody] WebRoomReq req)
        {
            if (req.Verify() == false) return BadRequest();

            Room? room = RoomManager.GetRoom(req.RoomId, false);
            if (room == null) return BadRequest();

            return Ok(room.GetRoomInfo());
        }

        [HttpPost("name/{room_id}")]
        public IActionResult GetRoomName(uint room_id)
        {
            Room? room = RoomManager.GetRoom(room_id, false);
            if (room != null) return Ok(room.RoomName);
            else return BadRequest();
        }

        [HttpPost("player/{room_id}/{user_index}")]
        public IActionResult GetPlayerInfo(uint room_id, uint user_index)
        {
            Room? room = RoomManager.GetRoom(room_id, false);
            if (room == null) return BadRequest();
            try
            {
                User user = room.Users[user_index];
                if (user.Valid) return Ok(new WebUserInfo(user));
                else return BadRequest();
            }
            catch (Exception)
            {
                return BadRequest();
            }
        }
    }
}