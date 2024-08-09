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

		_sessions.insert(session);
	}

	void RemoveSession(std::shared_ptr<GameSession> session)
	{
		NetCore::WriteLock l(__lock);

		_sessions.erase(session);
	}

	void FlushSessions()
	{
		for (auto& s : _sessions) s->Flush();
	}

private:
	NetCore::RWLock __lock;
	NetCore::Set<std::shared_ptr<GameSession>> _sessions;

	friend class GameManager;
};

