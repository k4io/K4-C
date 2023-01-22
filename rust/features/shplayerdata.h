#include <string>
#include <vector>

struct shPlayerData {
	std::string name, serverip, servername;
	int serverport, ingame;
	long serverid, userid;
	float cachedtime, x, y, z, vx, vy, vz;
	std::vector<shPlayerData*> rendered_players{};


	shPlayerData(unsigned long sid, unsigned long uid, float time,
		float _x, float _y, float _z,
		float _vx, float _vy, float _vz,
		std::string _name, std::string sip,
		std::string sname, int port, bool ig)
		: serverid(sid), userid(uid), cachedtime(time),
		x(_x), y(_y), z(_z),
		vx(_vx), vy(_vy), vz(_vz),
		name(_name), serverip(sip),
		servername(sname), serverport(port), ingame(ig) { }

	shPlayerData()
		: serverid(0), userid(0), cachedtime(0),
		x(0), y(0), z(0),
		vx(0), vy(0), vz(0),
		name("NOTSET"), serverip("NOTSET"),
		servername("NOTSET"), serverport(0), ingame(0) { }

	/// <summary>
	/// will serialize instance leaving beginning 8 bytes as placeholder
	/// </summary>
	/// <returns>serialized instance as string</returns>
	std::string serialize() {
		char packet[512];
		std::string sid = std::to_string(serverid);
		std::string uid = std::to_string(userid);
		std::string time = std::to_string(cachedtime);
		std::string sx = std::to_string(x);
		std::string sy = std::to_string(y);
		std::string sz = std::to_string(z);
		std::string svx = std::to_string(vx);
		std::string svy = std::to_string(vy);
		std::string svz = std::to_string(vz);
		std::string servport = std::to_string(serverport);

		for (size_t i = 0; i < 512; i++) {
			packet[i] = '\xA6';
		}

		packet[5] = ingame ? 't' : 'f';

		int ix = 7;
		for (size_t i = 0; i < sid.size(); i++) {
			packet[ix++] = sid[i];
		}

		ix = 31;
		for (size_t i = 0; i < uid.size(); i++) {
			packet[ix++] = uid[i];
		}

		ix = 63;
		for (size_t i = 0; i < time.size(); i++) {
			packet[ix++] = time[i];
		}

		ix = 95;
		for (size_t i = 0; i < sx.size(); i++) {
			packet[ix++] = sx[i];
		}

		ix = 111;
		for (size_t i = 0; i < sy.size(); i++) {
			packet[ix++] = sy[i];
		}

		ix = 127;
		for (size_t i = 0; i < sz.size(); i++) {
			packet[ix++] = sz[i];
		}

		ix = 143;
		for (size_t i = 0; i < svx.size(); i++) {
			packet[ix++] = svx[i];
		}

		ix = 159;
		for (size_t i = 0; i < svy.size(); i++) {
			packet[ix++] = svy[i];
		}

		ix = 175;
		for (size_t i = 0; i < svz.size(); i++) {
			packet[ix++] = svz[i];
		}

		ix = 191;
		for (size_t i = 0; i < name.size(); i++) {
			packet[ix++] = name[i];
		}
		/*
		std::string servport = std::to_string(serverport);
		std::string servip = serverip;
		std::string servname = servername;
		*/

		ix = 223;
		for (size_t i = 0; i < servport.size(); i++) {
			packet[ix++] = servport[i];
		}

		//max port len = 5
		ix = 228;
		for (size_t i = 0; i < serverip.size(); i++) {
			packet[ix++] = serverip[i];
		}

		//max connect addr len = 64
		ix = 292;
		for (size_t i = 0; i < servername.size(); i++) {
			packet[ix++] = servername[i];
		}

		return packet;
	}

	/// <summary>
	/// will serialize specified instance of renderer player leaving beginning 8 bytes as placeholder
	/// </summary>
	/// <returns>serialized instance as string</returns>
	std::string serialize_rendered_player(int idx) {
		char packet[512];
		auto m = rendered_players[idx];
		std::string sid = std::to_string(m->serverid);
		std::string uid = std::to_string(m->userid);
		std::string time = std::to_string(m->cachedtime);
		std::string sx = std::to_string(m->x);
		std::string sy = std::to_string(m->y);
		std::string sz = std::to_string(m->z);
		std::string svx = std::to_string(m->vx);
		std::string svy = std::to_string(m->vy);
		std::string svz = std::to_string(m->vz);
		std::string servport = std::to_string(m->serverport);

		for (size_t i = 0; i < 512; i++) {
			packet[i] = '\xA6';
		}

		packet[5] = ingame ? 't' : 'f';

		int ix = 7;
		for (size_t i = 0; i < sid.size(); i++) {
			packet[ix++] = sid[i];
		}

		ix = 31;
		for (size_t i = 0; i < uid.size(); i++) {
			packet[ix++] = uid[i];
		}

		ix = 63;
		for (size_t i = 0; i < time.size(); i++) {
			packet[ix++] = time[i];
		}

		ix = 95;
		for (size_t i = 0; i < sx.size(); i++) {
			packet[ix++] = sx[i];
		}

		ix = 111;
		for (size_t i = 0; i < sy.size(); i++) {
			packet[ix++] = sy[i];
		}

		ix = 127;
		for (size_t i = 0; i < sz.size(); i++) {
			packet[ix++] = sz[i];
		}

		ix = 143;
		for (size_t i = 0; i < svx.size(); i++) {
			packet[ix++] = svx[i];
		}

		ix = 159;
		for (size_t i = 0; i < svy.size(); i++) {
			packet[ix++] = svy[i];
		}

		ix = 175;
		for (size_t i = 0; i < svz.size(); i++) {
			packet[ix++] = svz[i];
		}

		ix = 191;
		for (size_t i = 0; i < name.size(); i++) {
			packet[ix++] = name[i];
		}

		ix = 223;
		for (size_t i = 0; i < servport.size(); i++) {
			packet[ix++] = servport[i];
		}

		//max port len = 5
		ix = 228;
		for (size_t i = 0; i < serverip.size(); i++) {
			packet[ix++] = serverip[i];
		}

		//max connect addr len = 64
		ix = 292;
		for (size_t i = 0; i < servername.size(); i++) {
			packet[ix++] = servername[i];
		}

		return packet;
	}

	/// <summary>
	/// will deserialize a packet into a shPlayerData ptr
	/// </summary>
	/// <param name="packet">packet to be interpreted</param>
	/// <returns>pointer to instance of shPlayerData</returns>
	static shPlayerData* deserialize(char* packet) {
		bool in_game = packet[5] == 't';

		std::string sid = "";
		for (size_t i = 7; i < 31; i++) {
			if (packet[i] == '\xA6') break;
			sid += packet[i];
		}

		std::string uid = "";
		for (size_t i = 31; i < 63; i++) {
			if (packet[i] == '\xA6') break;
			uid += packet[i];
		}

		std::string time = "";
		for (size_t i = 63; i < 95; i++) {
			if (packet[i] == '\xA6') break;
			time += packet[i];
		}

		std::string _x = "";
		for (size_t i = 95; i < 111; i++) {
			if (packet[i] == '\xA6') break;
			_x += packet[i];
		}

		std::string _y = "";
		for (size_t i = 111; i < 127; i++) {
			if (packet[i] == '\xA6') break;
			_y += packet[i];
		}

		std::string _z = "";
		for (size_t i = 127; i < 143; i++) {
			if (packet[i] == '\xA6') break;
			_z += packet[i];
		}

		std::string _vx = "";
		for (size_t i = 143; i < 159; i++) {
			if (packet[i] == '\xA6') break;
			_vx += packet[i];
		}

		std::string _vy = "";
		for (size_t i = 159; i < 175; i++) {
			if (packet[i] == '\xA6') break;
			_vy += packet[i];
		}

		std::string _vz = "";
		for (size_t i = 175; i < 191; i++) {
			if (packet[i] == '\xA6') break;
			_vz += packet[i];
		}

		std::string _name = "";
		for (size_t i = 191; i < 223; i++) {
			if (packet[i] == '\xA6') break;
			_name += packet[i];
		}

		std::string servport = "";
		for (size_t i = 223; i < 228; i++) {
			if (packet[i] == '\xA6') break;
			servport += packet[i];
		}

		std::string servip = "";
		for (size_t i = 228; i < 292; i++) {
			if (packet[i] == '\xA6') break;
			servip += packet[i];
		}

		std::string servname = "";
		for (size_t i = 292; i < 356; i++) {
			if (packet[i] == '\xA6') break;
			servname += packet[i];
		}

		return new shPlayerData(std::stol(sid),
			std::stol(uid),
			std::stof(time),
			std::stof(_x),
			std::stof(_y),
			std::stof(_z),
			std::stof(_vx),
			std::stof(_vy),
			std::stof(_vz),
			_name, servip,
			servname,
			stoi(servip),
			in_game);
	}
};