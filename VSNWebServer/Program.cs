using Google.FlatBuffers;
using VSN;
using VSNWebServer;
using VSNWebServer.GameServer;
using VSNWebServer.RoomServers;
using WebSocketSharp.Server;

const string GAMESERVER_IP = "127.0.0.1";
const ushort GAMESERVER_PORT = 8070;

Thread.Sleep(1000);

GameServerClientHelper.Init(GAMESERVER_IP, GAMESERVER_PORT);
await GameServerClientHelper.Instance!.ConnectAsync();

_ = RoomServer.Start("ws://127.0.0.1:8071", "/room");

var builder = Host.CreateDefaultBuilder(args)
    .ConfigureWebHostDefaults(config =>
    {
        config.UseStartup<StartUp>();
    });

builder.Build().Run();



//var builder = WebApplication.CreateBuilder(args);

//// Add services to the container.

//builder.Services.AddControllers();
//// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
//builder.Services.AddEndpointsApiExplorer();
//builder.Services.AddSwaggerGen();

//var app = builder.Build();

//// Configure the HTTP request pipeline.
//if (app.Environment.IsDevelopment())
//{
//    app.UseSwagger();
//    app.UseSwaggerUI();
//}

//app.UseHttpsRedirection();

//app.UseAuthorization();

//app.MapControllers();

//app.Run();
