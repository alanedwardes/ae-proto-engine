#pragma once

class IServerNetworked
{
public:
	virtual std::string Serialise() = 0;
};

class IClientNetworked
{
public:
	virtual void Unserialise(std::string szSerialised) = 0;
};

class ISharedNetworked : public IClientNetworked, public IServerNetworked
{
public:
	virtual std::string Serialise() = 0;
	virtual void Unserialise(std::string szSerialised) = 0;
};