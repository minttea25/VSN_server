#pragma once

class GameSession;

class SessionManager
{
public:
	SessionManager();
	~SessionManager();

	/*inline-methods*/
	std::shared_ptr<NetCore::Session> SessionFactory()
	{
		return NetCore::ObjectPool<GameSession>::make_shared();
	}

	void AddConnectedSession(std::shared_ptr<GameSession> session)
	{
		NetCore::WriteLock l(__lock);

		_sessions.insert({ session->SessionId(), session });
	}

	void RemoveSession(uint sessionId)
	{
		NetCore::WriteLock l(__lock);

		_sessions.erase(sessionId);
	}

	void FlushSessions()
	{
		for (auto& kv : _sessions)
		{
			kv.second->Flush();
		}
	}

private:
	NetCore::RWLock __lock;
	NetCore::HashMap<uint, std::shared_ptr<GameSession>> _sessions;

	friend class GameManager;
};

