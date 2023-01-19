#pragma once
#include <string>
#include <map>

struct shPlayerData {
	std::string name;
	long serverid, userid;
	float cachedtime, x, y, z, vx, vy, vz;

	shPlayerData(unsigned long sid, unsigned long uid, float time,
		float _x, float _y, float _z,
		float _vx, float _vy, float _vz, std::string _name)
		: serverid(sid), userid(uid), cachedtime(time), x(_x), y(_y), z(_z), vx(_vx), vy(_vy), vz(_vz), name(_name) { }

	shPlayerData() { }

	/// <summary>
	/// will serialize instance leaving beginning 8 bytes as placeholder
	/// </summary>
	/// <returns>serialized instance as char array</returns>
	char* serialize() {
		char packet[256];
		std::string sid = std::to_string(serverid);
		std::string uid = std::to_string(userid);
		std::string time = std::to_string(cachedtime);
		std::string sx = std::to_string(x);
		std::string sy = std::to_string(y);
		std::string sz = std::to_string(z);
		std::string svx = std::to_string(vx);
		std::string svy = std::to_string(vy);
		std::string svz = std::to_string(vz);

		for (size_t i = 0; i < 255; i++) {
			packet[i] = '\xA6';
		}

		int ix = 7;
		for (size_t i = 0; i < sid.size() - 1; i++) {
			packet[ix++] = sid[i];
		}

		ix = 31;
		for (size_t i = 0; i < uid.size() - 1; i++) {
			packet[ix++] = uid[i];
		}

		ix = 63;
		for (size_t i = 0; i < time.size() - 1; i++) {
			packet[ix++] = time[i];
		}

		ix = 95;
		for (size_t i = 0; i < sx.size() - 1; i++) {
			packet[ix++] = sx[i];
		}

		ix = 111;
		for (size_t i = 0; i < sy.size() - 1; i++) {
			packet[ix++] = sy[i];
		}

		ix = 127;
		for (size_t i = 0; i < sz.size() - 1; i++) {
			packet[ix++] = sz[i];
		}

		ix = 143;
		for (size_t i = 0; i < svx.size() - 1; i++) {
			packet[ix++] = svx[i];
		}

		ix = 159;
		for (size_t i = 0; i < svy.size() - 1; i++) {
			packet[ix++] = svy[i];
		}

		ix = 175;
		for (size_t i = 0; i < svz.size() - 1; i++) {
			packet[ix++] = svz[i];
		}

		ix = 191;
		for (size_t i = 0; i < name.size() - 1; i++) {
			packet[ix++] = name[i];
		}
	}

	/// <summary>
	/// will deserialize a packet into a shPlayerData ptr
	/// </summary>
	/// <param name="packet">packet to be interpreted</param>
	/// <returns>pointer to instance of shPlayerData</returns>
	static shPlayerData* deserialize(char* packet) {
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

		return new shPlayerData(std::stol(sid), 
								std::stol(uid), 
								std::stof(time), 
								std::stof(_x), 
								std::stof(_y), 
								std::stof(_z),
								std::stof(_vx), 
								std::stof(_vy), 
								std::stof(_vz),
								_name);
	}
};

std::map<long, shPlayerData*> shared_players;

