#pragma once

#include <string>
#include "Vector.h"
#include "Point.h"
#include "jsoncpp\json.h"

class Manifest
{
public:
	Manifest() {};
	Manifest(Json::Value oManifest){ m_oManifest = oManifest; }

	virtual bool ReadManifest(std::string szFilename);
	virtual void WriteManifest(std::string szFilename = std::string());

	virtual bool GetBool(std::string szKey, bool bDefault = false);
	virtual void SetBool(std::string szKey, bool bValue);

	virtual int GetInt(std::string szKey, int iDefault = 0);
	virtual void SetInt(std::string szKey, int iValue);

	virtual float GetFloat(std::string szKey, float flDefault = 0.0f);
	virtual void SetFloat(std::string szKey, float flValue);

	virtual std::string GetString(std::string szKey, std::string szDefault = std::string());
	virtual void SetString(std::string szKey, std::string szValue);

	virtual std::string GetFolder(std::string szKey, std::string szDefault = std::string());
	virtual std::string GetFile(std::string szKey, std::string szDefault = std::string());

	virtual Vector GetVector(std::string szKey, Vector veDefault = Vector());
	virtual void SetVector(std::string szKey, Vector veValue);

	virtual Point GetPoint(std::string szKey, Point poDefault = Point());
	virtual std::vector<Point> GetPointList(std::string szKey);
	virtual void SetPoint(std::string szKey, Point poValue);

	virtual Manifest GetManifest(std::string szKey);
	virtual void SetManifest(std::string szKey, Manifest oManifest);

	virtual Manifest GetIncludedManifest(std::string szKey, std::string szPath = std::string());

	virtual std::vector<Manifest> GetManifestList(std::string szKey);
	virtual void SetManifestList(std::string szKey, std::vector<Manifest> oManifestList);
private:
	std::string m_szPath;
	Json::Value m_oManifest;
};