using Microsoft.EntityFrameworkCore;
using VSNWebServer.DB;

namespace VSNWebServer
{
    public class StartUp(IConfiguration configuration)
    {

        public IConfiguration Configuration { get; set; } = configuration;

        public void ConfigureServices(IServiceCollection services)
        {
            services.AddControllers().AddJsonOptions(config =>
            {
                config.JsonSerializerOptions.PropertyNamingPolicy = null;
                config.JsonSerializerOptions.DictionaryKeyPolicy = null;
            });

            //services.AddDbContext<AppDbContext>(
            //    options
            //    => options.UseNpgsql(Configuration.GetConnectionString("DefaultConnection")));

            //services.AddSingleton<GameDbContext>();

        }

        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }
            else
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }
            app.UseHttpsRedirection();
            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthorization();
        }
    }
}
