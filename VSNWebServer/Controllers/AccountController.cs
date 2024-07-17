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
        public async Task<IActionResult> Register([FromBody] WebRegisterReq req)
        {
            if (req.Verify() == false) return BadRequest();
            // TODO : Verify the new information of account

            if (!Verification.VerifyRegister(req))
            {
                // TODO : error msg
                return BadRequest();
            }

            _context.Accounts.Add(new AccountDb() { AccountId = req.AccountId, AccountPasswordHash = req.PasswordHash, AccountName = req.AccountName });
            await _context.SaveChangesAsync();

            // TEMP
            return Ok();
        }

        [HttpPost("login")]
        public async Task<IActionResult> Login([FromBody] WebLoginReq req)
        {
            if (req.Verify() == false) return BadRequest();

            var account = await _context.Accounts.SingleOrDefaultAsync(a => a.AccountId == req.AccountId && a.AccountPasswordHash == req.PasswordHash);
            if (account == null || Security.VerifyPassword(req.PasswordHash, account.AccountPasswordHash))
            {
                return BadRequest();
            }

            // TODO : create and return token
            return Ok(new WebLoginRes() { AccountName = account.AccountName, AccountDbId = account.DbId, });
        }

        [HttpPost("logout")]
        public async Task<IActionResult> Logout([FromBody] WebLogoutReq req)
        {
            if (req.Verify() == false) return BadRequest();

            var user = await _context.Accounts.SingleOrDefaultAsync(a => a.DbId == req.AccountDbId);
            if (user == null) return BadRequest();
            else return Ok();
        }

        [HttpPost("login_test")]
        public IActionResult TestLogin([FromBody] WebLoginReq req)
        {
            return Ok(new WebLoginRes() { AccountName = "Test12345678", AccountDbId = 12345678, });
        }
    }
}
