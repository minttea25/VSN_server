using Microsoft.AspNetCore.Mvc;
using VSNWebServer.Packets;

namespace VSNWebServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class TestController : ControllerBase
    {
        static uint TestAccountDbId = 900;
        public Tuple<uint, string> GetTestAccountName()
        {
            var id = TestAccountDbId++;
            return Tuple.Create(id, $"TestName{id}");
        }



        [HttpPost("login")]
        public IActionResult TestLogin([FromBody] WebLoginReq req)
        {
            var u = GetTestAccountName();
            return Ok(new WebLoginRes() { AccountName = u.Item2, AccountDbId = u.Item1, });
        }
    }
}