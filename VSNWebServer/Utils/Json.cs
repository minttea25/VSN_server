using Newtonsoft.Json.Linq;
using System.Runtime.CompilerServices;
using VSNWebServer.Packets;
using VSNWebServer.RoomServers;

namespace VSNWebServer.Utils
{
    public class Json
    {
        public struct JsonObject
        {
            public int type;
            public string data;
        }


        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static string Serialize<T>(T data) where T : IWebResponse
        {
            return Newtonsoft.Json.JsonConvert.SerializeObject(data);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static string Serialize(MessageTypes type, string data)
        {
            return JObject.FromObject(new JsonObject() { type = (int)type, data = data }).ToString();
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static string Serialize<T>(MessageTypes type, T data) where T : IWebResponse
        {
            var json = Serialize(data);
            return JObject.FromObject(new JsonObject() { type = (int)type, data = json }).ToString();
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static T? Deserialize<T>(string json)
        {
            return Newtonsoft.Json.JsonConvert.DeserializeObject<T>(json);
        }

        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public static bool TryDeserialize<T>(string json, out T? data)
        {
            data = Newtonsoft.Json.JsonConvert.DeserializeObject<T>(json);
            if (data == null) return false;
            else return true;
        }
    }
}