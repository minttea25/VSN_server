using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using VSNWebServer.DB;
using VSNWebServer.Packets;

namespace VSNWebServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class RoomController(GameDbContext context) : ControllerBase
    {
        private readonly GameDbContext _redisContext = context;

        private const int MAX_PLAYER_IN_A_ROOM = 4;

        [HttpPost("enter")]
        public async Task<IActionResult> EnterRoom([FromBody] WebRoomInfo req)
        {
            var roomKey = $"room:{req.RoomId}";
            var playerCount = await _redisContext.Database.HashLengthAsync(roomKey);

            if (playerCount >= MAX_PLAYER_IN_A_ROOM) return BadRequest("Room is full.");

            await _redisContext.Database.HashSetAsync(roomKey, req.AccountDbId, "not ready");
            return Ok();
        }

        [HttpPost("leave")]
        public async Task<IActionResult> LeaveRoom([FromBody] WebRoomInfo req)
        {
            var roomKey = $"room:{req.RoomId}";

            await _redisContext.Database.HashDeleteAsync(roomKey, req.AccountDbId);
            return Ok();
        }

        [HttpPost("ready")]
        public async Task<IActionResult> SetReady([FromBody] WebRoomInfo req)
        {
            var roomKey = $"room:{req.RoomId}";
            var playerStatus = await _redisContext.Database.HashGetAsync(roomKey, req.AccountDbId);

            if (playerStatus.IsNullOrEmpty)
            {
                return BadRequest("Player not in room.");
            }

            await _redisContext.Database.HashSetAsync(roomKey, req.AccountDbId, "ready");

            // 모든 플레이어가 준비 상태인지 확인
            var allReady = true;
            var players = await _redisContext.Database.HashGetAllAsync(roomKey);
            foreach (var player in players)
            {
                if (player.Value != "ready")
                {
                    allReady = false;
                    break;
                }
            }

            if (allReady)
            {
                // TODO: 모든 플레이어가 준비 상태일 때 게임 서버에 신호를 보내는 로직을 추가
            }

            return Ok();
        }
    }
}
