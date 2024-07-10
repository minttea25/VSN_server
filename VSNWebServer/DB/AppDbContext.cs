using Microsoft.EntityFrameworkCore;
using VSNWebServer.Data;

namespace VSNWebServer.DB
{
    public class AppDbContext(DbContextOptions<AppDbContext> options) : DbContext(options)
    {
        public DbSet<AccountDb> Accounts { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            modelBuilder.Entity<AccountDb>()
                .HasIndex(a => a.DbId)
                .IsUnique();
        }
    }
}
