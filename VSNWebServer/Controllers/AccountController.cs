using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using VSNWebServer.Data;
using VSNWebServer.DB;
using VSNWebServer.Packets;
using VSNWebServer.Utils;

namespace VSNWebServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class AccountController(AppDbContext context) : ControllerBase
    {
        private readonly AppDbContext _context = context;

        [HttpPost("register")]
        public async Task<IActionResult> Register([FromBody] AccountDb account)
        {
            // TODO : Verify the new information of account

            if (!Verification.VerifyRegister(account))
            {
                // TODO : error msg
                return BadRequest();
            }

            _context.Accounts.Add(account);
            await _context.SaveChangesAsync();

            // TEMP
            return Ok(account);
        }

        [HttpPost("login")]
        public async Task<IActionResult> Login([FromBody] AccountDb account)
        {
            var user = await _context.Accounts.SingleOrDefaultAsync(a => a.AccountId == account.AccountId);
            if (user == null || Security.VerifyPassword(account.AccountPasswordHash, user.AccountPasswordHash))
            {
                return BadRequest();
            }

            // TODO : create and return token
            return Ok();
        }

        [HttpPost("logout")]
        public async Task<IActionResult> Logout([FromBody] WebLogoutReqPacket req)
        {
            var user = await _context.Accounts.SingleOrDefaultAsync(a => a.DbId == req.AccountDbId);
            if (user == null) return BadRequest();
            else return Ok();
        }
    }
}
